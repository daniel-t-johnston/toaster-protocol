/* ---------------------------------------------------------------------------
** I attest that this following code represents my own work and is subject to
** the plagiarism policy found in the course syllabus.
**
**       Class: CSC 242
**  Assignment: Lesson 5 / Part 1 / SpellCheck
**        File: SpellCheck.Test.cpp
** Description: Unit tests for SpellCheck functions.
**      Author: Afshin Bahrampour, Andre Gonzalez, Daniel Johnston
**        Date: 14 Feb 26
** -------------------------------------------------------------------------*/

#include "pch.h"
#include "CppUnitTest.h"
#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

// Include the spellcheck source directly so static functions are visible.
// Rename main to avoid a duplicate symbol.
#define main spellcheck_main_impl
#include "../SpellCheck/Source.cpp"
#undef main

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// ---------------------------------------------------------------------------
// Helper: write content to a file at the given path.
// ---------------------------------------------------------------------------
static void WriteFile(const std::string& path, const std::string& content)
{
    std::ofstream f(path);
    f << content;
}

namespace SpellCheckTest
{
    // ---------------------------------------------------------------------------
    // TEST CLASS: CleanWordTests
    // Verifies that clean_word() correctly strips non-alpha characters,
    // converts to lowercase, and preserves apostrophes.
    // ---------------------------------------------------------------------------
    TEST_CLASS(CleanWordTests)
    {
    public:

        TEST_METHOD(PlainLowercase_Unchanged)
        {
            Assert::AreEqual(std::string("hello"), clean_word("hello"));
        }

        TEST_METHOD(AllUppercase_ConvertedToLower)
        {
            Assert::AreEqual(std::string("upper"), clean_word("UPPER"));
            Assert::AreEqual(std::string("hello"), clean_word("Hello"));
        }

        TEST_METHOD(TrailingPunctuation_Stripped)
        {
            Assert::AreEqual(std::string("word"), clean_word("word."));
            Assert::AreEqual(std::string("word"), clean_word("word,"));
            Assert::AreEqual(std::string("word"), clean_word("word!"));
        }

        // Apostrophes are explicitly preserved by the implementation.
        TEST_METHOD(Apostrophe_Preserved)
        {
            Assert::AreEqual(std::string("don't"), clean_word("don't"));
            Assert::AreEqual(std::string("it's"),  clean_word("it's"));
            Assert::AreEqual(std::string("o'brien"), clean_word("O'Brien"));
        }

        TEST_METHOD(Digits_Stripped)
        {
            Assert::AreEqual(std::string("abc"),  clean_word("abc123"));
            Assert::AreEqual(std::string("abc"),  clean_word("123abc"));
            Assert::AreEqual(std::string("abc"),  clean_word("a1b2c3"));
        }

        // Hyphens are not alphabetic and not apostrophes, so they are removed.
        TEST_METHOD(Hyphen_Stripped)
        {
            Assert::AreEqual(std::string("firstclass"), clean_word("first-class"));
            Assert::AreEqual(std::string("welltodo"),   clean_word("well-to-do"));
        }

        TEST_METHOD(AllNonAlpha_ReturnsEmpty)
        {
            Assert::AreEqual(std::string(""), clean_word("123"));
            Assert::AreEqual(std::string(""), clean_word("!@#"));
            Assert::AreEqual(std::string(""), clean_word(""));
        }

        TEST_METHOD(MixedContent_OnlyAlphaAndApostropheKept)
        {
            Assert::AreEqual(std::string("abcdef"), clean_word("abc-123!def"));
        }
    };


