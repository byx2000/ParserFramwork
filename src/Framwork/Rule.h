/******************************************************
 *	Author: byx
 *	Date: 2020/6/4
******************************************************/

#pragma once

#include "ParserCombinator.h"

namespace byx
{
	template<typename T>
	class Rule
	{
	public:
		Rule(const std::shared_ptr<Parser<T>>& p) : p(p) {}

		static Rule Empty() 
		{ 
			return Rule(std::make_shared<byx::Empty<T>>()); 
		}

		static Rule Any() 
		{ 
			return Rule(std::make_shared<byx::Any<T>>());
		}

		static Rule Symbol(const T& elem) 
		{ 
			return Rule(std::make_shared<byx::Symbol<T>>(elem));
		}

		static Rule Exclude(const T& elem)
		{
			return Rule(std::make_shared<byx::Exclude<T>>(elem));
		}

		static Rule SymbolSet(const std::vector<T>& symbols)
		{
			return Rule(std::make_shared<byx::SymbolSet<T>>(symbols));
		}

		static Rule SymbolSet(const std::initializer_list<T>& symbols)
		{
			return Rule(std::make_shared<byx::SymbolSet<T>>(symbols));
		}

		static Rule ExcludeSet(const std::vector<T>& symbols)
		{
			return Rule(std::make_shared<byx::ExcludeSet<T>>(symbols));
		}

		static Rule ExcludeSet(const std::initializer_list<T>& symbols)
		{
			return Rule(std::make_shared<byx::SymbolSet<T>>(symbols));
		}

		static Rule Satisfy(const Rule& rule, const std::function<bool(const std::vector<T>&, size_t, size_t)>& predicate) 
		{ 
			return Rule(std::make_shared<byx::Satisfy<T>>(rule.p, predicate)); 
		}

		static Rule Expect(const std::function<bool(const T&)>& predicate)
		{
			return Rule(std::make_shared<byx::Expect<T>>(predicate));
		}

		static Rule Lazy(const std::function<Rule(void)>& getRule)
		{
			auto getParser = [getRule]()
			{
				return getRule().p;
			};
			return Rule(std::make_shared<byx::Lazy<T>>(getParser));
		}

		Rule zeroOrMore() const 
		{ 
			return Rule(std::make_shared<byx::ZeroOrMore<T>>(p));
		}

		Rule oneOrMore() const 
		{ 
			return Rule(std::make_shared<byx::OneOrMore<T>>(p)); 
		}

		Rule setCallback(const std::function<void(const std::vector<T>&, size_t begin, size_t end)>& callback) const
		{
			return Rule(std::make_shared<byx::CallbackAfterSucceeded<T>>(p, callback));
		}

		Rule operator+(const Rule& rule) const
		{
			return Rule(std::make_shared<byx::Concat<T>>(p, rule.p));
		}

		Rule concat(const Rule& rule) const
		{
			return (*this) + rule;
		}

		Rule operator|(const Rule& rule) const
		{
			return Rule(std::make_shared<byx::Choose<T>>(p, rule.p));
		}

		Rule Or(const Rule & rule) const
		{
			return (*this) | rule;
		}

		int parse(const std::vector<T>& input)
		{
			Scanner<T> scanner(input);
			return p->parse(scanner);
		}
		
	private:
		std::shared_ptr<Parser<T>> p;
	};
	template class Rule<char>;
}