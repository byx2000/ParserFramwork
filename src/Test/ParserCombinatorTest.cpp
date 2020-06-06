#include "ParserCombinatorTest.h"
#include "../Framwork/ParserCombinator.h"

#include <string>
#include <cassert>

using namespace std;
using namespace byx;

static Scanner<char> StringToScanner(const string& str)
{
	vector<char> v;
	for (size_t i = 0; i < str.size(); ++i)
	{
		v.push_back(str[i]);
	}
	return Scanner<char>(v);
}

// Range

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

// ParserCombinatorTest

void byx::ParserCombinatorTest::Run()
{
	EmptyTest();
	AnyTest();
	ExpectTest();
	SatisfyTest();
	SymbolTest();
	ExcludeTest();
	SymbolSetTest();
	ExcludeSetTest();
	ZeroOrMoreTest();
	OneOrMoreTest();
	ConcatTest();
	ChooseTest();
	CallbackAfterSucceededTest();
	LazyTest();
	RangeTest();
	PrefixTest();

	SimpleLexerTest();
	SimpleExprCalcTest();
}

void byx::ParserCombinatorTest::EmptyTest()
{
	Scanner<char> input = StringToScanner("abc");
	int res = Empty<char>().parse(input);
	assert(res == 0);
	assert(input.getIndex() == 0);

	input = StringToScanner("");
	res = Empty<char>().parse(input);
	assert(res == 0);
	assert(input.getIndex() == 0);
}

void byx::ParserCombinatorTest::AnyTest()
{
	Scanner<char> input = StringToScanner("abc");
	int res = Any<char>().parse(input);
	assert(res == 1);
	assert(input.getIndex() == 1);

	input = StringToScanner("a");
	res = Any<char>().parse(input);
	assert(res == 1);
	assert(input.getIndex() == 1);

	input = StringToScanner("");
	res = Any<char>().parse(input);
	assert(res < 0);
	assert(input.getIndex() == 0);
}

void byx::ParserCombinatorTest::ExpectTest()
{
	auto predicate = [](const char& c)
	{
		return c == 'a';
	};

	Scanner<char> input = StringToScanner("abc");
	int res = Expect<char>(predicate).parse(input);
	assert(res == 1);
	assert(input.getIndex() == 1);

	input = StringToScanner("a");
	res = Expect<char>(predicate).parse(input);
	assert(res == 1);
	assert(input.getIndex() == 1);

	input = StringToScanner("bcd");
	res = Expect<char>(predicate).parse(input);
	assert(res < 0);
	assert(input.getIndex() == 0);

	input = StringToScanner("b");
	res = Expect<char>(predicate).parse(input);
	assert(res < 0);
	assert(input.getIndex() == 0);

	input = StringToScanner("");
	res = Expect<char>(predicate).parse(input);
	assert(res < 0);
	assert(input.getIndex() == 0);
}

void byx::ParserCombinatorTest::SatisfyTest()
{
	auto isDigit = [](const vector<char>& input, size_t begin, size_t end) 
	{ 
		char c = input[begin];
		return c >= '0' && c <= '9'; 
	};

	Scanner<char> input = StringToScanner("123");
	int res =  Satisfy<char>(make_shared<Any<char>>(), isDigit).parse(input);
	assert(res == 1);
	assert(input.getIndex() == 1);

	input = StringToScanner("abc");
	res = Satisfy<char>(make_shared<Any<char>>(), isDigit).parse(input);
	assert(res < 0);
	assert(input.getIndex() == 0);

	input = StringToScanner("0");
	res = Satisfy<char>(make_shared<Any<char>>(), isDigit).parse(input);
	assert(res == 1);
	assert(input.getIndex() == 1);

	input = StringToScanner("a");
	res = Satisfy<char>(make_shared<Any<char>>(), isDigit).parse(input);
	assert(res < 0);
	assert(input.getIndex() == 0);

	input = StringToScanner("");
	res = Satisfy<char>(make_shared<Any<char>>(), isDigit).parse(input);
	assert(res < 0);
	assert(input.getIndex() == 0);
}

