#include "pch.h"
#include "CppUnitTest.h"
#include "../SpellCheck.Core/SpellCheck.h"

#include <fstream>
#include <vector>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SpellCheckTest
{
	TEST_CLASS(SpellCheckTest)
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

		TEST_METHOD(CreateDict_LoadsAllWords)
		{
			std::vector<std::string> dict;

			create_dict(dict, get_test_dict_path());

			Assert::AreEqual(static_cast<size_t>(8), dict.size());
		}

		TEST_METHOD(CreateDict_NormalizesAndSorts)
		{
			std::vector<std::string> dict;

			create_dict(dict, get_test_dict_path());

			std::vector<std::string> expected =
			{
				"brown",
				"dog",
				"fox",
				"jumped",
				"lazy",
				"over",
				"quick",
				"the"
			};

			Assert::IsTrue(dict == expected);
		}

		TEST_METHOD(CreateDict_ThrowsWhenDictionaryMissing)
		{
			std::vector<std::string> dict;

			Assert::ExpectException<std::runtime_error>([&dict]()
				{
					create_dict(dict, "definitely_missing_dictionary_file.txt");
				});
		}
	};
}
