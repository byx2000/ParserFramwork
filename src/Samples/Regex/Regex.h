#pragma once

#include "../../Framwork/Rule.h"

#include <stack>

namespace byx
{
	// 示例：简易正规式解析器
	class Regex
	{
	public:
		static bool Match(const std::string& r, const std::string& s);
	private:
		static Rule<char> empty, symbol, escape, lp, rp, star, add, choose, dot, escapeChar;
		static Rule<char> lazyExpr, term, factor, factor_prime, catExpr, expr;
		static std::stack<Rule<char>> stack;

		static Rule<char> GetExpr();

		static void PushChar(const std::vector<char>& input, size_t begin, size_t end);
		static std::function<void(const std::vector<char>&, size_t, size_t)> PushEscapeChar(char c);
		static std::function<void(const std::vector<char>&, size_t, size_t)> PushRule(const Rule<char> r);
		static void StarOrAdd(const std::vector<char>& input, size_t begin, size_t end);
		static void Concat(const std::vector<char>& input, size_t begin, size_t end);
		static void Choose(const std::vector<char>& input, size_t begin, size_t end);
	};
}