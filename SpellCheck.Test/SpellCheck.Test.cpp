#include "pch.h"
#include "CppUnitTest.h"
#include "../SpellCheck.Core/SpellCheck.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SpellCheckTest
{
	TEST_CLASS(SpellCheckTest)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			Assert::AreEqual("This is a test.", test().c_str());
		}
	};
}