void byx::ParserCombinatorTest::SymbolTest()
{
	Scanner<char> input = StringToScanner("abc");
	int res = Symbol<char>('a').parse(input);
	assert(res == 1);
	assert(input.getIndex() == 1);

	input = StringToScanner("123");
	res = Symbol<char>('a').parse(input);
	assert(res < 0);
	assert(input.getIndex() == 0);

	input = StringToScanner("a");
	res = Symbol<char>('a').parse(input);
	assert(res == 1);
	assert(input.getIndex() == 1);

	input = StringToScanner("1");
	res = Symbol<char>('a').parse(input);
	assert(res < 0);
	assert(input.getIndex() == 0);

	input = StringToScanner("");
	res = Symbol<char>('a').parse(input);
	assert(res < 0);
	assert(input.getIndex() == 0);
}

void byx::ParserCombinatorTest::ExcludeTest()
{
	Scanner<char> input = StringToScanner("abc");
	int res = Exclude<char>('a').parse(input);
	assert(res < 0);
	assert(input.getIndex() == 0);

	input = StringToScanner("xyz");
	res = Exclude<char>('a').parse(input);
	assert(res == 1);
	assert(input.getIndex() == 1);

	input = StringToScanner("");
	res = Exclude<char>('a').parse(input);
	assert(res < 0);
	assert(input.getIndex() == 0);
}

void byx::ParserCombinatorTest::SymbolSetTest()
{
	Scanner<char> input = StringToScanner("apple");
	int res = SymbolSet<char>{ 'a', 'b', 'c' }.parse(input);
	assert(res == 1);
	assert(input.getIndex() == 1);

	input = StringToScanner("b");
	res = SymbolSet<char>{ 'a', 'b', 'c' }.parse(input);
	assert(res == 1);
	assert(input.getIndex() == 1);

	input = StringToScanner("cat");
	res = SymbolSet<char>{ 'a', 'b', 'c' }.parse(input);
	assert(res == 1);
	assert(input.getIndex() == 1);

	input = StringToScanner("delicious");
	res = SymbolSet<char>{ 'a', 'b', 'c' }.parse(input);
	assert(res < 0);
	assert(input.getIndex() == 0);

	input = StringToScanner("e");
	res = SymbolSet<char>{ 'a', 'b', 'c' }.parse(input);
	assert(res < 0);
	assert(input.getIndex() == 0);

	input = StringToScanner("");
	res = SymbolSet<char>{ 'a', 'b', 'c' }.parse(input);
	assert(res < 0);
	assert(input.getIndex() == 0);

	input = StringToScanner("abc");
	res = SymbolSet<char>{}.parse(input);
	assert(res < 0);
	assert(input.getIndex() == 0);

	input = StringToScanner("");
	res = SymbolSet<char>{}.parse(input);
	assert(res < 0);
	assert(input.getIndex() == 0);
}

void byx::ParserCombinatorTest::ExcludeSetTest()
{
	Scanner<char> input = StringToScanner("apple");
	int res = ExcludeSet<char>{ 'a', 'b', 'c' }.parse(input);
	assert(res < 0);
	assert(input.getIndex() == 0);

	input = StringToScanner("b");
	res = ExcludeSet<char>{ 'a', 'b', 'c' }.parse(input);
	assert(res < 0);
	assert(input.getIndex() == 0);

	input = StringToScanner("cat");
	res = ExcludeSet<char>{ 'a', 'b', 'c' }.parse(input);
	assert(res < 0);
	assert(input.getIndex() == 0);

	input = StringToScanner("delicious");
	res = ExcludeSet<char>{ 'a', 'b', 'c' }.parse(input);
	assert(res == 1);
	assert(input.getIndex() == 1);

	input = StringToScanner("e");
	res = ExcludeSet<char>{ 'a', 'b', 'c' }.parse(input);
	assert(res == 1);
	assert(input.getIndex() == 1);

	input = StringToScanner("");
	res = ExcludeSet<char>{ 'a', 'b', 'c' }.parse(input);
	assert(res < 0);
	assert(input.getIndex() == 0);
	
	input = StringToScanner("abc");
	res = ExcludeSet<char>{}.parse(input);
	assert(res == 1);
	assert(input.getIndex() == 1);

	input = StringToScanner("");
	res = ExcludeSet<char>{}.parse(input);
	assert(res < 0);
	assert(input.getIndex() == 0);
}

