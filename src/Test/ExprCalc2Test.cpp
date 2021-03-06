#include "ExprCalc2Test.h"
#include "../Samples/ExprCalc2/ExprCalc2.h"

#include <cassert>

using namespace std;
using namespace byx;

void byx::ExprCalc2Test::Run()
{
	assert(ExprCalc2::Eval("1") == 1);
	assert(ExprCalc2::Eval("2.5") == 2.5);
	assert(ExprCalc2::Eval(" ( 1 ) ") == 1);
	assert(ExprCalc2::Eval("(2.5)") == 2.5);
	assert(ExprCalc2::Eval("((1))") == 1);
	assert(ExprCalc2::Eval("((2.5))") == 2.5);
	assert(ExprCalc2::Eval("+1") == 1);
	assert(ExprCalc2::Eval("+2.5") == 2.5);
	assert(ExprCalc2::Eval("++++1") == 1);
	assert(ExprCalc2::Eval("+-1") == -1);
	assert(ExprCalc2::Eval("+ ++ + +2.5") == 2.5);
	assert(ExprCalc2::Eval("(+1)") == 1);
	assert(ExprCalc2::Eval("(+2.5)") == 2.5);
	assert(ExprCalc2::Eval("-1") == -1);
	assert(ExprCalc2::Eval("-2.5") == -2.5);
	assert(ExprCalc2::Eval("-----1") == -1);
	assert(ExprCalc2::Eval("------2.5") == 2.5);
	assert(ExprCalc2::Eval("(-1)") == -1);
	assert(ExprCalc2::Eval("(-2.5)") == -2.5);
	assert(ExprCalc2::Eval("(-1)") == -1);
	assert(ExprCalc2::Eval("(-2.5)") == -2.5);
	assert(ExprCalc2::Eval("1 + 2") == 3);
	assert(ExprCalc2::Eval("+1+2") == 3);
	assert(ExprCalc2::Eval("+1+(+2)") == 3);
	assert(ExprCalc2::Eval("-1+2") == 1);
	assert(ExprCalc2::Eval("1+2.56") == 1 + 2.56);
	assert(ExprCalc2::Eval("2.56-1") == 2.56 - 1);
	assert(ExprCalc2::Eval("2.7-3.4") == 2.7 - 3.4);
	assert(ExprCalc2::Eval("1+2*3") == 7);
	assert(ExprCalc2::Eval("(1+2)*(3+4)") == 21);
	assert(ExprCalc2::Eval(" 2/3-4") == 2.0 / 3 - 4);
	assert(ExprCalc2::Eval("(4.567*3+6.4/21)/(12.45-(-2-3+4.5*6))") == (4.567 * 3 + 6.4 / 21) / (12.45 - (-2 - 3 + 4.5 * 6)));
	assert(ExprCalc2::Eval("1*2*3*4*5+6*7*8*9*10-11*12") == 30228);
	assert(ExprCalc2::Eval("24-6+4+4/2*3-(10-2)") == 20);
	assert(ExprCalc2::Eval("(5873.2567+3.1415)*(-2.5 / 666)-(27+35)*(8.444-1231)") == (5873.2567 + 3.1415) * (-2.5 / 666) - (27.0 + 35) * (8.444 - 1231));
	assert(ExprCalc2::Eval("-(((6+6)*(6+3)*2+6)*2)-(24-6+(+4)+4/2*3-(10-2))") == -464);
}
