/* ---------------------------------------------------------------------------
** I attest that this following code represents my own work and is subject to
** the plagiarism policy found in the course syllabus.
**
**       Class: CSC 242
**  Assignment: Lesson 5 / Part 1 / Cipher
**        File: Cipher.Test.cpp
** Description: Unit tests for Cipher encryption/decryption functions.
**      Author: Afshin Bahrampour, Andre Gonzalez, Daniel Johnston
**        Date: 14 Feb 26
** -------------------------------------------------------------------------*/

#include "pch.h"
#include "CppUnitTest.h"
#include <cstdio>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

// Include the cipher source directly so static functions are visible.
// Rename main to avoid a duplicate symbol.
#define main cipher_main_impl
#include "../Cipher/Source.cpp"
#undef main

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// ---------------------------------------------------------------------------
// Helper: build a char* argv[] from a vector<string>.
// The strings vector must remain alive for the lifetime of the returned array.
// ---------------------------------------------------------------------------
static std::vector<const char*> MakeArgv(const std::vector<std::string>& args)
{
    std::vector<const char*> v;
    v.reserve(args.size());
    for (const auto& s : args)
        v.push_back(s.c_str());
    return v;
}

// ---------------------------------------------------------------------------
// Helper: write a small file and return its path.
// ---------------------------------------------------------------------------
static void WriteFile(const std::string& path, const std::string& content)
{
    std::ofstream f(path, std::ios::binary);
    f << content;
}

