#include "ExprCalc1.h"

#include <sstream>
#include <iostream>

using namespace std;
using namespace byx;

// ExprCalc1

Rule<char> ExprCalc1::digit = Rule<char>(make_shared<Range>('0', '9'));
Rule<char> ExprCalc1::add = Rule<char>::Symbol('+');
Rule<char> ExprCalc1::sub = Rule<char>::Symbol('-');
Rule<char> ExprCalc1::mul = Rule<char>::Symbol('*');
Rule<char> ExprCalc1::div = Rule<char>::Symbol('/');
Rule<char> ExprCalc1::lp = Rule<char>::Symbol('(');
Rule<char> ExprCalc1::rp = Rule<char>::Symbol(')');
Rule<char> ExprCalc1::dot = Rule<char>::Symbol('.');
Rule<char> ExprCalc1::integer = digit.oneOrMore().setCallback(PushValue);
Rule<char> ExprCalc1::decimal = (digit.oneOrMore() + dot + digit.oneOrMore()).setCallback(PushValue);
Rule<char> ExprCalc1::lazyFact = Rule<char>::Lazy(GetFact);
Rule<char> ExprCalc1::lazyExpr = Rule<char>::Lazy(GetExpr);

Rule<char> ExprCalc1::fact = decimal
                           | integer
                           | lp + lazyExpr + rp
                           | (add + lazyFact).setCallback(CalcUnary)
                           | (sub + lazyFact).setCallback(CalcUnary);
Rule<char> ExprCalc1::term = fact + ((mul | div) + fact).setCallback(CalcBinary).zeroOrMore();
Rule<char> ExprCalc1::expr = term + ((add | sub) + term).setCallback(CalcBinary).zeroOrMore();

stack<double> ExprCalc1::stack;

double byx::ExprCalc1::Eval(const std::string& s)
{
    while (!stack.empty()) stack.pop();
    expr.parse(vector<char>(s.begin(), s.end()));
    return stack.top();
}

Rule<char> byx::ExprCalc1::GetExpr()
{
    return expr;
}

Rule<char> byx::ExprCalc1::GetFact()
{
    return fact;
}

void byx::ExprCalc1::PushValue(const std::vector<char>& input, size_t begin, size_t end)
{
    string s = "";
    for (size_t i = begin; i < end; ++i)
    {
        s.push_back(input[i]);
    }

    double val;
    (stringstream() << s) >> val;
    stack.push(val);
}

void byx::ExprCalc1::CalcBinary(const std::vector<char>& input, size_t begin, size_t end)
{
    double rhs = stack.top();
    stack.pop();
    double lhs = stack.top();
    stack.pop();
    char op = input[begin];
    switch (op)
    {
    case '+':
        stack.push(lhs + rhs);
        break;
    case '-':
        stack.push(lhs - rhs);
        break;
    case '*':
        stack.push(lhs * rhs);
        break;
    case '/':
        stack.push(lhs / rhs);
        break;
    default:
        break;
    }
}

void byx::ExprCalc1::CalcUnary(const std::vector<char>& input, size_t begin, size_t end)
{
    double val = stack.top();
    stack.pop();
    char op = input[begin];
    switch (op)
    {
    case '+':
        stack.push(val);
        break;
    case '-':
        stack.push(-val);
        break;
    default:
        break;
    }
}
