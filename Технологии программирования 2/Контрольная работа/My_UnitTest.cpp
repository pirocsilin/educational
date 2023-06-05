#include "pch.h"
#include "CppUnitTest.h"
#include "../Pset/Uset.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MyUnitTest
{
	TEST_CLASS(MyUnitTest) {
	public:
		
		TEST_METHOD(InitTests) {

			Tset<int> s1 = Tset<int>();
			s1.Tinsert(777);	
			s1.Tinsert(12);
			s1.Tinsert(34);
			s1.Tinsert(5);

			Tset<int> tmp = Tset<int>();

			Tset<int> s2 = Tset<int>({ 777, 12, 27, -4 });

			Assert::IsTrue(tmp.Tempty());
			Assert::IsFalse(s1.Tempty());

			Assert::IsTrue(s1.Tfind(777));
			Assert::IsFalse(s1.Tfind(-777));

			tmp = s1;
			Assert::IsTrue(tmp.Tsize() == 4);
			Assert::IsTrue(tmp.GetElem(1) == 5);
			
			tmp.Terase(5);
			Assert::IsFalse(tmp.GetElem(1) == 5);

			tmp.Tclear();
			Assert::IsTrue(tmp.Tempty());
		}

		TEST_METHOD(SumTests) {

			Tset<int> s1 = Tset<int>();
			Tset<int> s2 = Tset<int>();

			Assert::IsTrue(s1 + s2 == Tset<int>());
			Assert::IsTrue(Tset<int>({ 0 }) + s2 == Tset<int>({ 0 }));
			Assert::IsTrue(Tset<int>({ 0 }) + Tset<int>({ 1 }) == Tset<int>({ 0,1 }));
			Assert::IsTrue(Tset<int>({ 0,1 }) + Tset<int>({ 0,1 }) == Tset<int>({ 0,1 }));
			Assert::IsTrue(Tset<int>({ 1,2,3 }) + Tset<int>({ 3,4,5 }) == Tset<int>({ 1,2,3,4,5 }));
		}

		TEST_METHOD(MultiplyTests) {

			Assert::IsTrue(Tset<int>() * Tset<int>() == Tset<int>());
			Assert::IsTrue(Tset<int>({ 0 }) * Tset<int>() == Tset<int>());
			Assert::IsTrue(Tset<int>({ 0 }) * Tset<int>({ 1 }) == Tset<int>());
			Assert::IsTrue(Tset<int>({ 1,2,3 }) * Tset<int>({ 4,5,6 }) == Tset<int>());
			Assert::IsTrue(Tset<int>({ 0,1 }) * Tset<int>({ 0,1 }) == Tset<int>({ 0,1 }));
			Assert::IsTrue(Tset<int>({ 1,2,3 }) * Tset<int>({ 2,3,4 }) == Tset<int>({ 2,3 }));
		}

		TEST_METHOD(DifferenceTests) {

			Assert::IsTrue(Tset<int>() - Tset<int>() == Tset<int>());
			Assert::IsTrue(Tset<int>({ 0 }) - Tset<int>() == Tset<int>({ 0 }));
			Assert::IsTrue(Tset<int>() - Tset<int>({ 0 }) == Tset<int>());
			Assert::IsTrue(Tset<int>({ 0,1 }) - Tset<int>({ 0 }) == Tset<int>({ 1 }));
			Assert::IsTrue(Tset<int>({ 1,2,3 }) - Tset<int>({ 4,5,6 }) == Tset<int>({ 1,2,3 }));
			Assert::IsTrue(Tset<int>({ 1,2,3 }) - Tset<int>({ 3,4,5 }) == Tset<int>({ 1,2 }));
		}
	};
}