void byx::ParserCombinatorTest::ZeroOrMoreTest()
{
	Scanner<char> input = StringToScanner("aaaabb");
	int res = ZeroOrMore<char>(make_shared<Symbol<char>>('a')).parse(input);
	assert(res == 4);
	assert(input.getIndex() == 4);

	input = StringToScanner("aaaaaaa");
	res = ZeroOrMore<char>(make_shared<Symbol<char>>('a')).parse(input);
	assert(res == 7);
	assert(input.getIndex() == 7);

	input = StringToScanner("abbbbb");
	res = ZeroOrMore<char>(make_shared<Symbol<char>>('a')).parse(input);
	assert(res == 1);
	assert(input.getIndex() == 1);

	input = StringToScanner("a");
	res = ZeroOrMore<char>(make_shared<Symbol<char>>('a')).parse(input);
	assert(res == 1);
	assert(input.getIndex() == 1);

	input = StringToScanner("baaaaaaa");
	res = ZeroOrMore<char>(make_shared<Symbol<char>>('a')).parse(input);
	assert(res == 0);
	assert(input.getIndex() == 0);

	input = StringToScanner("b");
	res = ZeroOrMore<char>(make_shared<Symbol<char>>('a')).parse(input);
	assert(res == 0);
	assert(input.getIndex() == 0);

	input = StringToScanner("");
	res = ZeroOrMore<char>(make_shared<Symbol<char>>('a')).parse(input);
	assert(res == 0);
	assert(input.getIndex() == 0);
}

void byx::ParserCombinatorTest::OneOrMoreTest()
{
	Scanner<char> input = StringToScanner("aaaabb");
	int res = OneOrMore<char>(make_shared<Symbol<char>>('a')).parse(input);
	assert(res == 4);
	assert(input.getIndex() == 4);

	input = StringToScanner("aaaaaaa");
	res = OneOrMore<char>(make_shared<Symbol<char>>('a')).parse(input);
	assert(res == 7);
	assert(input.getIndex() == 7);

	input = StringToScanner("abbbbb");
	res = OneOrMore<char>(make_shared<Symbol<char>>('a')).parse(input);
	assert(res == 1);
	assert(input.getIndex() == 1);

	input = StringToScanner("a");
	res = OneOrMore<char>(make_shared<Symbol<char>>('a')).parse(input);
	assert(res == 1);
	assert(input.getIndex() == 1);

	input = StringToScanner("baaaaaaa");
	res = OneOrMore<char>(make_shared<Symbol<char>>('a')).parse(input);
	assert(res < 0);
	assert(input.getIndex() == 0);

	input = StringToScanner("b");
	res = OneOrMore<char>(make_shared<Symbol<char>>('a')).parse(input);
	assert(res < 0);
	assert(input.getIndex() == 0);

	input = StringToScanner("");
	res = OneOrMore<char>(make_shared<Symbol<char>>('a')).parse(input);
	assert(res < 0);
	assert(input.getIndex() == 0);
}

void byx::ParserCombinatorTest::ConcatTest()
{
	Scanner<char> input = StringToScanner("abcde");
	int res = Concat<char>(make_shared<Symbol<char>>('a'), make_shared<Symbol<char>>('b')).parse(input);
	assert(res == 2);
	assert(input.getIndex() == 2);

	input = StringToScanner("ab");
	res = Concat<char>(make_shared<Symbol<char>>('a'), make_shared<Symbol<char>>('b')).parse(input);
	assert(res == 2);
	assert(input.getIndex() == 2);


	input = StringToScanner("xycde");
	res = Concat<char>(make_shared<Symbol<char>>('a'), make_shared<Symbol<char>>('b')).parse(input);
	assert(res < 0);
	assert(input.getIndex() == 0);

	input = StringToScanner("acde");
	res = Concat<char>(make_shared<Symbol<char>>('a'), make_shared<Symbol<char>>('b')).parse(input);
	assert(res < 0);
	assert(input.getIndex() == 0);


	input = StringToScanner("a");
	res = Concat<char>(make_shared<Symbol<char>>('a'), make_shared<Symbol<char>>('b')).parse(input);
	assert(res < 0);
	assert(input.getIndex() == 0);

	input = StringToScanner("b");
	res = Concat<char>(make_shared<Symbol<char>>('a'), make_shared<Symbol<char>>('b')).parse(input);
	assert(res < 0);
	assert(input.getIndex() == 0);

	input = StringToScanner("x");
	res = Concat<char>(make_shared<Symbol<char>>('a'), make_shared<Symbol<char>>('b')).parse(input);
	assert(res < 0);
	assert(input.getIndex() == 0);

	input = StringToScanner("");
	res = Concat<char>(make_shared<Symbol<char>>('a'), make_shared<Symbol<char>>('b')).parse(input);
	assert(res < 0);
	assert(input.getIndex() == 0);
}