// ---------------------------------------------------------------------------
// TEST CLASS: GenerateKeyTests
// Verifies that generate_key() builds the correct 52-row substitution table.
//
// For keyword "feather":
//   Unique letters in order: f e a t h r
//   Remaining letters z->a (skipping f,e,a,t,h,r): z y x w v u s q p o n m l k j i g d c b
//   Full cipher alphabet: f e a t h r z y x w v u s q p o n m l k j i g d c b
//   Mappings: a->f  b->e  c->a  d->t  e->h  f->r  g->z  h->y  i->x  j->w
//             k->v  l->u  m->s  n->q  o->p  p->o  q->n  r->m  s->l  t->k
//             u->j  v->i  w->g  x->d  y->c  z->b
// ---------------------------------------------------------------------------
namespace CipherTest
{
    TEST_CLASS(GenerateKeyTests)
    {
    public:

        // Verify the first six cipher letters and the last for "feather".
        TEST_METHOD(FeatherKeyword_LowercaseRowsCorrect)
        {
            char key[KEY_ROWS][KEY_COLS] = {};
            generate_key("feather", key);

            // Plain-text column should be sequential a-z
            Assert::AreEqual('a', key[0][0]);
            Assert::AreEqual('b', key[1][0]);
            Assert::AreEqual('z', key[25][0]);

            // Cipher column: keyword letters first, then z->a for unused
            Assert::AreEqual('f', key[0][1]);  // a -> f
            Assert::AreEqual('e', key[1][1]);  // b -> e
            Assert::AreEqual('a', key[2][1]);  // c -> a
            Assert::AreEqual('t', key[3][1]);  // d -> t
            Assert::AreEqual('h', key[4][1]);  // e -> h
            Assert::AreEqual('r', key[5][1]);  // f -> r
            Assert::AreEqual('b', key[25][1]); // z -> b
        }

        // Rows 26-51 are uppercase mirrors of rows 0-25.
        TEST_METHOD(FeatherKeyword_UppercaseRowsCorrect)
        {
            char key[KEY_ROWS][KEY_COLS] = {};
            generate_key("feather", key);

            Assert::AreEqual('A', key[26][0]);
            Assert::AreEqual('F', key[26][1]);  // A -> F
            Assert::AreEqual('B', key[27][0]);
            Assert::AreEqual('E', key[27][1]);  // B -> E
            Assert::AreEqual('Z', key[51][0]);
            Assert::AreEqual('B', key[51][1]);  // Z -> B
        }

        // "FEATHER" (uppercase keyword) must produce the same table as "feather".
        TEST_METHOD(UppercaseKeyword_SameResultAsLowercase)
        {
            char keyLower[KEY_ROWS][KEY_COLS] = {};
            char keyUpper[KEY_ROWS][KEY_COLS] = {};
            generate_key("feather", keyLower);
            generate_key("FEATHER", keyUpper);

            for (int i = 0; i < KEY_ROWS; ++i)
            {
                Assert::AreEqual(keyLower[i][0], keyUpper[i][0]);
                Assert::AreEqual(keyLower[i][1], keyUpper[i][1]);
            }
        }

        // Duplicate letters in the keyword must be deduplicated.
        TEST_METHOD(KeywordWithDuplicates_DeduplicatesLetters)
        {
            char keyNormal[KEY_ROWS][KEY_COLS] = {};
            char keyDuped[KEY_ROWS][KEY_COLS] = {};
            generate_key("feather", keyNormal);
            generate_key("ffeather", keyDuped); // doubled 'f'

            for (int i = 0; i < KEY_ROWS; ++i)
            {
                Assert::AreEqual(keyNormal[i][1], keyDuped[i][1]);
            }
        }

        // Non-alpha characters in the keyword must be ignored.
        TEST_METHOD(KeywordWithNonAlpha_IgnoresNonAlpha)
        {
            char keyNormal[KEY_ROWS][KEY_COLS] = {};
            char keyExtra[KEY_ROWS][KEY_COLS] = {};
            generate_key("feather", keyNormal);
            generate_key("feather123!", keyExtra);

            for (int i = 0; i < KEY_ROWS; ++i)
            {
                Assert::AreEqual(keyNormal[i][1], keyExtra[i][1]);
            }
        }

        // An empty keyword fills the cipher alphabet z->a (reverse).
        TEST_METHOD(EmptyKeyword_AllZtoA)
        {
            char key[KEY_ROWS][KEY_COLS] = {};
            generate_key("", key);

            Assert::AreEqual('z', key[0][1]);  // a -> z
            Assert::AreEqual('a', key[25][1]); // z -> a
            Assert::AreEqual('Z', key[26][1]); // A -> Z
            Assert::AreEqual('A', key[51][1]); // Z -> A
        }

        // A keyword containing all 26 letters (in order a-z) is an identity mapping.
        TEST_METHOD(FullAlphabetKeyword_IdentityMapping)
        {
            char key[KEY_ROWS][KEY_COLS] = {};
            generate_key("abcdefghijklmnopqrstuvwxyz", key);

            for (int i = 0; i < 26; ++i)
            {
                Assert::AreEqual(char('a' + i), key[i][1]);       // lowercase identity
                Assert::AreEqual(char('A' + i), key[26 + i][1]);  // uppercase identity
            }
        }

        // The plain-text column (index 0) must always be the sequential alphabet.
        TEST_METHOD(AllRowsHaveCorrectPlainChar)
        {
            char key[KEY_ROWS][KEY_COLS] = {};
            generate_key("feather", key);

            for (int i = 0; i < 26; ++i)
            {
                Assert::AreEqual(char('a' + i), key[i][0]);
                Assert::AreEqual(char('A' + i), key[26 + i][0]);
            }
        }

        // Mixed-case keyword should produce the same table as lowercase.
        TEST_METHOD(MixedCaseKeyword_SameAsLowercase)
        {
            char keyLower[KEY_ROWS][KEY_COLS] = {};
            char keyMixed[KEY_ROWS][KEY_COLS] = {};
            generate_key("feather", keyLower);
            generate_key("FeAtHeR", keyMixed);

            for (int i = 0; i < KEY_ROWS; ++i)
            {
                Assert::AreEqual(keyLower[i][1], keyMixed[i][1]);
            }
        }
    };


