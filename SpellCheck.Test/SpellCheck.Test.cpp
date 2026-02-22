#include "pch.h"
#include "CppUnitTest.h"
#include "../SpellCheck.Core/SpellCheck.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SpellCheckTest
{
	TEST_CLASS(SpellCheckTest)
	{
	public:
		
		TEST_METHOD(dictionary_test)
		{
			std::vector<std::string> dict;
			create_dict(dict, "test_dict.txt");
			std::vector<std::string> expected = { "brown", "dog", "fox", "jumped", "lazy", "over", "quick", "the" };
			Assert::AreEqual(expected.size(), dict.size(), L"Dictionary size does not match expected size.");
			for (int i = 0; i < expected.size(); ++i)
			{
				Assert::AreEqual(expected[i].c_str(), dict[i].c_str(), L"Dictionary word does not match expected word.");
			}
		}
	};
}