void byx::ParserCombinatorTest::ChooseTest()
{
	Scanner<char> input = StringToScanner("apple");
	int res = Choose<char>(make_shared<Symbol<char>>('a'), make_shared<Symbol<char>>('b')).parse(input);
	assert(res == 1);
	assert(input.getIndex() == 1);

	input = StringToScanner("banana");
	res = Choose<char>(make_shared<Symbol<char>>('a'), make_shared<Symbol<char>>('b')).parse(input);
	assert(res == 1);
	assert(input.getIndex() == 1);

	input = StringToScanner("cat");
	res = Choose<char>(make_shared<Symbol<char>>('a'), make_shared<Symbol<char>>('b')).parse(input);
	assert(res < 0);
	assert(input.getIndex() == 0);

	input = StringToScanner("d");
	res = Choose<char>(make_shared<Symbol<char>>('a'), make_shared<Symbol<char>>('b')).parse(input);
	assert(res < 0);
	assert(input.getIndex() == 0);

	input = StringToScanner("");
	res = Choose<char>(make_shared<Symbol<char>>('a'), make_shared<Symbol<char>>('b')).parse(input);
	assert(res < 0);
	assert(input.getIndex() == 0);

	// ÌØÊâ²âÊÔÓÃÀý
	input = StringToScanner("ac");
	res = Choose<char>(make_shared<Prefix>("ab"), make_shared<Prefix>("ac")).parse(input);
	assert(res == 2); 
	assert(input.getIndex() == 2);
}

void byx::ParserCombinatorTest::CallbackAfterSucceededTest()
{
	Scanner<char> input = StringToScanner("abcde");
	bool called = false;
	auto callback = [&called](const vector<char>& input, size_t begin, size_t end)
	{
		called = true;
	};
	int res = CallbackAfterSucceeded<char>(make_shared<Symbol<char>>('a'), callback).parse(input);
	assert(res == 1);
	assert(input.getIndex() == 1);
	assert(called);

	called = false;
	input = StringToScanner("xyz");
	res = CallbackAfterSucceeded<char>(make_shared<Symbol<char>>('a'), callback).parse(input);
	assert(res < 0);
	assert(input.getIndex() == 0);
	assert(!called);
}

void byx::ParserCombinatorTest::LazyTest()
{
	bool called = false;
	auto getParser = [&called]()
	{
		called = true;
		return make_shared<Symbol<char>>('a');
	};

	Scanner<char> input = StringToScanner("abcde");
	shared_ptr<Parser<char>> p = make_shared<Lazy<char>>(getParser);
	assert(!called);
	int res = p->parse(input);
	assert(called);
	assert(res == 1);
	assert(input.getIndex() == 1);
}

