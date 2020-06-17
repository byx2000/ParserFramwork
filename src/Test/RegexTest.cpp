#include "RegexTest.h"
#include "../Samples/Regex/Regex.h"

#include <cassert>

using namespace std;
using namespace byx;

void byx::RegexTest::Run()
{
	assert(Regex::Match("a", "a"));
	assert(!Regex::Match("a", "b"));
	assert(!Regex::Match("a", "ab"));
	assert(!Regex::Match("a", "xy"));
	assert(!Regex::Match("a", ""));

	assert(Regex::Match("ab", "ab"));
	assert(!Regex::Match("ab", "ax"));
	assert(!Regex::Match("ab", "xb"));
	assert(!Regex::Match("ab", "a"));
	assert(!Regex::Match("ab", "b"));
	assert(!Regex::Match("ab", "abc"));
	assert(!Regex::Match("ab", ""));

	assert(Regex::Match("abc", "abc"));
	assert(!Regex::Match("abc", "abx"));
	assert(!Regex::Match("abc", "axc"));
	assert(!Regex::Match("abc", "xyz"));
	assert(!Regex::Match("abc", "a"));
	assert(!Regex::Match("abc", "b"));
	assert(!Regex::Match("abc", "ab"));
	assert(!Regex::Match("abc", ""));

	assert(Regex::Match("a|b", "a"));
	assert(Regex::Match("a|b", "b"));
	assert(!Regex::Match("a|b", "c"));
	assert(!Regex::Match("a|b", "ab"));
	assert(!Regex::Match("a|b", "ba"));
	assert(!Regex::Match("a|b", "aa"));
	assert(!Regex::Match("a|b", "bb"));
	assert(!Regex::Match("a|b", ""));

	assert(Regex::Match("a|b|c", "a"));
	assert(Regex::Match("a|b|c", "b"));
	assert(Regex::Match("a|b|c", "c"));
	assert(!Regex::Match("a|b|c", "x"));
	assert(!Regex::Match("a|b|c", "ab"));
	assert(!Regex::Match("a|b|c", "bc"));
	assert(!Regex::Match("a|b|c", "ac"));
	assert(!Regex::Match("a|b|c", "abc"));
	assert(!Regex::Match("a|b|c", ""));

	assert(Regex::Match("ab|c", "ab"));
	assert(Regex::Match("ab|c", "c"));
	assert(!Regex::Match("ab|c", "ac"));
	assert(!Regex::Match("ab|c", "bc"));
	assert(!Regex::Match("ab|c", "abc"));
	assert(!Regex::Match("ab|c", "a"));
	assert(!Regex::Match("ab|c", "b"));
	assert(!Regex::Match("ab|c", "ax"));
	assert(!Regex::Match("ab|c", ""));

	assert(Regex::Match("a(b|c)", "ab"));
	assert(Regex::Match("a(b|c)", "ac"));
	assert(!Regex::Match("a(b|c)", "bc"));
	assert(!Regex::Match("a(b|c)", "a"));
	assert(!Regex::Match("a(b|c)", "b"));
	assert(!Regex::Match("a(b|c)", "abc"));
	assert(!Regex::Match("a(b|c)", "ax"));
	assert(!Regex::Match("a(b|c)", ""));

	assert(Regex::Match("a|bc|c", "a"));
	assert(Regex::Match("a|bc|c", "bc"));
	assert(Regex::Match("a|bc|c", "c"));
	assert(!Regex::Match("a|bc|c", "d"));
	assert(!Regex::Match("a|bc|c", "ab"));
	assert(!Regex::Match("a|bc|c", "ac"));
	assert(!Regex::Match("a|bc|c", "abc"));
	assert(!Regex::Match("a|bc|c", ""));

	assert(Regex::Match("(a|b)(c|d)", "ac"));
	assert(Regex::Match("(a|b)(c|d)", "ad"));
	assert(Regex::Match("(a|b)(c|d)", "bc"));
	assert(Regex::Match("(a|b)(c|d)", "bd"));
	assert(!Regex::Match("(a|b)(c|d)", "ab"));
	assert(!Regex::Match("(a|b)(c|d)", "cd"));
	assert(!Regex::Match("(a|b)(c|d)", "acc"));
	assert(!Regex::Match("(a|b)(c|d)", ""));

	assert(Regex::Match("a*", ""));
	assert(Regex::Match("a*", "a"));
	assert(Regex::Match("a*", "aaaaaaa"));
	assert(!Regex::Match("a*", "b"));
	assert(!Regex::Match("a*", "baaa"));

	assert(!Regex::Match("a+", ""));
	assert(Regex::Match("a+", "a"));
	assert(Regex::Match("a+", "aaaaaaa"));
	assert(!Regex::Match("a+", "b"));
	assert(!Regex::Match("a+", "baaa"));

	assert(Regex::Match("(ab)*", ""));
	assert(Regex::Match("(ab)*", "ab"));
	assert(Regex::Match("(ab)*", "ababab"));
	assert(!Regex::Match("(ab)*", "ababa"));
	assert(!Regex::Match("(ab)*", "bababab"));

	assert(Regex::Match(".", "a"));
	assert(Regex::Match(".", "b"));
	assert(Regex::Match(".", "c"));
	assert(!Regex::Match(".", ""));

	assert(Regex::Match(".*", ""));
	assert(Regex::Match(".*", "f"));
	assert(Regex::Match(".*", "efvjkkwhvbeuvc"));

	assert(Regex::Match("a*b*", ""));
	assert(Regex::Match("a*b*", "a"));
	assert(Regex::Match("a*b*", "b"));
	assert(Regex::Match("a*b*", "aaaaa"));
	assert(Regex::Match("a*b*", "bbbbb"));
	assert(Regex::Match("a*b*", "ab"));
	assert(Regex::Match("a*b*", "aaabbbbb"));

	assert(Regex::Match("\\\\", "\\"));
	assert(Regex::Match("\\*", "*"));
	assert(Regex::Match("\\+", "+"));
	assert(Regex::Match("\\.", "."));
	assert(Regex::Match("\\(", "("));
	assert(Regex::Match("\\)", ")"));
	assert(Regex::Match("\\|", "|"));

	assert(Regex::Match("1((10*1)|(01*0))*10*", "101110001"));
	assert(!Regex::Match("1((10*1)|(01*0))*10*", "101110010"));
	assert(!Regex::Match("1((10*1)|(01*0))*10*", "101110011"));
	assert(Regex::Match("1((10*1)|(01*0))*10*", "101110100"));
}
