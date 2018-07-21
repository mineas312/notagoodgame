#include "stdafx.h"
#include "CppUnitTest.h"
#include <KTXImage.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace KTHTests
{		
	TEST_CLASS(ReadTests)
	{
	public:

		TEST_METHOD(ReadTest)
		{
			KTXImage img ("C:\\Code\\notagoodgame\\KTHTests\\lazy.KTX");

			Assert::IsTrue(img.depth == 0);
			Assert::IsTrue(img.width == 128);
			Assert::IsTrue(img.height == 128);

			Assert::IsNotNull(img.pixelData);
			Assert::IsTrue(img.mips == 1);
		}

	};
}