void byx::ParserCombinatorTest::RangeTest()
{
	Scanner<char> input = StringToScanner("apple");
	int res = Range('a', 'c').parse(input);
	assert(res == 1);
	assert(input.getIndex() == 1);

	input = StringToScanner("banana");
	res = Range('a', 'c').parse(input);
	assert(res == 1);
	assert(input.getIndex() == 1);

	input = StringToScanner("c");
	res = Range('a', 'c').parse(input);
	assert(res == 1);
	assert(input.getIndex() == 1);

	input = StringToScanner("delicious");
	res = Range('a', 'c').parse(input);
	assert(res < 0);
	assert(input.getIndex() == 0);

	input = StringToScanner("d");
	res = Range('a', 'c').parse(input);
	assert(res < 0);
	assert(input.getIndex() == 0);

	input = StringToScanner("");
	res = Range('a', 'c').parse(input);
	assert(res < 0);
	assert(input.getIndex() == 0);

	input = StringToScanner("apple");
	res = Range('c', 'a').parse(input);
	assert(res == 1);
	assert(input.getIndex() == 1);
	
	input = StringToScanner("banana");
	res = Range('c', 'a').parse(input);
	assert(res == 1);
	assert(input.getIndex() == 1);

	input = StringToScanner("c");
	res = Range('c', 'a').parse(input);
	assert(res == 1);
	assert(input.getIndex() == 1);

	input = StringToScanner("delicious");
	res = Range('c', 'a').parse(input);
	assert(res < 0);
	assert(input.getIndex() == 0);

	input = StringToScanner("d");
	res = Range('c', 'a').parse(input);
	assert(res < 0);
	assert(input.getIndex() == 0);

	input = StringToScanner("");
	res = Range('c', 'a').parse(input);
	assert(res < 0);
	assert(input.getIndex() == 0);

	input = StringToScanner("apple");
	res = Range('a', 'a').parse(input);
	assert(res == 1);
	assert(input.getIndex() == 1);

	input = StringToScanner("box");
	res = Range('a', 'a').parse(input);
	assert(res < 0);
	assert(input.getIndex() == 0);
}

void byx::ParserCombinatorTest::PrefixTest()
{
	Scanner<char> input = StringToScanner("int i = 0");
	int res = Prefix("int").parse(input);
	assert(res == 3);
	assert(input.getIndex() == 3);

	input = StringToScanner("int");
	res = Prefix("int").parse(input);
	assert(res == 3);
	assert(input.getIndex() == 3);

	input = StringToScanner("inout");
	res = Prefix("int").parse(input);
	assert(res < 0);
	assert(input.getIndex() == 0);

	input = StringToScanner("in");
	res = Prefix("int").parse(input);
	assert(res < 0);
	assert(input.getIndex() == 0);

	input = StringToScanner("");
	res = Prefix("int").parse(input);
	assert(res < 0);
	assert(input.getIndex() == 0);

	input = StringToScanner("abc");
	res = Prefix("a").parse(input);
	assert(res == 1);
	assert(input.getIndex() == 1);

	input = StringToScanner("a");
	res = Prefix("a").parse(input);
	assert(res == 1);
	assert(input.getIndex() == 1);

	input = StringToScanner("abc");
	res = Prefix("").parse(input);
	assert(res == 0);
	assert(input.getIndex() == 0);

	input = StringToScanner("");
	res = Prefix("").parse(input);
	assert(res == 0);
	assert(input.getIndex() == 0);
}

void byx::ParserCombinatorTest::SimpleLexerTest()
{
	vector<pair<string, bool>> tokens = SimpleLexer::Lex("  123456	56.75 \t8\n 0.23 	\n986\n 787\n 234.3432 \n\r 1.1");
	assert(tokens[0].first == "123456");
	assert(tokens[0].second);
	assert(tokens[1].first == "56.75");
	assert(!tokens[1].second);
	assert(tokens[2].first == "8");
	assert(tokens[2].second);
	assert(tokens[3].first == "0.23");
	assert(!tokens[3].second);
	assert(tokens[4].first == "986");
	assert(tokens[4].second);
	assert(tokens[5].first == "787");
	assert(tokens[5].second);
	assert(tokens[6].first == "234.3432");
	assert(!tokens[6].second);
	assert(tokens[7].first == "1.1");
	assert(!tokens[7].second);
}

void byx::ParserCombinatorTest::SimpleExprCalcTest()
{
	assert(SimpleExprCalc::Eval("1") == 1);
	assert(SimpleExprCalc::Eval("25") == 25);
	assert(SimpleExprCalc::Eval("1+2") == 3);
	assert(SimpleExprCalc::Eval("15-13") == 2);
	assert(SimpleExprCalc::Eval("(1+2)*(3+4)") == 21);
	assert(SimpleExprCalc::Eval("2+15*13") == 197);
	assert(SimpleExprCalc::Eval("25-(3+4*7/6*356)-22*(34+(54-6*7))+78/3") == -2388);
}

// SimpleLexer