    // ---------------------------------------------------------------------------
    // TEST CLASS: CreateDictTests
    // Verifies that create_dict() loads, cleans, sorts, and deduplicates words.
    // ---------------------------------------------------------------------------
    TEST_CLASS(CreateDictTests)
    {
    public:

        TEST_METHOD(ValidFile_LoadsSortedAndDeduped)
        {
            const std::string path = "sc_dict_test_1.txt";
            WriteFile(path, "cat\ndog\nbird\ncat"); // 'cat' duplicated

            std::vector<std::string> dict;
            create_dict(dict, path);
            std::remove(path.c_str());

            Assert::AreEqual(size_t(3), dict.size());
            Assert::AreEqual(std::string("bird"), dict[0]);
            Assert::AreEqual(std::string("cat"),  dict[1]);
            Assert::AreEqual(std::string("dog"),  dict[2]);
        }

        TEST_METHOD(ValidFile_CleanedToLowercase)
        {
            const std::string path = "sc_dict_test_2.txt";
            WriteFile(path, "Hello\nWORLD");

            std::vector<std::string> dict;
            create_dict(dict, path);
            std::remove(path.c_str());

            Assert::AreEqual(size_t(2), dict.size());
            // sorted: "hello" < "world"
            Assert::AreEqual(std::string("hello"), dict[0]);
            Assert::AreEqual(std::string("world"), dict[1]);
        }

        // Punctuation on words is stripped by clean_word before insertion.
        TEST_METHOD(FileWithPunctuation_WordsCleanedBeforeStoring)
        {
            const std::string path = "sc_dict_test_3.txt";
            WriteFile(path, "Hello!\nworld.");

            std::vector<std::string> dict;
            create_dict(dict, path);
            std::remove(path.c_str());

            Assert::IsTrue(std::binary_search(dict.begin(), dict.end(), std::string("hello")));
            Assert::IsTrue(std::binary_search(dict.begin(), dict.end(), std::string("world")));
        }

        // Words that clean to empty strings (e.g. pure numbers) must be skipped.
        TEST_METHOD(FileWithAllNumbers_EmptyDict)
        {
            const std::string path = "sc_dict_test_4.txt";
            WriteFile(path, "123\n456");

            std::vector<std::string> dict;
            create_dict(dict, path);
            std::remove(path.c_str());

            Assert::IsTrue(dict.empty());
        }

        TEST_METHOD(NonexistentFile_Throws)
        {
            std::vector<std::string> dict;
            Assert::ExpectException<std::runtime_error>([&]() {
                create_dict(dict, "sc_no_such_file_xyz.txt");
                });
        }

        // Case-insensitive duplicates are collapsed: "Hello" and "hello" both clean
        // to "hello", so only one entry should appear after deduplication.
        TEST_METHOD(CaseInsensitiveDuplicates_Deduped)
        {
            const std::string path = "sc_dict_test_5.txt";
            WriteFile(path, "Hello\nhello\nHELLO");

            std::vector<std::string> dict;
            create_dict(dict, path);
            std::remove(path.c_str());

            Assert::AreEqual(size_t(1), dict.size());
            Assert::AreEqual(std::string("hello"), dict[0]);
        }
    };


    // ---------------------------------------------------------------------------
    // TEST CLASS: LoadSourceTests
    // Verifies that load_source() loads words in order, cleans them,
    // but does NOT sort or deduplicate.
    // ---------------------------------------------------------------------------
    TEST_CLASS(LoadSourceTests)
    {
    public:

        // Order must be preserved; duplicate words must NOT be removed.
        TEST_METHOD(ValidFile_LoadsInOrder_NoDeduplicate)
        {
            const std::string path = "sc_source_test_1.txt";
            WriteFile(path, "cat dog cat");

            std::vector<std::string> source;
            load_source(source, path);
            std::remove(path.c_str());

            Assert::AreEqual(size_t(3), source.size());
            Assert::AreEqual(std::string("cat"), source[0]);
            Assert::AreEqual(std::string("dog"), source[1]);
            Assert::AreEqual(std::string("cat"), source[2]); // duplicate kept
        }

        TEST_METHOD(ValidFile_CleanedToLowercase)
        {
            const std::string path = "sc_source_test_2.txt";
            WriteFile(path, "Hello World");

            std::vector<std::string> source;
            load_source(source, path);
            std::remove(path.c_str());

            Assert::AreEqual(std::string("hello"), source[0]);
            Assert::AreEqual(std::string("world"), source[1]);
        }

        TEST_METHOD(ValidFile_PunctuationStripped)
        {
            const std::string path = "sc_source_test_3.txt";
            WriteFile(path, "cat. dog, bird!");

            std::vector<std::string> source;
            load_source(source, path);
            std::remove(path.c_str());

            Assert::AreEqual(size_t(3), source.size());
            Assert::AreEqual(std::string("cat"),  source[0]);
            Assert::AreEqual(std::string("dog"),  source[1]);
            Assert::AreEqual(std::string("bird"), source[2]);
        }

        // Apostrophes must be preserved in source words.
        TEST_METHOD(ValidFile_ApostrophePreserved)
        {
            const std::string path = "sc_source_test_4.txt";
            WriteFile(path, "don't it's");

            std::vector<std::string> source;
            load_source(source, path);
            std::remove(path.c_str());

            Assert::AreEqual(std::string("don't"), source[0]);
            Assert::AreEqual(std::string("it's"),  source[1]);
        }

        TEST_METHOD(NonexistentFile_Throws)
        {
            std::vector<std::string> source;
            Assert::ExpectException<std::runtime_error>([&]() {
                load_source(source, "sc_no_source_file_xyz.txt");
                });
        }

        // Words that clean to an empty string (e.g. "123", "!!!") must be skipped.
        TEST_METHOD(EmptyWordsAfterCleaning_Skipped)
        {
            const std::string path = "sc_source_test_6.txt";
            WriteFile(path, "123 !!! cat");

            std::vector<std::string> source;
            load_source(source, path);
            std::remove(path.c_str());

            Assert::AreEqual(size_t(1), source.size());
            Assert::AreEqual(std::string("cat"), source[0]);
        }
    };


