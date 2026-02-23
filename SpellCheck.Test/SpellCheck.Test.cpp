#include "pch.h"
#include "CppUnitTest.h"
#include "../SpellCheck.Core/SpellCheck.h"

#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SpellCheckTest
{
	TEST_CLASS(SpellCheckTest_Core)
	{
	public:
		std::string get_test_dict_path()
		{
			std::ifstream local("test_dict.txt");
			if (local.good())
			{
				return "test_dict.txt";
			}

			std::ifstream nested("SpellCheck.Test/test_dict.txt");
			if (nested.good())
			{
				return "SpellCheck.Test/test_dict.txt";
			}

			Assert::Fail(L"Unable to locate test_dict.txt from current working directory.");
			return "";
		}

		std::string create_temp_file(const std::string& file_name, const std::string& content)
		{
			std::ofstream temp(file_name);
			temp << content;
			temp.close();
			return file_name;
		}

		TEST_METHOD(CreateDict_LoadsAllWords)
		{
			std::vector<std::string> dict;

			create_dict(dict, get_test_dict_path());

			Assert::AreEqual(static_cast<size_t>(8), dict.size());
		}

		TEST_METHOD(CreateDict_NormalizesSortsAndDeduplicates)
		{
			std::vector<std::string> dict;
			const std::string dict_file = create_temp_file(
				"create_dict_temp.txt",
				"Apple\nbanana\nBANANA\napple\ncherry\n");

			create_dict(dict, dict_file);

			std::vector<std::string> expected = { "apple", "banana", "cherry" };
			Assert::IsTrue(dict == expected);

			std::remove(dict_file.c_str());
		}

		TEST_METHOD(CreateDict_ThrowsWhenDictionaryMissing)
		{
			std::vector<std::string> dict;

			Assert::ExpectException<std::runtime_error>([&dict]()
				{
					create_dict(dict, "definitely_missing_dictionary_file.txt");
				});
		}

		TEST_METHOD(LoadSource_CleansWordsFromInput)
		{
			std::vector<std::string> source;
			const std::string source_file = create_temp_file(
				"load_source_temp.txt",
				"Hello, WORLD! can't... #123 done?\n");

			load_source(source, source_file);

			std::vector<std::string> expected = { "hello", "world", "can't", "", "done" };
			Assert::IsTrue(source == expected);

			std::remove(source_file.c_str());
		}

		TEST_METHOD(LoadSource_ThrowsWhenInputMissing)
		{
			std::vector<std::string> source;

			Assert::ExpectException<std::runtime_error>([&source]()
				{
					load_source(source, "definitely_missing_input_file.txt");
				});
		}

		TEST_METHOD(CleanWord_RemovesNonLettersAndLowercases)
		{
			const std::string cleaned = clean_word("Te-st's! 123");
			Assert::AreEqual(std::string("test's"), cleaned);
		}

		TEST_METHOD(FindMisspelled_FindsWordsAndPositions)
		{
			std::vector<std::string> source = { "the", "qwick", "brown", "", "focks", "fox" };
			std::vector<std::string> dict = { "brown", "fox", "the" };
			std::vector<misspelled_word> misspelled;

			find_misspelled(source, dict, misspelled);

			Assert::AreEqual(static_cast<size_t>(2), misspelled.size());
			Assert::AreEqual(std::string("qwick"), misspelled[0].word);
			Assert::AreEqual(2, misspelled[0].word_number);
			Assert::AreEqual(std::string("focks"), misspelled[1].word);
			Assert::AreEqual(5, misspelled[1].word_number);
		}

		TEST_METHOD(OutputMisspelled_WritesFileAndPrintsCounts)
		{
			std::vector<misspelled_word> misspelled = {
				misspelled_word("zeta", 2),
				misspelled_word("alpha", 1),
				misspelled_word("alpha", 3)
			};

			std::stringstream capture;
			std::streambuf* original = std::cout.rdbuf(capture.rdbuf());

			output_misspelled(misspelled);

			std::cout.rdbuf(original);

			std::ifstream output_file("output.txt");
			std::stringstream file_contents;
			file_contents << output_file.rdbuf();
			const std::string file_text = file_contents.str();

			Assert::IsTrue(file_text.find("Misspelled word: zeta at position 2") != std::string::npos);
			Assert::IsTrue(file_text.find("Misspelled word: alpha at position 1") != std::string::npos);
			Assert::IsTrue(file_text.find("Misspelled word: alpha at position 3") != std::string::npos);

			const std::string console_text = capture.str();
			Assert::IsTrue(console_text.find("Total misspelled words: 3") != std::string::npos);
			Assert::IsTrue(console_text.find("alpha: 2") != std::string::npos);
			Assert::IsTrue(console_text.find("zeta: 1") != std::string::npos);
		}
	};
}
