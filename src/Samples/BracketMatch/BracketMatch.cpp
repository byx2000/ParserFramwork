#include "BracketMatch.h"

using namespace std;
using namespace byx;

// BracketMatch

Rule<char> BracketMatch::lp = Rule<char>::Symbol('(');
Rule<char> BracketMatch::rp = Rule<char>::Symbol(')');
Rule<char> BracketMatch::empty = Rule<char>::Empty();
Rule<char> BracketMatch::lazyExpr = Rule<char>::Lazy(GetExpr);
Rule<char> BracketMatch::term = lp + lazyExpr + rp;
Rule<char> BracketMatch::expr = term + lazyExpr | empty;

bool byx::BracketMatch::Match(const std::string& s)
{
    return expr.parse(vector<char>(s.begin(), s.end())) == s.size();
}

Rule<char> byx::BracketMatch::GetExpr()
{
    return expr;
}
