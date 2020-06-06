#pragma once

#include "../../Framwork/Rule.h"

#include <string>
#include <vector>

namespace byx
{
	class ExprCalc2
	{
	public:
		// 单词类型
		enum class TokenType { Integer, Decimal, Add, Sub, Mul, Div, OpenBracket, CloseBracket };

		// 单词
		struct Token
		{
			TokenType type;
			std::string value;
			Token(TokenType type, const std::string& value);
			std::string toString() const;
		};

		static double Eval(const std::string& s);

	private:
		static Rule<char> blank, dot, digit, integer, decimal, add, sub, mul, div, lp, rp, rule;
		static Rule<Token> integerToken, decimalToken, addToken, subToken, mulToken, divToken, lpToken, rpToken, factor, term, expr, lazyExpr, lazyFactor;

		static std::vector<Token> tokens;
		static std::stack<double> stack;

		static void Lex(const std::string& input);
		static std::function<void(const std::vector<char>&, size_t, size_t)> AddToken(TokenType type);
		static std::function<bool(const Token&)> ExpectTokenType(TokenType type);
		static Rule<Token> GetExpr();
		static Rule<Token> GetFactor();
		static void PushValue(const std::vector<Token>& tokens, size_t begin, size_t end);
		static void CalcBinary(const std::vector<Token>& tokens, size_t begin, size_t end);
		static void CalcUnary(const std::vector<Token>& tokens, size_t begin, size_t end);
	};
}