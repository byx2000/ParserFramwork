#pragma once

#include "../../Framwork/Rule.h"

#include <string>
#include <stack>

namespace byx
{
	class ExprCalc1
	{
	public:
		static double Eval(const std::string& s);
	private:
		static Rule<char> digit, add, sub, mul, div, lp, rp, dot;
		static Rule<char> integer, decimal;
		static Rule<char> lazyFact, lazyExpr, fact, term, expr;

		static std::stack<double> stack;

		static Rule<char> GetExpr();
		static Rule<char> GetFact();
		static void PushValue(const std::vector<char>& input, size_t begin, size_t end);
		static void CalcBinary(const std::vector<char>& input, size_t begin, size_t end);
		static void CalcUnary(const std::vector<char>& input, size_t begin, size_t end);
	};
}