    // ---------------------------------------------------------------------------
    // TEST CLASS: ShiftTextTests
    // Verifies encryption and decryption using the "feather" keyword.
    // ---------------------------------------------------------------------------
    TEST_CLASS(ShiftTextTests)
    {
    private:
        // Build the feather key once via a helper called inside each test.
        static void FeatherKey(char key[KEY_ROWS][KEY_COLS])
        {
            generate_key("feather", key);
        }

    public:

        TEST_METHOD(Encrypt_Hello_ReturnsYhuup)
        {
            char key[KEY_ROWS][KEY_COLS] = {};
            FeatherKey(key);
            Assert::AreEqual(std::string("yhuup"), shift_text("hello", key, 'e'));
        }

        TEST_METHOD(Decrypt_Yhuup_ReturnsHello)
        {
            char key[KEY_ROWS][KEY_COLS] = {};
            FeatherKey(key);
            Assert::AreEqual(std::string("hello"), shift_text("yhuup", key, 'd'));
        }

        TEST_METHOD(Encrypt_Abc_ReturnsFea)
        {
            char key[KEY_ROWS][KEY_COLS] = {};
            FeatherKey(key);
            Assert::AreEqual(std::string("fea"), shift_text("abc", key, 'e'));
        }

        TEST_METHOD(Decrypt_Fea_ReturnsAbc)
        {
            char key[KEY_ROWS][KEY_COLS] = {};
            FeatherKey(key);
            Assert::AreEqual(std::string("abc"), shift_text("fea", key, 'd'));
        }

        // Non-alpha characters (comma, space, exclamation) must pass through unchanged.
        // Cipher map for "feather": d->t, W->G, o->p, r->m, l->u, d->t => "World"->"Gpmut"
        TEST_METHOD(Encrypt_HelloWorldWithPunct_PreservesNonAlpha)
        {
            char key[KEY_ROWS][KEY_COLS] = {};
            FeatherKey(key);
            Assert::AreEqual(std::string("Yhuup, Gpmut!"),
                shift_text("Hello, World!", key, 'e'));
        }

        TEST_METHOD(Decrypt_HelloWorldCiphertext_ReturnsOriginal)
        {
            char key[KEY_ROWS][KEY_COLS] = {};
            FeatherKey(key);
            Assert::AreEqual(std::string("Hello, World!"),
                shift_text("Yhuup, Gpmut!", key, 'd'));
        }

        // Uppercase action 'E' must behave identically to lowercase 'e'.
        TEST_METHOD(Encrypt_UppercaseAction_SameAsLowercase)
        {
            char key[KEY_ROWS][KEY_COLS] = {};
            FeatherKey(key);
            Assert::AreEqual(std::string("yhuup"), shift_text("hello", key, 'E'));
        }

        // Uppercase action 'D' must behave identically to lowercase 'd'.
        TEST_METHOD(Decrypt_UppercaseAction_SameAsLowercase)
        {
            char key[KEY_ROWS][KEY_COLS] = {};
            FeatherKey(key);
            Assert::AreEqual(std::string("hello"), shift_text("yhuup", key, 'D'));
        }

        TEST_METHOD(Encrypt_EmptyString_ReturnsEmpty)
        {
            char key[KEY_ROWS][KEY_COLS] = {};
            FeatherKey(key);
            Assert::AreEqual(std::string(""), shift_text("", key, 'e'));
        }

        // A string with only non-alpha characters must be returned unchanged.
        TEST_METHOD(Encrypt_NonAlphaOnly_PassesThrough)
        {
            char key[KEY_ROWS][KEY_COLS] = {};
            FeatherKey(key);
            const std::string input = "123!@# \t\n";
            Assert::AreEqual(input, shift_text(input, key, 'e'));
        }

        // Lowercase and uppercase of the same letter must map to the same cipher letter
        // in matching case (rows 0-25 handle lowercase, rows 26-51 handle uppercase).
        TEST_METHOD(Encrypt_MixedCase_PreservesCase)
        {
            char key[KEY_ROWS][KEY_COLS] = {};
            FeatherKey(key);
            // a -> f (row 0),  A -> F (row 26)
            Assert::AreEqual(std::string("fF"), shift_text("aA", key, 'e'));
        }

        // Encrypting then decrypting must recover the original string exactly.
        TEST_METHOD(Roundtrip_EncryptThenDecrypt_ReturnsOriginal)
        {
            char key[KEY_ROWS][KEY_COLS] = {};
            FeatherKey(key);
            const std::string original =
                "The quick brown fox jumps over the lazy dog.";
            const std::string encrypted = shift_text(original, key, 'e');
            const std::string decrypted = shift_text(encrypted, key, 'd');
            Assert::AreEqual(original, decrypted);
        }

        // An unknown action character leaves every character unreplaced (identity).
        TEST_METHOD(UnknownAction_ReturnsInputUnchanged)
        {
            char key[KEY_ROWS][KEY_COLS] = {};
            FeatherKey(key);
            Assert::AreEqual(std::string("hello"), shift_text("hello", key, 'x'));
        }
    };


