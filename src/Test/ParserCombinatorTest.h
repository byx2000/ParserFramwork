#pragma once

#include "../Framwork/Rule.h"

#include <string>
#include <stack>

namespace byx
{
	class ParserCombinatorTest
	{
	public:
		static void Run();
	private:
		static void EmptyTest();
		static void AnyTest();
		static void ExpectTest();
		static void SatisfyTest();
		static void SymbolTest();
		static void ExcludeTest();
		static void SymbolSetTest();
		static void ExcludeSetTest();
		static void ZeroOrMoreTest();
		static void OneOrMoreTest();
		static void ConcatTest();
		static void ChooseTest();
		static void CallbackAfterSucceededTest();
		static void LazyTest();
		static void RangeTest();
		static void PrefixTest();

		static void SimpleLexerTest();
		static void SimpleExprCalcTest();

		class SimpleLexer
		{
		public:
			static std::vector<std::pair<std::string, bool>> Lex(const std::string& s);
		private:
			static Rule<char> blank, dot, digit, integer, decimal, rule;
			static void AddInteger(const std::vector<char>& input, size_t begin, size_t end);
			static void AddDecimal(const std::vector<char>& input, size_t begin, size_t end);
			static std::vector<std::pair<std::string, bool>> tokens;
		};

		class SimpleExprCalc
		{
		public:
			static int Eval(const std::string& s);
		private:
			static Rule<char> digit, integer, add, sub, mul, div, lp, rp, lazyExpr, factor, term, expr;
			static std::stack<int> stack;
			static Rule<char> getExpr();
			static void PushInteger(const std::vector<char>& input, size_t begin, size_t end);
			static void Calculate(const std::vector<char>& input, size_t begin, size_t end);
		};
	};
}