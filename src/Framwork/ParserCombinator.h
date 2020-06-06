/******************************************************
 *	Author: byx
 *	Date: 2020/6/4
******************************************************/

#pragma once

#include "Scanner.h"

#include <memory>
#include <functional>
#include <string>

namespace byx
{
	// Parser<T>

	template<typename T>
	class Parser
	{
	public:
		bool parse(Scanner<T>& input)
		{
			size_t backup = input.getIndex();
			if (!consume(input))
			{
				input.setIndex(backup);
				return false;
			}
			return true;
		}
		virtual ~Parser() { }
	protected:
		virtual bool consume(Scanner<T>& input) = 0;
	};

	// Empty<T>

	template<typename T>
	class Empty : public Parser<T>
	{
	protected:
		virtual bool consume(Scanner<T>& input) override
		{
			return true;
		}
	};

	// Any<T>

	template<typename T>
	class Any : public Parser<T>
	{
	protected:
		virtual bool consume(Scanner<T>& input) override
		{
			if (input.end())
			{
				return false;
			}
			input.next();
			return true;
		}
	};

	// Expect<T>

	template<typename T>
	class Expect : public Parser<T>
	{
	public:
		Expect(const std::function<bool(const T&)>& predicate) : predicate(predicate) {}
	protected:
		virtual bool consume(Scanner<T>& input) override
		{
			if (input.end())
			{
				return false;
			}
			return predicate(input.next());
		}
	private:
		std::function<bool(const T&)> predicate;
	};

	// Satisfy<T>

	template<typename T>
	class Satisfy : public Parser<T>
	{
	public:
		Satisfy(const std::shared_ptr<Parser<T>>& parser, const std::function<bool(const std::vector<T>&, size_t, size_t)>& predicate)
			: parser(parser), predicate(predicate) {}
	protected:
		virtual bool consume(Scanner<T>& input) override
		{
			if (input.end())
			{
				return false;
			}

			size_t begin = input.getIndex();
			if (!parser->parse(input))
			{
				return false;
			}

			return predicate(input.getElements(), begin, input.getIndex());
		}
	private:
		std::shared_ptr<Parser<T>> parser;
		std::function<bool(const std::vector<T>&, size_t, size_t)> predicate;
	};

	// Symbol<T>

	template<typename T>
	class Symbol : public Parser<T>
	{
	public:
		Symbol(const T& elem) : elem(elem) {}
	protected:
		virtual bool consume(Scanner<T>& input) override
		{
			if (input.end())
			{
				return false;
			}
			T e = input.next();
			return e == elem;
		}
	private:
		T elem;
	};

	// Exclude<T>

	template<typename T>
	class Exclude : public Parser<T>
	{
	public:
		Exclude(const T& elem) : elem(elem) {}
	protected:
		virtual bool consume(Scanner<T>& input) override
		{
			if (input.end())
			{
				return false;
			}
			T e = input.next();
			return e != elem;
		}
	private:
		T elem;
	};

	// SymbolSet<T>

	template<typename T>
	class SymbolSet : public Parser<T>
	{
	public:
		SymbolSet(const std::vector<T>& symbols) : symbols(symbols) {}
		SymbolSet(const std::initializer_list<T>& symbols)
		{
			for (auto item : symbols)
			{
				this->symbols.push_back(item);
			}
		}
	protected:
		virtual bool consume(Scanner<T>& input) override
		{
			if (input.end())
			{
				return false;
			}

			T elem = input.next();
			for (size_t i = 0; i < symbols.size(); ++i)
			{
				if (symbols[i] == elem)
				{
					return true;
				}
			}
			return false;
		}
	private:
		std::vector<T> symbols;
	};

	// ExcludeSet<T>

	template<typename T>
	class ExcludeSet : public Parser<T>
	{
	public:
		ExcludeSet(const std::vector<T>& symbols) : symbols(symbols) {}
		ExcludeSet(const std::initializer_list<T>& symbols)
		{
			for (auto item : symbols)
			{
				this->symbols.push_back(item);
			}
		}
	protected:
		virtual bool consume(Scanner<T>& input) override
		{
			if (input.end())
			{
				return false;
			}

			T elem = input.next();
			for (size_t i = 0; i < symbols.size(); ++i)
			{
				if (symbols[i] == elem)
				{
					return false;
				}
			}
			return true;
		}
	private:
		std::vector<T> symbols;
	};

