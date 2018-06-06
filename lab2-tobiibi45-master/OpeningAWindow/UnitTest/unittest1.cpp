#include "stdafx.h"
#include "CppUnitTest.h"
#include <FirstGame.h>
#include <ModelManager.h>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(ArithmeticTest)
		{
			// ARRANGE
			int a = 3;
			int b = 6;
			// ACTION
			int c = a + b;
			int d = b + a;
			// ASSERT
			Assert::AreEqual(c, d);
		}

		TEST_METHOD(IncorrectJSONFileTest)
		{
			//// ARRANGE
			//FirstGame game;
			//// ACTION
			//bool loadSuccess = game.loadLevelJSON("JSONFailure1.json");
			//// ASSERT
			//Assert::IsFalse(loadSuccess);
			//// SECOND ACTION
			//loadSuccess = game.loadLevelJSON("JSONFailure2.json");
			//// SECOND ASSERT
			//Assert::IsFalse(loadSuccess);

		}

		TEST_METHOD(IncorrectModel)
		{
			//// ARRANGE
			//ModelManager MM;
			//// ACTION
			//bool loadSuccess = MM.load_model("MODELSuccess.json");
			//// ASSERT
			//Assert::IsTrue(loadSuccess);
			//// SECOND ACTION
			//loadSuccess = MM.load_model("MODELFailure.json");
			//// SECOND ASSERT
			//Assert::IsFalse(loadSuccess);

		}

	};
}