    // ---------------------------------------------------------------------------
    // TEST CLASS: FindMisspelledTests
    // Verifies that find_misspelled() correctly identifies words not in the
    // dictionary and records their 1-indexed positions.
    //
    // IMPORTANT: dict must always be sorted (binary_search precondition).
    // ---------------------------------------------------------------------------
    TEST_CLASS(FindMisspelledTests)
    {
    public:

        // All source words are in the dictionary -> no misspelled words.
        TEST_METHOD(AllWordsInDict_EmptyResult)
        {
            std::vector<std::string> source = { "cat", "dog", "bird" };
            std::vector<std::string> dict   = { "bird", "cat", "dog" }; // sorted
            std::vector<misspelled_word> misspelled;

            find_misspelled(source, dict, misspelled);

            Assert::IsTrue(misspelled.empty());
        }

        // One word absent from dict -> recorded with the correct 1-indexed position.
        TEST_METHOD(OneWordNotInDict_CorrectPosition)
        {
            std::vector<std::string> source = { "cat", "xyz", "dog" };
            std::vector<std::string> dict   = { "cat", "dog" }; // sorted

            std::vector<misspelled_word> misspelled;
            find_misspelled(source, dict, misspelled);

            Assert::AreEqual(size_t(1), misspelled.size());
            Assert::AreEqual(std::string("xyz"), misspelled[0].word);
            Assert::AreEqual(2, misspelled[0].word_number); // position 2 (1-indexed)
        }

        // Multiple absent words -> all recorded with correct positions.
        TEST_METHOD(MultipleWordsMisspelled_AllRecorded)
        {
            std::vector<std::string> source = { "abc", "cat", "xyz" };
            std::vector<std::string> dict   = { "cat" }; // sorted

            std::vector<misspelled_word> misspelled;
            find_misspelled(source, dict, misspelled);

            Assert::AreEqual(size_t(2), misspelled.size());
            Assert::AreEqual(std::string("abc"), misspelled[0].word);
            Assert::AreEqual(1, misspelled[0].word_number);
            Assert::AreEqual(std::string("xyz"), misspelled[1].word);
            Assert::AreEqual(3, misspelled[1].word_number);
        }

        TEST_METHOD(EmptySource_EmptyResult)
        {
            std::vector<std::string> source;
            std::vector<std::string> dict = { "cat", "dog" };
            std::vector<misspelled_word> misspelled;

            find_misspelled(source, dict, misspelled);

            Assert::IsTrue(misspelled.empty());
        }

        // An empty dictionary means every word is misspelled.
        TEST_METHOD(EmptyDict_AllWordsMisspelled)
        {
            std::vector<std::string> source = { "cat", "dog" };
            std::vector<std::string> dict;
            std::vector<misspelled_word> misspelled;

            find_misspelled(source, dict, misspelled);

            Assert::AreEqual(size_t(2), misspelled.size());
            Assert::AreEqual(1, misspelled[0].word_number);
            Assert::AreEqual(2, misspelled[1].word_number);
        }

        // A word appearing twice in source is checked at both positions independently.
        TEST_METHOD(DuplicateSourceWords_EachPositionChecked)
        {
            std::vector<std::string> source = { "cat", "xyz", "cat" }; // cat at 1 & 3
            std::vector<std::string> dict   = { "cat" }; // sorted

            std::vector<misspelled_word> misspelled;
            find_misspelled(source, dict, misspelled);

            // Only "xyz" at position 2 should be misspelled.
            Assert::AreEqual(size_t(1), misspelled.size());
            Assert::AreEqual(std::string("xyz"), misspelled[0].word);
            Assert::AreEqual(2, misspelled[0].word_number);
        }

        // The very first word is at position 1, not 0.
        TEST_METHOD(Position_IsOneIndexed)
        {
            std::vector<std::string> source = { "xyz" };
            std::vector<std::string> dict;
            std::vector<misspelled_word> misspelled;

            find_misspelled(source, dict, misspelled);

            Assert::AreEqual(1, misspelled[0].word_number);
        }

        // Verifies the struct fields .word and .word_number are correctly populated.
        TEST_METHOD(MisspelledWord_StructFieldsCorrect)
        {
            std::vector<std::string> source = { "foo" };
            std::vector<std::string> dict;
            std::vector<misspelled_word> misspelled;

            find_misspelled(source, dict, misspelled);

            Assert::AreEqual(std::string("foo"), misspelled[0].word);
            Assert::AreEqual(1, misspelled[0].word_number);
        }
    };


