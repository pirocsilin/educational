#include "pch.h"
#include "CppUnitTest.h"
#include "../Labs_New_Tech/UFrac.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:							
		
		TEST_METHOD(InitTests)
		{
			TFract f = TFract();
			Assert::IsTrue(f.Display() == "0/1", L"Init test 1");
			f = TFract(-1, 2);
			Assert::IsTrue(f.Display() == "-1/2", L"Init test 2");
			f = TFract(3, 2);
			Assert::IsTrue(f.Display() == "3/2", L"Init test 3");
			f = TFract(3, 6);
			Assert::IsTrue(f.Display() == "1/2", L"Init test 4");
			f = TFract(0, 6);
			Assert::IsTrue(f.Display() == "0/6", L"Init test 5");
			f = TFract(-3, -6);
			Assert::IsTrue(f.Display() == "1/2", L"Init test 6");
			f = TFract(4, -16);
			Assert::IsTrue(f.Display() == "-1/4", L"Init test 7");
			f = TFract("-9/12");
			Assert::IsTrue(f.Display() == "-3/4", L"Init test 8");
			f = TFract("0/-12");
			Assert::IsTrue(f.Display() == "0/12", L"Init test 9");
		}

		TEST_METHOD(ArithmeticTests)
		{
			TFract f1 = TFract("1/2");
			TFract f2 = TFract("3/4");
			TFract f3 = f1+f2;
			Assert::AreEqual(f3.Display().c_str(), "5/4");
			Assert::AreEqual((f1 * f2).Display().c_str(), "3/8");
			Assert::AreEqual((f1 - f2).Display().c_str(), "-1/4");
			Assert::AreEqual((f1 / f2).Display().c_str(), "2/3");
			f2 = TFract("-3/4");
			Assert::AreEqual((f1 / f2).Display().c_str(), "-2/3");
			Assert::AreEqual((f2.GetSqr()).Display().c_str(), "9/16");
		}
		TEST_METHOD(CompairTests) {
			TFract f1 = TFract("3/4");
			TFract f2 = TFract("9/12");
			Assert::IsTrue(f1 == f2);
			Assert::IsFalse(f1 > f2);
			Assert::IsFalse(f1 < f2);
		}
	};
}