Rule<char> ParserCombinatorTest::SimpleLexer::blank = Rule<char>::SymbolSet({ ' ', '\t', '\n', '\r' });
Rule<char> ParserCombinatorTest::SimpleLexer::dot = Rule<char>::Symbol('.');
Rule<char> ParserCombinatorTest::SimpleLexer::digit = Rule<char>(make_shared<Range>('0', '9'));
Rule<char> ParserCombinatorTest::SimpleLexer::integer = digit.oneOrMore().setCallback(AddInteger);
Rule<char> ParserCombinatorTest::SimpleLexer::decimal = (digit.oneOrMore() + dot + digit.oneOrMore()).setCallback(AddDecimal);
Rule<char> ParserCombinatorTest::SimpleLexer::rule = (blank | decimal | integer).oneOrMore();
vector<pair<string, bool>> ParserCombinatorTest::SimpleLexer::tokens;

std::vector<std::pair<std::string, bool>> byx::ParserCombinatorTest::SimpleLexer::Lex(const std::string& s)
{
	tokens.clear();
	rule.parse(vector<char>(s.begin(), s.end()));
	return tokens;
}

void byx::ParserCombinatorTest::SimpleLexer::AddInteger(const std::vector<char>& input, size_t begin, size_t end)
{
	string s = "";
	for (size_t i = begin; i < end; ++i)
	{
		s.push_back(input[i]);
	}
	tokens.push_back(pair<string, bool>(s, true));
}

void byx::ParserCombinatorTest::SimpleLexer::AddDecimal(const std::vector<char>& input, size_t begin, size_t end)
{
	string s = "";
	for (size_t i = begin; i < end; ++i)
	{
		s.push_back(input[i]);
	}
	tokens.push_back(pair<string, bool>(s, false));
}


// ExprCalc

Rule<char> ParserCombinatorTest::SimpleExprCalc::digit = Rule<char>(make_shared<Range>('0', '9'));
Rule<char> ParserCombinatorTest::SimpleExprCalc::integer = digit.oneOrMore().setCallback(PushInteger);
Rule<char> ParserCombinatorTest::SimpleExprCalc::add = Rule<char>::Symbol('+');
Rule<char> ParserCombinatorTest::SimpleExprCalc::sub = Rule<char>::Symbol('-');
Rule<char> ParserCombinatorTest::SimpleExprCalc::mul = Rule<char>::Symbol('*');
Rule<char> ParserCombinatorTest::SimpleExprCalc::div = Rule<char>::Symbol('/');
Rule<char> ParserCombinatorTest::SimpleExprCalc::lp = Rule<char>::Symbol('(');
Rule<char> ParserCombinatorTest::SimpleExprCalc::rp = Rule<char>::Symbol(')');
Rule<char> ParserCombinatorTest::SimpleExprCalc::lazyExpr = Rule<char>::Lazy(getExpr);
Rule<char> ParserCombinatorTest::SimpleExprCalc::factor = integer | lp + lazyExpr + rp;
Rule<char> ParserCombinatorTest::SimpleExprCalc::term = factor + ((mul | div) + factor).setCallback(Calculate).zeroOrMore();
Rule<char> ParserCombinatorTest::SimpleExprCalc::expr = term + ((add | sub) + term).setCallback(Calculate).zeroOrMore();
stack<int> ParserCombinatorTest::SimpleExprCalc::stack;

int byx::ParserCombinatorTest::SimpleExprCalc::Eval(const std::string& s)
{
	string t = "";
	for (size_t i = 0; i < s.size(); ++i)
	{
		if (s[i] != ' ')
		{
			t.push_back(s[i]);
		}
	}
	
	while (!stack.empty()) stack.pop();
	expr.parse(vector<char>(s.begin(), s.end()));
	return stack.top();
}

Rule<char> byx::ParserCombinatorTest::SimpleExprCalc::getExpr()
{
	return expr;
}

void byx::ParserCombinatorTest::SimpleExprCalc::PushInteger(const std::vector<char>& input, size_t begin, size_t end)
{
	int val = 0;
	for (size_t i = begin; i < end; ++i)
	{
		val = val * 10 + (input[i] - '0');
	}
	stack.push(val);
}

void byx::ParserCombinatorTest::SimpleExprCalc::Calculate(const std::vector<char>& input, size_t begin, size_t end)
{
	int rhs = stack.top();
	stack.pop();
	int lhs = stack.top();
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