    // ---------------------------------------------------------------------------
    // TEST CLASS: OutputMisspelledTests
    // Verifies that output_misspelled() writes the correct text to stdout.
    // Uses cout-redirection to capture output.
    // ---------------------------------------------------------------------------
    TEST_CLASS(OutputMisspelledTests)
    {
    public:

        // An empty misspelled list must print the header and a "none found" message.
        TEST_METHOD(EmptyMisspelled_PrintsHeaderAndNoneFound)
        {
            std::vector<misspelled_word> misspelled;

            std::streambuf* oldBuf = std::cout.rdbuf();
            std::ostringstream captured;
            std::cout.rdbuf(captured.rdbuf());

            output_misspelled(misspelled);

            std::cout.rdbuf(oldBuf);

            Assert::AreEqual(std::string("Misspelled words:\nNo misspelled words found.\n"), captured.str());
        }

        // One misspelled word must be printed in the expected format.
        TEST_METHOD(OneMisspelledWord_CorrectFormat)
        {
            std::vector<misspelled_word> misspelled;
            misspelled.emplace_back("xyz", 3);

            std::streambuf* oldBuf = std::cout.rdbuf();
            std::ostringstream captured;
            std::cout.rdbuf(captured.rdbuf());

            output_misspelled(misspelled);

            std::cout.rdbuf(oldBuf);

            const std::string expected = "Misspelled words:\nWord: xyz, Position: 3\n";
            Assert::AreEqual(expected, captured.str());
        }

        // Multiple misspelled words must all appear in insertion order.
        TEST_METHOD(MultipleMisspelledWords_AllListedInOrder)
        {
            std::vector<misspelled_word> misspelled;
            misspelled.emplace_back("abc", 1);
            misspelled.emplace_back("xyz", 5);

            std::streambuf* oldBuf = std::cout.rdbuf();
            std::ostringstream captured;
            std::cout.rdbuf(captured.rdbuf());

            output_misspelled(misspelled);

            std::cout.rdbuf(oldBuf);

            const std::string expected =
                "Misspelled words:\n"
                "Word: abc, Position: 1\n"
                "Word: xyz, Position: 5\n";
            Assert::AreEqual(expected, captured.str());
        }

        // The format string uses "Word: X, Position: N" with a comma-space separator.
        TEST_METHOD(OutputFormat_CommaSpaceSeparator)
        {
            std::vector<misspelled_word> misspelled;
            misspelled.emplace_back("cat", 2);

            std::streambuf* oldBuf = std::cout.rdbuf();
            std::ostringstream captured;
            std::cout.rdbuf(captured.rdbuf());

            output_misspelled(misspelled);

            std::cout.rdbuf(oldBuf);

            const std::string output = captured.str();
            Assert::IsTrue(output.find("Word: cat, Position: 2") != std::string::npos);
        }
    };
}