    // ---------------------------------------------------------------------------
    // TEST CLASS: ParseArgsTests
    // Verifies argument parsing, flag detection, and error handling.
    // ---------------------------------------------------------------------------
    TEST_CLASS(ParseArgsTests)
    {
    private:
        // Convenience wrapper that builds argv and calls parse_args.
        static void CallParse(const std::vector<std::string>& argStrings,
            char& action,
            std::string& keyword,
            std::string& input_file,
            std::string& output_file)
        {
            auto argv = MakeArgv(argStrings);
            parse_args(static_cast<int>(argv.size()),
                const_cast<char**>(argv.data()),
                action, keyword, input_file, output_file);
        }

    public:

        // Fewer than 5 arguments must throw.
        TEST_METHOD(TooFewArgs_Throws)
        {
            char action = '\0';
            std::string kw, in, out;
            std::vector<std::string> args = { "prog", "-e", "-kfeather", "in.txt" }; // argc=4
            auto argv = MakeArgv(args);
            Assert::ExpectException<std::runtime_error>([&]() {
                parse_args(static_cast<int>(argv.size()),
                    const_cast<char**>(argv.data()),
                    action, kw, in, out);
                });
        }

        // Valid encrypt with inline keyword (-kKEYWORD).
        TEST_METHOD(ValidEncryptInlineKeyword_ParsesCorrectly)
        {
            char action = '\0';
            std::string kw, in, out;
            CallParse({ "prog", "-e", "-kfeather", "in.txt", "out.txt" },
                action, kw, in, out);
            Assert::AreEqual('e', action);
            Assert::AreEqual(std::string("feather"), kw);
            Assert::AreEqual(std::string("in.txt"), in);
            Assert::AreEqual(std::string("out.txt"), out);
        }

        // Valid decrypt with inline keyword.
        TEST_METHOD(ValidDecryptInlineKeyword_ParsesCorrectly)
        {
            char action = '\0';
            std::string kw, in, out;
            CallParse({ "prog", "-d", "-kfeather", "in.txt", "out.txt" },
                action, kw, in, out);
            Assert::AreEqual('d', action);
            Assert::AreEqual(std::string("feather"), kw);
        }

        // Valid encrypt with keyword as a separate argument (-k KEYWORD).
        TEST_METHOD(ValidEncryptSeparateKeyword_ParsesCorrectly)
        {
            char action = '\0';
            std::string kw, in, out;
            CallParse({ "prog", "-e", "-k", "feather", "in.txt", "out.txt" },
                action, kw, in, out);
            Assert::AreEqual('e', action);
            Assert::AreEqual(std::string("feather"), kw);
            Assert::AreEqual(std::string("in.txt"), in);
            Assert::AreEqual(std::string("out.txt"), out);
        }

        // Valid decrypt with separate keyword.
        TEST_METHOD(ValidDecryptSeparateKeyword_ParsesCorrectly)
        {
            char action = '\0';
            std::string kw, in, out;
            CallParse({ "prog", "-d", "-k", "feather", "in.txt", "out.txt" },
                action, kw, in, out);
            Assert::AreEqual('d', action);
            Assert::AreEqual(std::string("feather"), kw);
        }

        // Missing -e / -d flag must throw.
        TEST_METHOD(MissingAction_Throws)
        {
            char action = '\0';
            std::string kw, in, out;
            std::vector<std::string> args = { "prog", "-kfeather", "in.txt", "out.txt", "extra" };
            auto argv = MakeArgv(args);
            Assert::ExpectException<std::runtime_error>([&]() {
                parse_args(static_cast<int>(argv.size()),
                    const_cast<char**>(argv.data()),
                    action, kw, in, out);
                });
        }

        // Missing -k keyword must throw.
        TEST_METHOD(MissingKeyword_Throws)
        {
            char action = '\0';
            std::string kw, in, out;
            std::vector<std::string> args = { "prog", "-e", "in.txt", "out.txt", "extra" };
            auto argv = MakeArgv(args);
            Assert::ExpectException<std::runtime_error>([&]() {
                parse_args(static_cast<int>(argv.size()),
                    const_cast<char**>(argv.data()),
                    action, kw, in, out);
                });
        }

        // Missing output file must throw (only one positional arg provided).
        TEST_METHOD(MissingOutputFile_Throws)
        {
            char action = '\0';
            std::string kw, in, out;
            // argc=5 but only one positional file arg after flags
            std::vector<std::string> args = { "prog", "-e", "-k", "feather", "in.txt" };
            auto argv = MakeArgv(args);
            Assert::ExpectException<std::runtime_error>([&]() {
                parse_args(static_cast<int>(argv.size()),
                    const_cast<char**>(argv.data()),
                    action, kw, in, out);
                });
        }

        // Three or more positional file arguments must throw.
        TEST_METHOD(TooManyPositionalArgs_Throws)
        {
            char action = '\0';
            std::string kw, in, out;
            std::vector<std::string> args =
            { "prog", "-e", "-kfeather", "in.txt", "out.txt", "extra.txt" };
            auto argv = MakeArgv(args);
            Assert::ExpectException<std::runtime_error>([&]() {
                parse_args(static_cast<int>(argv.size()),
                    const_cast<char**>(argv.data()),
                    action, kw, in, out);
                });
        }

        // "-k" as the very last argument (no following keyword value) must throw.
        TEST_METHOD(DashKAtEnd_Throws)
        {
            char action = '\0';
            std::string kw, in, out;
            std::vector<std::string> args = { "prog", "-e", "in.txt", "out.txt", "-k" };
            auto argv = MakeArgv(args);
            Assert::ExpectException<std::runtime_error>([&]() {
                parse_args(static_cast<int>(argv.size()),
                    const_cast<char**>(argv.data()),
                    action, kw, in, out);
                });
        }

        // parse_args must preserve the keyword exactly as given (generate_key handles case).
        TEST_METHOD(KeywordCasePreserved)
        {
            char action = '\0';
            std::string kw, in, out;
            CallParse({ "prog", "-e", "-kFEATHER", "in.txt", "out.txt" },
                action, kw, in, out);
            Assert::AreEqual(std::string("FEATHER"), kw);
        }

        // "-E" and "-D" are not valid action flags (exact string match only).
        TEST_METHOD(UppercaseActionFlag_Throws)
        {
            char action = '\0';
            std::string kw, in, out;
            std::vector<std::string> args = { "prog", "-E", "-kfeather", "in.txt", "out.txt" };
            auto argv = MakeArgv(args);
            Assert::ExpectException<std::runtime_error>([&]() {
                parse_args(static_cast<int>(argv.size()),
                    const_cast<char**>(argv.data()),
                    action, kw, in, out);
                });
        }

        // Flags may appear in any order; positional args are still detected correctly.
        TEST_METHOD(FlagsOrderIndependent)
        {
            char action = '\0';
            std::string kw, in, out;
            // keyword flag before action flag
            CallParse({ "prog", "-kfeather", "-e", "in.txt", "out.txt" },
                action, kw, in, out);
            Assert::AreEqual('e', action);
            Assert::AreEqual(std::string("feather"), kw);
            Assert::AreEqual(std::string("in.txt"), in);
            Assert::AreEqual(std::string("out.txt"), out);
        }

        // File arguments appearing before flag arguments are captured as positional args.
        TEST_METHOD(FilesBeforeFlags_ParsedCorrectly)
        {
            char action = '\0';
            std::string kw, in, out;
            CallParse({ "prog", "in.txt", "out.txt", "-e", "-kfeather" },
                action, kw, in, out);
            Assert::AreEqual('e', action);
            Assert::AreEqual(std::string("feather"), kw);
            Assert::AreEqual(std::string("in.txt"), in);
            Assert::AreEqual(std::string("out.txt"), out);
        }
    };


