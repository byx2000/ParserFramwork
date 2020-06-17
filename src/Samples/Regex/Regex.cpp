#include "Regex.h"

#include <iostream>

using namespace std;
using namespace byx;

// Regex

Rule<char> Regex::empty = Rule<char>::Empty();
Rule<char> Regex::symbol = Rule<char>::ExcludeSet({ '*', '+', '(', ')', '\\', '|', ' ', '\t', '\n', '\r' }).setCallback(PushChar);
Rule<char> Regex::escape = Rule<char>::Symbol('\\');
Rule<char> Regex::lp = Rule<char>::Symbol('(');
Rule<char> Regex::rp = Rule<char>::Symbol(')');
Rule<char> Regex::star = Rule<char>::Symbol('*');
Rule<char> Regex::add = Rule<char>::Symbol('+');
Rule<char> Regex::choose = Rule<char>::Symbol('|');
Rule<char> Regex::dot = Rule<char>::Symbol('.');
Rule<char> Regex::escapeChar = (escape + escape).setCallback(PushEscapeChar('\\'))
                             | (escape + lp).setCallback(PushEscapeChar('('))
                             | (escape + rp).setCallback(PushEscapeChar(')'))
                             | (escape + star).setCallback(PushEscapeChar('*'))
                             | (escape + add).setCallback(PushEscapeChar('+'))
                             | (escape + choose).setCallback(PushEscapeChar('|'))
                             | (escape + dot).setCallback(PushEscapeChar('.'));
Rule<char> Regex::lazyExpr = Rule<char>::Lazy(GetExpr);
Rule<char> Regex::term = dot.setCallback(PushRule(Rule<char>::Any()))
                       | escapeChar 
                       | lp + lazyExpr + rp
                       | symbol;
Rule<char> Regex::factor_prime = star.setCallback(StarOrAdd)
                               | add.setCallback(StarOrAdd)
                               | empty;
Rule<char> Regex::factor = term + factor_prime;
Rule<char> Regex::catExpr = factor + factor.setCallback(Concat).zeroOrMore();
Rule<char> Regex::expr = catExpr + (choose + catExpr).setCallback(Choose).zeroOrMore();

stack<Rule<char>> Regex::stack;

bool byx::Regex::Match(const std::string& r, const std::string& s)
{
    while (!stack.empty()) stack.pop();
    int res = expr.parse(vector<char>(r.begin(), r.end()));
    /*for (size_t i = 0; i < (size_t)res; ++i)
    {
        cout << r[i];
    }
    cout << endl;*/
    Rule<char> rule = stack.top();
    res = rule.parse(vector<char>(s.begin(), s.end()));
    return res == s.size();
}

Rule<char> byx::Regex::GetExpr()
{
    return expr;
}

void byx::Regex::PushChar(const std::vector<char>& input, size_t begin, size_t end)
{
    stack.push(Rule<char>::Symbol(input[begin]));
}

std::function<void(const std::vector<char>&, size_t, size_t)> byx::Regex::PushEscapeChar(char c)
{
    return [c](const std::vector<char>& input, size_t begin, size_t end)
    {
        stack.push(Rule<char>::Symbol(c));
    };
}

std::function<void(const std::vector<char>&, size_t, size_t)> byx::Regex::PushRule(const Rule<char> r)
{
    return [r](const std::vector<char>& input, size_t begin, size_t end)
    {
        stack.push(r);
    };
}

void byx::Regex::StarOrAdd(const std::vector<char>& input, size_t begin, size_t end)
{
    Rule<char> r = stack.top();
    stack.pop();
    if (input[begin] == '*')
    {
        stack.push(r.zeroOrMore());
    }
    else if (input[begin] == '+')
    {
        stack.push(r.oneOrMore());
    }
}

void byx::Regex::Concat(const std::vector<char>& input, size_t begin, size_t end)
{
    Rule<char> rhs = stack.top();
    stack.pop();
    Rule<char> lhs = stack.top();
    stack.pop();
    stack.push(lhs + rhs);
}

void byx::Regex::Choose(const std::vector<char>& input, size_t begin, size_t end)
{
    Rule<char> rhs = stack.top();
    stack.pop();
    Rule<char> lhs = stack.top();
    stack.pop();
    stack.push(lhs | rhs);
}
