#include "BracketMatchTest.h"
#include "../Samples/BracketMatch/BracketMatch.h"

#include <cassert>

using namespace std;
using namespace byx;

void byx::BracketMatchTest::Run()
{
	assert(BracketMatch::Match(""));
	assert(!BracketMatch::Match("("));
	assert(!BracketMatch::Match(")"));
	assert(BracketMatch::Match("()"));
	assert(!BracketMatch::Match(")("));
	assert(!BracketMatch::Match("(("));
	assert(!BracketMatch::Match("))"));
	assert(!BracketMatch::Match("((("));
	assert(!BracketMatch::Match(")))"));
	assert(!BracketMatch::Match("))("));
	assert(!BracketMatch::Match(")(("));
	assert(!BracketMatch::Match("(()"));
	assert(!BracketMatch::Match("())"));
	assert(!BracketMatch::Match("()("));
	assert(!BracketMatch::Match(")()"));
	assert(BracketMatch::Match("()()"));
	assert(BracketMatch::Match("(())"));
	assert(BracketMatch::Match("()()()"));
	assert(BracketMatch::Match("()(())"));
	assert(BracketMatch::Match("(())()"));
	assert(!BracketMatch::Match("())()"));
	assert(!BracketMatch::Match(")(()"));
	assert(!BracketMatch::Match("(()))"));
	assert(BracketMatch::Match("()(()(()))(()())((()))"));
	assert(!BracketMatch::Match("()(()(()))((())((()))"));
	assert(!BracketMatch::Match(")(()(()))(()())((()))"));
	assert(!BracketMatch::Match("()(()(()))(()())((()("));
	assert(BracketMatch::Match("()(()(()))(()(()(()(()))(()())((()))))((()))()(()(()))(()())((()))"));
	assert(!BracketMatch::Match("()(()(()))(()(()(()(()))()())((()))))((()))()(()(()))(()())((()))"));
	assert(!BracketMatch::Match("()(()(()))(()(()(()(()))(()())((()))))((()))())(()(()))(()())((()))"));
}