    // ---------------------------------------------------------------------------
    // TEST CLASS: SaveOutputTests
    // Verifies that save_output() writes file content correctly.
    // ---------------------------------------------------------------------------
    TEST_CLASS(SaveOutputTests)
    {
    public:

        TEST_METHOD(WritesContentToFile)
        {
            const std::string path = "cipher_save_test_1.txt";
            save_output(path, "hello world");

            std::ifstream f(path);
            std::stringstream ss;
            ss << f.rdbuf();
            std::remove(path.c_str());

            Assert::AreEqual(std::string("hello world"), ss.str());
        }

        TEST_METHOD(WritesEmptyString)
        {
            const std::string path = "cipher_save_test_2.txt";
            save_output(path, "");

            std::ifstream f(path);
            std::stringstream ss;
            ss << f.rdbuf();
            std::remove(path.c_str());

            Assert::AreEqual(std::string(""), ss.str());
        }

        TEST_METHOD(OverwritesExistingFile)
        {
            const std::string path = "cipher_save_test_3.txt";
            WriteFile(path, "old content");

            save_output(path, "new content");

            std::ifstream f(path);
            std::stringstream ss;
            ss << f.rdbuf();
            std::remove(path.c_str());

            Assert::AreEqual(std::string("new content"), ss.str());
        }

        TEST_METHOD(WritesMultilineContent)
        {
            const std::string path = "cipher_save_test_4.txt";
            const std::string content = "line1\nline2\nline3";
            save_output(path, content);

            std::ifstream f(path); // text mode: translates \r\n -> \n on Windows
            std::stringstream ss;
            ss << f.rdbuf();
            std::remove(path.c_str());

            Assert::AreEqual(content, ss.str());
        }

        TEST_METHOD(InvalidPath_Throws)
        {
            Assert::ExpectException<std::runtime_error>([]() {
                save_output("/no_such_dir_xyz_abc/out.txt", "data");
                });
        }
    };


