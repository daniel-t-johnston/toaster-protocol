#include "pch.h"
#include "CppUnitTest.h"
#include "../Cipher.Core/Cipher.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CipherTest
{
	TEST_CLASS(CipherTest)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			Assert::AreEqual("This is a test.", test().c_str());
		}
	};
}
