#include <iostream>
#include <string>
#include <stack>
#include "Framwork/Scanner.h"
#include "Framwork/ParserCombinator.h"
#include "Test/ParserCombinatorTest.h"
#include "Framwork/Rule.h"
#include "Samples/CLexer/CLexer.h"
#include "Test/CLexerTest.h"
#include "Samples/BracketMatch/BracketMatch.h"
#include "Test/BracketMatchTest.h"
#include "Samples/ExprCalc1/ExprCalc1.h"
#include "Test/ExprCalc1Test.h"
#include "Samples/ExprCalc2/ExprCalc2.h"
#include "Test/ExprCalc2Test.h"

using namespace std;
using namespace byx;

int main()
{
	// ÄÚ´æÐ¹Â©¼ì²â
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// µ¥Ôª²âÊÔ
	ParserCombinatorTest::Run();
	CLexerTest::Run();
	BracketMatchTest::Run();
	ExprCalc1Test::Run();
	ExprCalc2Test::Run();

	return 0;
}