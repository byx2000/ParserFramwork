#include "ExprCalc2.h"

#include <iostream>
#include <sstream>

using namespace std;
using namespace byx;

using TokenType = ExprCalc2::TokenType;
using Token = ExprCalc2::Token;

// Token

byx::ExprCalc2::Token::Token(TokenType type, const std::string& value) : type(type), value(value) {}

std::string byx::ExprCalc2::Token::toString() const
{
	static string desc[] = { "Integer", "Decimal", "Add", "Sub", "Mul", "Div", "OpenBracket", "CloseBracket" };
	string s = "type: ";
	s += desc[(int)type];
	for (size_t i = s.size(); i < 20; ++i)
	{
		s.push_back(' ');
	}
	s += "\tvalue: ";
	s += value;
	return s;
}

// ExprCalc2

Rule<char> ExprCalc2::blank = Rule<char>::SymbolSet({ ' ', '\t', '\n', '\r' });
Rule<char> ExprCalc2::dot = Rule<char>::Symbol('.');
Rule<char> ExprCalc2::digit = Rule<char>(make_shared<Range>('0', '9'));
Rule<char> ExprCalc2::integer = digit.oneOrMore().setCallback(AddToken(TokenType::Integer));
Rule<char> ExprCalc2::decimal = (digit.oneOrMore() + dot + digit.oneOrMore()).setCallback(AddToken(TokenType::Decimal));
Rule<char> ExprCalc2::add = Rule<char>::Symbol('+').setCallback(AddToken(TokenType::Add));
Rule<char> ExprCalc2::sub = Rule<char>::Symbol('-').setCallback(AddToken(TokenType::Sub));
Rule<char> ExprCalc2::mul = Rule<char>::Symbol('*').setCallback(AddToken(TokenType::Mul));
Rule<char> ExprCalc2::div = Rule<char>::Symbol('/').setCallback(AddToken(TokenType::Div));
Rule<char> ExprCalc2::lp = Rule<char>::Symbol('(').setCallback(AddToken(TokenType::OpenBracket));
Rule<char> ExprCalc2::rp = Rule<char>::Symbol(')').setCallback(AddToken(TokenType::CloseBracket));
Rule<char> ExprCalc2::rule = (blank | decimal | integer | add | sub | mul | div | lp | rp).oneOrMore();

Rule<Token> ExprCalc2::integerToken = Rule<Token>::Expect(ExpectTokenType(TokenType::Integer));
Rule<Token> ExprCalc2::decimalToken = Rule<Token>::Expect(ExpectTokenType(TokenType::Decimal));
Rule<Token> ExprCalc2::addToken = Rule<Token>::Expect(ExpectTokenType(TokenType::Add));
Rule<Token> ExprCalc2::subToken = Rule<Token>::Expect(ExpectTokenType(TokenType::Sub));
Rule<Token> ExprCalc2::mulToken = Rule<Token>::Expect(ExpectTokenType(TokenType::Mul));
Rule<Token> ExprCalc2::divToken = Rule<Token>::Expect(ExpectTokenType(TokenType::Div));
Rule<Token> ExprCalc2::lpToken = Rule<Token>::Expect(ExpectTokenType(TokenType::OpenBracket));
Rule<Token> ExprCalc2::rpToken = Rule<Token>::Expect(ExpectTokenType(TokenType::CloseBracket));
Rule<Token> ExprCalc2::lazyExpr = Rule<Token>::Lazy(GetExpr);
Rule<Token> ExprCalc2::lazyFactor = Rule<Token>::Lazy(GetFactor);
Rule<Token> ExprCalc2::factor = integerToken.setCallback(PushValue)
                              | decimalToken.setCallback(PushValue)
	                          | (addToken + lazyFactor).setCallback(CalcUnary)
	                          | (subToken + lazyFactor).setCallback(CalcUnary)
                              | lpToken + lazyExpr + rpToken;
Rule<Token> ExprCalc2::term = factor + ((mulToken | divToken) + factor).setCallback(CalcBinary).zeroOrMore();
Rule<Token> ExprCalc2::expr = term + ((addToken | subToken) + term).setCallback(CalcBinary).zeroOrMore();

vector<Token> ExprCalc2::tokens;
stack<double> ExprCalc2::stack;

double byx::ExprCalc2::Eval(const std::string& s)
{
	tokens.clear();
	Lex(s);
	while (!stack.empty()) stack.pop();
	expr.parse(tokens);
	return stack.top();
}

void byx::ExprCalc2::Lex(const std::string& input)
{
	rule.parse(vector<char>(input.begin(), input.end()));
}

std::function<void(const std::vector<char>&, size_t, size_t)> byx::ExprCalc2::AddToken(TokenType type)
{
	return [type](const std::vector<char>& input, size_t begin, size_t end)
	{
		string val = "";
		for (size_t i = begin; i < end; ++i)
		{
			val.push_back(input[i]);
		}
		tokens.push_back(Token(type, val));
	};
}

std::function<bool(const Token& token)> byx::ExprCalc2::ExpectTokenType(TokenType type)
{
	return [type](const Token& token)
	{
		return token.type == type;
	};
}

Rule<Token> byx::ExprCalc2::GetExpr()
{
	return expr;
}

Rule<Token> byx::ExprCalc2::GetFactor()
{
	return factor;
}

void byx::ExprCalc2::PushValue(const std::vector<Token>& tokens, size_t begin, size_t end)
{
	double val;
	stringstream(tokens[begin].value) >> val;
	stack.push(val);
}

void byx::ExprCalc2::CalcBinary(const std::vector<Token>& tokens, size_t begin, size_t end)
{
	double rhs = stack.top();
	stack.pop();
	double lhs = stack.top();
	stack.pop();
	TokenType op = tokens[begin].type;
	switch (op)
	{
	case TokenType::Add:
		stack.push(lhs + rhs);
		break;
	case TokenType::Sub:
		stack.push(lhs - rhs);
		break;
	case TokenType::Mul:
		stack.push(lhs * rhs);
		break;
	case TokenType::Div:
		stack.push(lhs / rhs);
		break;
	default:
		break;
	}
}

void byx::ExprCalc2::CalcUnary(const std::vector<Token>& tokens, size_t begin, size_t end)
{
	double val = stack.top();
	stack.pop();
	TokenType op = tokens[begin].type;
	switch (op)
	{
	case TokenType::Add:
		stack.push(val);
		break;
	case TokenType::Sub:
		stack.push(-val);
		break;
	default:
		break;
	}
}
