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
		// 解析成功，返回解析后输入流的索引
		// 解析失败，返回一个小于0的值
		int parse(Scanner<T>& input)
		{
			size_t backup = input.getIndex();
			if (!consume(input))
			{
				input.setIndex(backup);
				return -1;
			}
			return input.getIndex();
		}
		virtual ~Parser() { }
	protected:
		// 消耗输入流，消耗成功返回true，否则返回false
		virtual bool consume(Scanner<T>& input) = 0;
	};

	// Empty<T>
	// 不消耗输入，永远返回成功

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
	// 输入流前移一位，返回成功
	// 若无法前移（输入流已到末尾），返回失败

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
	// 检查输入流下一个元素是否满足指定条件

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
	// 检查指定解析器的解析结果是否符合指定条件

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
	// 检查输入流下一个元素是否为期望元素

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
	// 检查输入流下一个元素是否不等于指定元素

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
	// 检查输入流下一个元素是否在可接受集合之中

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
	// 检查输入流下一个元素是否在不可接受集合之中

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
	// 对指定解析器应用零次或多次

	template<typename T>
	class ZeroOrMore : public Parser<T>
	{
	public:
		ZeroOrMore(const std::shared_ptr<Parser<T>>& parser) : parser(parser) {}
	protected:
		virtual bool consume(Scanner<T>& input) override
		{
			while (parser->parse(input) > 0);
			return true;
		}
	private:
		std::shared_ptr<Parser<T>> parser;
	};

	// OneOrMore<T>
	// 对指定解析器应用至少一次

	template<typename T>
	class OneOrMore : public Parser<T>
	{
	public:
		OneOrMore(const std::shared_ptr<Parser<T>>& parser) : parser(parser) {}
	protected:
		virtual bool consume(Scanner<T>& input) override
		{
			if (parser->parse(input) <= 0)
			{
				return false;
			}
			while (parser->parse(input) > 0);
			return true;
		}
	private:
		std::shared_ptr<Parser<T>> parser;
	};

	// Concat<T>
	// 依次应用两个解析器

	template<typename T>
	class Concat : public Parser<T>
	{
	public:
		Concat(const std::shared_ptr<Parser<T>>& lhs, const std::shared_ptr<Parser<T>>& rhs)
			: lhs(lhs), rhs(rhs) {}
	protected:
		virtual bool consume(Scanner<T>& input) override
		{
			if (lhs->parse(input) < 0)
			{
				return false;
			}
			return rhs->parse(input) >= 0;
		}
	private:
		std::shared_ptr<Parser<T>> lhs, rhs;
	};

	// Choose<T>
	// 应用两个解析器中的一个

	template<typename T>
	class Choose : public Parser<T>
	{
	public:
		Choose(const std::shared_ptr<Parser<T>>& lhs, const std::shared_ptr<Parser<T>>& rhs)
			: lhs(lhs), rhs(rhs) {}
	protected:
		virtual bool consume(Scanner<T>& input) override
		{
			if (lhs->parse(input) >= 0)
			{
				return true;
			}
			return rhs->parse(input) >= 0;
		}
	private:
		std::shared_ptr<Parser<T>> lhs, rhs;
	};

	// CallbackAfterSucceeded<T>
	// 解析成功后调用回调函数

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
			if (parser->parse(input) >= 0)
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
	// 惰性获取解析器

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
	// 指定字符范围

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
	// 指定字符串前缀

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