    // ---------------------------------------------------------------------------
    // TEST CLASS: GetInputTests
    // Verifies that get_input() reads .txt files correctly.
    // Note: the non-.txt interactive path requires stdin and is not unit-testable.
    // ---------------------------------------------------------------------------
    TEST_CLASS(GetInputTests)
    {
    public:

        TEST_METHOD(ValidTxtFile_ReturnsContents)
        {
            const std::string path = "cipher_get_test_1.txt";
            WriteFile(path, "hello world");

            const std::string result = get_input(path);
            std::remove(path.c_str());

            Assert::AreEqual(std::string("hello world"), result);
        }

        TEST_METHOD(EmptyTxtFile_ReturnsEmptyString)
        {
            const std::string path = "cipher_get_test_2.txt";
            WriteFile(path, "");

            const std::string result = get_input(path);
            std::remove(path.c_str());

            Assert::AreEqual(std::string(""), result);
        }

        TEST_METHOD(MultilineTxtFile_ReturnsAllContent)
        {
            const std::string path = "cipher_get_test_3.txt";
            const std::string content = "line1\nline2\nline3";
            WriteFile(path, content);

            const std::string result = get_input(path);
            std::remove(path.c_str());

            Assert::AreEqual(content, result);
        }

        TEST_METHOD(NonExistentTxtFile_Throws)
        {
            Assert::ExpectException<std::runtime_error>([]() {
                get_input("definitely_does_not_exist_xyz.txt");
                });
        }
    };
}