	// ZeroOrMore<T>

	template<typename T>
	class ZeroOrMore : public Parser<T>
	{
	public:
		ZeroOrMore(const std::shared_ptr<Parser<T>>& parser) : parser(parser) {}
	protected:
		virtual bool consume(Scanner<T>& input) override
		{
			while (parser->parse(input));
			return true;
		}
	private:
		std::shared_ptr<Parser<T>> parser;
	};

	// OneOrMore<T>

	template<typename T>
	class OneOrMore : public Parser<T>
	{
	public:
		OneOrMore(const std::shared_ptr<Parser<T>>& parser) : parser(parser) {}
	protected:
		virtual bool consume(Scanner<T>& input) override
		{
			if (!parser->parse(input))
			{
				return false;
			}
			while (parser->parse(input));
			return true;
		}
	private:
		std::shared_ptr<Parser<T>> parser;
	};

	// Concat<T>

	template<typename T>
	class Concat : public Parser<T>
	{
	public:
		Concat(const std::shared_ptr<Parser<T>>& lhs, const std::shared_ptr<Parser<T>>& rhs)
			: lhs(lhs), rhs(rhs) {}
	protected:
		virtual bool consume(Scanner<T>& input) override
		{
			if (!lhs->parse(input))
			{
				return false;
			}
			return rhs->parse(input);
		}
	private:
		std::shared_ptr<Parser<T>> lhs, rhs;
	};

	// Choose<T>

	template<typename T>
	class Choose : public Parser<T>
	{
	public:
		Choose(const std::shared_ptr<Parser<T>>& lhs, const std::shared_ptr<Parser<T>>& rhs)
			: lhs(lhs), rhs(rhs) {}
	protected:
		virtual bool consume(Scanner<T>& input) override
		{
			if (lhs->parse(input))
			{
				return true;
			}
			return rhs->parse(input);
		}
	private:
		std::shared_ptr<Parser<T>> lhs, rhs;
	};

	// CallbackAfterSucceeded<T>

	template<typename T>
	class CallbackAfterSucceeded : public Parser<T>
	{
	public:
		CallbackAfterSucceeded(const std::shared_ptr<Parser<T>>& parser, const std::function<void(const std::vector<T>&, size_t begin, size_t end)>& callback)
			: parser(parser), callback(callback) {}
	protected:
		virtual bool consume(Scanner<T>& input) override
		{
			size_t begin = input.getIndex();
			if (parser->parse(input))
			{
				callback(input.getElements(), begin, input.getIndex());
				return true;
			}
			return false;
		}
	public:
		std::shared_ptr<Parser<T>> parser;
		std::function<void(const std::vector<T>&, size_t begin, size_t end)> callback;
	};

	// Lazy<T>

	template<typename T>
	class Lazy : public Parser<T>
	{
	public:
		Lazy(const std::function<std::shared_ptr<Parser<T>>(void)>& getParser) : getParser(getParser) {}
	protected:
		virtual bool consume(Scanner<T>& input) override
		{
			return getParser()->parse(input);
		}
	private:
		std::function<std::shared_ptr<Parser<T>>(void)> getParser;
	};

	// Range<char>

	class Range : public Parser<char>
	{
	public:
		Range(char c1, char c2) : c1(c1), c2(c2) {}
	protected:
		virtual bool consume(Scanner<char>& input) override
		{
			if (input.end())
			{
				return false;
			}
			char c = input.next();
			return (c - c1) * (c - c2) <= 0;
		}
	private:
		char c1, c2;
	};

	// Prefix<char>

	class Prefix : public Parser<char>
	{
	public:
		Prefix(const std::string& pre) : pre(pre) {}
	protected:
		virtual bool consume(Scanner<char>& input) override
		{
			for (size_t i = 0; i < pre.size(); ++i)
			{
				if (input.end() || input.next() != pre[i])
				{
					return false;
				}
			}
			return true;
		}
	private:
		std::string pre;
	};
}