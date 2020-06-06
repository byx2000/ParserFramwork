#include "CLexerTest.h"
#include "../Samples/CLexer/CLexer.h"

#include <cassert>

using namespace std;
using namespace byx;

using TokenType = CLexer::TokenType;
using Token = CLexer::Token;

#define assert_exception(expr)	\
do								\
{								\
    bool flag = false;			\
    try							\
    {							\
        expr;					\
    }							\
    catch (...)					\
    {							\
        flag = true;				\
    }							\
    assert(flag);				\
}								\
while (0)						\

// CLexerTest

void byx::CLexerTest::Run()
{
	IntegerTest();
	DecimalTest();
	IntegerDecimalTest();
	IdentifierTest();
	IntegerDecimalIdentTest();
	KeywordTest();
	StringTest();
	CharTest();
	LineComment();
	BlockComment();
	AddTest();
	SubTest();
	MulTest();
	DivTest();
	RemTest();
	EqualTest();
	LogicAndTest();
	LogicOrTest();
	BitXorTest();
	LogicNotTest();
	LessTest();
	LargerTest();
	BracketTest();
	OtherTest();

	PreprocessTest();
	ArithExprTest();
	BitExprTest();
	CmpExprTest();
	DeclareStmtTest();
}

void byx::CLexerTest::IntegerTest()
{
	vector<Token> tokens = CLexer::Lex(" 0 123 4 567 8	 90		");
	assert(tokens.size() == 6);
	assert(tokens[0] == Token(TokenType::Integer, "0"));
	assert(tokens[1] == Token(TokenType::Integer, "123"));
	assert(tokens[2] == Token(TokenType::Integer, "4"));
	assert(tokens[3] == Token(TokenType::Integer, "567"));
	assert(tokens[4] == Token(TokenType::Integer, "8"));
	assert(tokens[5] == Token(TokenType::Integer, "90"));
}

void byx::CLexerTest::DecimalTest()
{
	vector<Token> tokens = CLexer::Lex("	0.23 7.4\r 6.88	\n	312.113	 \t47663.376278	");
	assert(tokens.size() == 5);
	assert(tokens[0] == Token(TokenType::Decimal, "0.23"));
	assert(tokens[1] == Token(TokenType::Decimal, "7.4"));
	assert(tokens[2] == Token(TokenType::Decimal, "6.88"));
	assert(tokens[3] == Token(TokenType::Decimal, "312.113"));
	assert(tokens[4] == Token(TokenType::Decimal, "47663.376278"));
}

void byx::CLexerTest::IntegerDecimalTest()
{
	vector<Token> tokens = CLexer::Lex("12.56 33		6 0.4 88862   7623.322 ");
	assert(tokens.size() == 6);
	assert(tokens[0] == Token(TokenType::Decimal, "12.56"));
	assert(tokens[1] == Token(TokenType::Integer, "33"));
	assert(tokens[2] == Token(TokenType::Integer, "6"));
	assert(tokens[3] == Token(TokenType::Decimal, "0.4"));
	assert(tokens[4] == Token(TokenType::Integer, "88862"));
	assert(tokens[5] == Token(TokenType::Decimal, "7623.322"));
}

void byx::CLexerTest::IdentifierTest()
{
	vector<Token> tokens = CLexer::Lex("i j k	cnt sum res		_printf ____ _32as __asdca34__2343c  __asssd  ____235435344    weiugdbjbcdkagu238jvbdjasvg237i4572vjh7hg8");
	assert(tokens.size() == 13);
	assert(tokens[0] == Token(TokenType::Identifier, "i"));
	assert(tokens[1] == Token(TokenType::Identifier, "j"));
	assert(tokens[2] == Token(TokenType::Identifier, "k"));
	assert(tokens[3] == Token(TokenType::Identifier, "cnt"));
	assert(tokens[4] == Token(TokenType::Identifier, "sum"));
	assert(tokens[5] == Token(TokenType::Identifier, "res"));
	assert(tokens[6] == Token(TokenType::Identifier, "_printf"));
	assert(tokens[7] == Token(TokenType::Identifier, "____"));
	assert(tokens[8] == Token(TokenType::Identifier, "_32as"));
	assert(tokens[9] == Token(TokenType::Identifier, "__asdca34__2343c"));
	assert(tokens[10] == Token(TokenType::Identifier, "__asssd"));
	assert(tokens[11] == Token(TokenType::Identifier, "____235435344"));
	assert(tokens[12] == Token(TokenType::Identifier, "weiugdbjbcdkagu238jvbdjasvg237i4572vjh7hg8"));
}

void byx::CLexerTest::IntegerDecimalIdentTest()
{
	vector<Token> tokens = CLexer::Lex("23 45.76 sum 3.5 7 a 88");
	assert(tokens.size() == 7);
	assert(tokens[0] == Token(TokenType::Integer, "23"));
	assert(tokens[1] == Token(TokenType::Decimal, "45.76"));
	assert(tokens[2] == Token(TokenType::Identifier, "sum"));
	assert(tokens[3] == Token(TokenType::Decimal, "3.5"));
	assert(tokens[4] == Token(TokenType::Integer, "7"));
	assert(tokens[5] == Token(TokenType::Identifier, "a"));
	assert(tokens[6] == Token(TokenType::Integer, "88"));
}

void byx::CLexerTest::KeywordTest()
{
	vector<Token> tokens = CLexer::Lex("int double char const sum struct");
	assert(tokens.size() == 6);
	assert(tokens[0] == Token(TokenType::Keyword, "int"));
	assert(tokens[1] == Token(TokenType::Keyword, "double"));
	assert(tokens[2] == Token(TokenType::Keyword, "char"));
	assert(tokens[3] == Token(TokenType::Keyword, "const"));
	assert(tokens[4] == Token(TokenType::Identifier, "sum"));
	assert(tokens[5] == Token(TokenType::Keyword, "struct"));
}

void byx::CLexerTest::StringTest()
{
	vector<Token> tokens = CLexer::Lex("\"This is a string\"  \"\"");
	assert(tokens.size() == 2);
	assert(tokens[0] == Token(TokenType::String, "This is a string"));
	assert(tokens[1] == Token(TokenType::String, ""));
}

void byx::CLexerTest::CharTest()
{
	vector<Token> tokens = CLexer::Lex("'a' 'b''c'");
	assert(tokens.size() == 3);
	assert(tokens[0] == Token(TokenType::Char, "a"));
	assert(tokens[1] == Token(TokenType::Char, "b"));
	assert(tokens[2] == Token(TokenType::Char, "c"));
}

void byx::CLexerTest::LineComment()
{
	vector<Token> tokens = CLexer::Lex("123 3.5 sum // This is line comment \n321 5.3 mus//this//is//anothercomment");
	assert(tokens.size() == 6);
	assert(tokens[0] == Token(TokenType::Integer, "123"));
	assert(tokens[1] == Token(TokenType::Decimal, "3.5"));
	assert(tokens[2] == Token(TokenType::Identifier, "sum"));
	assert(tokens[3] == Token(TokenType::Integer, "321"));
	assert(tokens[4] == Token(TokenType::Decimal, "5.3"));
	assert(tokens[5] == Token(TokenType::Identifier, "mus"));
}

void byx::CLexerTest::BlockComment()
{
	vector<Token> tokens = CLexer::Lex("123 3.5 sum /*This is a comment*/321 5.3 mus /*//\\\\///*\\\\*/ i j k /*dsjgvsudkvbshjb*/");
	assert(tokens.size() == 9);
	assert(tokens[0] == Token(TokenType::Integer, "123"));
	assert(tokens[1] == Token(TokenType::Decimal, "3.5"));
	assert(tokens[2] == Token(TokenType::Identifier, "sum"));
	assert(tokens[3] == Token(TokenType::Integer, "321"));
	assert(tokens[4] == Token(TokenType::Decimal, "5.3"));
	assert(tokens[5] == Token(TokenType::Identifier, "mus"));
	assert(tokens[6] == Token(TokenType::Identifier, "i"));
	assert(tokens[7] == Token(TokenType::Identifier, "j"));
	assert(tokens[8] == Token(TokenType::Identifier, "k"));
}

void byx::CLexerTest::AddTest()
{
	vector<Token> tokens = CLexer::Lex("+ ++ += ++ + += ++++ +++++ +=++");
	assert(tokens.size() == 13);
	assert(tokens[0] == Token(TokenType::Add, "+"));
	assert(tokens[1] == Token(TokenType::Inc, "++"));
	assert(tokens[2] == Token(TokenType::AddAssign, "+="));
	assert(tokens[3] == Token(TokenType::Inc, "++"));
	assert(tokens[4] == Token(TokenType::Add, "+"));
	assert(tokens[5] == Token(TokenType::AddAssign, "+="));
	assert(tokens[6] == Token(TokenType::Inc, "++"));
	assert(tokens[7] == Token(TokenType::Inc, "++"));
	assert(tokens[8] == Token(TokenType::Inc, "++"));
	assert(tokens[9] == Token(TokenType::Inc, "++"));
	assert(tokens[10] == Token(TokenType::Add, "+"));
	assert(tokens[11] == Token(TokenType::AddAssign, "+="));
	assert(tokens[12] == Token(TokenType::Inc, "++"));
}

void byx::CLexerTest::SubTest()
{
	vector<Token> tokens = CLexer::Lex("- -- -= -- - -= ---- ----- -=--->");
	assert(tokens.size() == 14);
	assert(tokens[0] == Token(TokenType::Sub, "-"));
	assert(tokens[1] == Token(TokenType::Dec, "--"));
	assert(tokens[2] == Token(TokenType::SubAssign, "-="));
	assert(tokens[3] == Token(TokenType::Dec, "--"));
	assert(tokens[4] == Token(TokenType::Sub, "-"));
	assert(tokens[5] == Token(TokenType::SubAssign, "-="));
	assert(tokens[6] == Token(TokenType::Dec, "--"));
	assert(tokens[7] == Token(TokenType::Dec, "--"));
	assert(tokens[8] == Token(TokenType::Dec, "--"));
	assert(tokens[9] == Token(TokenType::Dec, "--"));
	assert(tokens[10] == Token(TokenType::Sub, "-"));
	assert(tokens[11] == Token(TokenType::SubAssign, "-="));
	assert(tokens[12] == Token(TokenType::Dec, "--"));
	assert(tokens[13] == Token(TokenType::PointerMember, "->"));
}

void byx::CLexerTest::MulTest()
{
	vector<Token> tokens = CLexer::Lex("* *= *= * * *=*=* ****");
	assert(tokens.size() == 12);
	assert(tokens[0] == Token(TokenType::Mul, "*"));
	assert(tokens[1] == Token(TokenType::MulAssign, "*="));
	assert(tokens[2] == Token(TokenType::MulAssign, "*="));
	assert(tokens[3] == Token(TokenType::Mul, "*"));
	assert(tokens[4] == Token(TokenType::Mul, "*"));
	assert(tokens[5] == Token(TokenType::MulAssign, "*="));
	assert(tokens[6] == Token(TokenType::MulAssign, "*="));
	assert(tokens[7] == Token(TokenType::Mul, "*"));
	assert(tokens[8] == Token(TokenType::Mul, "*"));
	assert(tokens[9] == Token(TokenType::Mul, "*"));
	assert(tokens[10] == Token(TokenType::Mul, "*"));
	assert(tokens[11] == Token(TokenType::Mul, "*"));
}

void byx::CLexerTest::DivTest()
{
	vector<Token> tokens = CLexer::Lex("/ /= /= / / /=/=/");
	assert(tokens.size() == 8);
	assert(tokens[0] == Token(TokenType::Div, "/"));
	assert(tokens[1] == Token(TokenType::DivAssign, "/="));
	assert(tokens[2] == Token(TokenType::DivAssign, "/="));
	assert(tokens[3] == Token(TokenType::Div, "/"));
	assert(tokens[4] == Token(TokenType::Div, "/"));
	assert(tokens[5] == Token(TokenType::DivAssign, "/="));
	assert(tokens[6] == Token(TokenType::DivAssign, "/="));
	assert(tokens[7] == Token(TokenType::Div, "/"));
}

void byx::CLexerTest::RemTest()
{
	vector<Token> tokens = CLexer::Lex("% %= %=	% %=%%=");
	assert(tokens.size() == 7);
	assert(tokens[0] == Token(TokenType::Rem, "%"));
	assert(tokens[1] == Token(TokenType::RemAssign, "%="));
	assert(tokens[2] == Token(TokenType::RemAssign, "%="));
	assert(tokens[3] == Token(TokenType::Rem, "%"));
	assert(tokens[4] == Token(TokenType::RemAssign, "%="));
	assert(tokens[5] == Token(TokenType::Rem, "%"));
	assert(tokens[6] == Token(TokenType::RemAssign, "%="));
}

void byx::CLexerTest::EqualTest()
{
	vector<Token> tokens = CLexer::Lex("= == == = =======");
	assert(tokens.size() == 8);
	assert(tokens[0] == Token(TokenType::Assign, "="));
	assert(tokens[1] == Token(TokenType::Equal, "=="));
	assert(tokens[2] == Token(TokenType::Equal, "=="));
	assert(tokens[3] == Token(TokenType::Assign, "="));
	assert(tokens[4] == Token(TokenType::Equal, "=="));
	assert(tokens[5] == Token(TokenType::Equal, "=="));
	assert(tokens[6] == Token(TokenType::Equal, "=="));
	assert(tokens[7] == Token(TokenType::Assign, "="));
}

void byx::CLexerTest::LogicAndTest()
{
	vector<Token> tokens = CLexer::Lex("& && &= && & & && &&&&& &=&=&&=");
	assert(tokens.size() == 14);
	assert(tokens[0] == Token(TokenType::BitAnd, "&"));
	assert(tokens[1] == Token(TokenType::LogicAnd, "&&"));
	assert(tokens[2] == Token(TokenType::BitAndAssign, "&="));
	assert(tokens[3] == Token(TokenType::LogicAnd, "&&"));
	assert(tokens[4] == Token(TokenType::BitAnd, "&"));
	assert(tokens[5] == Token(TokenType::BitAnd, "&"));
	assert(tokens[6] == Token(TokenType::LogicAnd, "&&"));
	assert(tokens[7] == Token(TokenType::LogicAnd, "&&"));
	assert(tokens[8] == Token(TokenType::LogicAnd, "&&"));
	assert(tokens[9] == Token(TokenType::BitAnd, "&"));
	assert(tokens[10] == Token(TokenType::BitAndAssign, "&="));
	assert(tokens[11] == Token(TokenType::BitAndAssign, "&="));
	assert(tokens[12] == Token(TokenType::LogicAnd, "&&"));
	assert(tokens[13] == Token(TokenType::Assign, "="));
}

void byx::CLexerTest::LogicOrTest()
{
	vector<Token> tokens = CLexer::Lex("| || |= || | | || ||||| |=|=||=");
	assert(tokens.size() == 14);
	assert(tokens[0] == Token(TokenType::BitOr, "|"));
	assert(tokens[1] == Token(TokenType::LogicOr, "||"));
	assert(tokens[2] == Token(TokenType::BitOrAssign, "|="));
	assert(tokens[3] == Token(TokenType::LogicOr, "||"));
	assert(tokens[4] == Token(TokenType::BitOr, "|"));
	assert(tokens[5] == Token(TokenType::BitOr, "|"));
	assert(tokens[6] == Token(TokenType::LogicOr, "||"));
	assert(tokens[7] == Token(TokenType::LogicOr, "||"));
	assert(tokens[8] == Token(TokenType::LogicOr, "||"));
	assert(tokens[9] == Token(TokenType::BitOr, "|"));
	assert(tokens[10] == Token(TokenType::BitOrAssign, "|="));
	assert(tokens[11] == Token(TokenType::BitOrAssign, "|="));
	assert(tokens[12] == Token(TokenType::LogicOr, "||"));
	assert(tokens[13] == Token(TokenType::Assign, "="));
}

void byx::CLexerTest::BitXorTest()
{
	vector<Token> tokens = CLexer::Lex("^ ^= ^^ ^=^^=");
	assert(tokens.size() == 7);
	assert(tokens[0] == Token(TokenType::BitXor, "^"));
	assert(tokens[1] == Token(TokenType::BitXorAssign, "^="));
	assert(tokens[2] == Token(TokenType::BitXor, "^"));
	assert(tokens[3] == Token(TokenType::BitXor, "^"));
	assert(tokens[4] == Token(TokenType::BitXorAssign, "^="));
	assert(tokens[5] == Token(TokenType::BitXor, "^"));
	assert(tokens[6] == Token(TokenType::BitXorAssign, "^="));
}

void byx::CLexerTest::LogicNotTest()
{
	vector<Token> tokens = CLexer::Lex("! != !!! !=!==");
	assert(tokens.size() == 8);
	assert(tokens[0] == Token(TokenType::LogicNot, "!"));
	assert(tokens[1] == Token(TokenType::NotEqual, "!="));
	assert(tokens[2] == Token(TokenType::LogicNot, "!"));
	assert(tokens[3] == Token(TokenType::LogicNot, "!"));
	assert(tokens[4] == Token(TokenType::LogicNot, "!"));
	assert(tokens[5] == Token(TokenType::NotEqual, "!="));
	assert(tokens[6] == Token(TokenType::NotEqual, "!="));
	assert(tokens[7] == Token(TokenType::Assign, "="));
}

void byx::CLexerTest::LessTest()
{
	vector<Token> tokens = CLexer::Lex("< <= << <<= <<<<=<< <==<=");
	assert(tokens.size() == 10);
	assert(tokens[0] == Token(TokenType::Less, "<"));
	assert(tokens[1] == Token(TokenType::LessEqual, "<="));
	assert(tokens[2] == Token(TokenType::LeftShift, "<<"));
	assert(tokens[3] == Token(TokenType::LeftShiftAssign, "<<="));
	assert(tokens[4] == Token(TokenType::LeftShift, "<<"));
	assert(tokens[5] == Token(TokenType::LeftShiftAssign, "<<="));
	assert(tokens[6] == Token(TokenType::LeftShift, "<<"));
	assert(tokens[7] == Token(TokenType::LessEqual, "<="));
	assert(tokens[8] == Token(TokenType::Assign, "="));
	assert(tokens[9] == Token(TokenType::LessEqual, "<="));
}

void byx::CLexerTest::LargerTest()
{
	vector<Token> tokens = CLexer::Lex("> >= >> >>= >>>>=>> >==>=");
	assert(tokens.size() == 10);
	assert(tokens[0] == Token(TokenType::Larger, ">"));
	assert(tokens[1] == Token(TokenType::LargerEqual, ">="));
	assert(tokens[2] == Token(TokenType::RightShift, ">>"));
	assert(tokens[3] == Token(TokenType::RightShiftAssign, ">>="));
	assert(tokens[4] == Token(TokenType::RightShift, ">>"));
	assert(tokens[5] == Token(TokenType::RightShiftAssign, ">>="));
	assert(tokens[6] == Token(TokenType::RightShift, ">>"));
	assert(tokens[7] == Token(TokenType::LargerEqual, ">="));
	assert(tokens[8] == Token(TokenType::Assign, "="));
	assert(tokens[9] == Token(TokenType::LargerEqual, ">="));
}

void byx::CLexerTest::BracketTest()
{
	vector<Token> tokens = CLexer::Lex("()[]{}( {]{)}]");
	assert(tokens.size() == 13);
	assert(tokens[0] == Token(TokenType::OpenParenthese, "("));
	assert(tokens[1] == Token(TokenType::CloseParenthese, ")"));
	assert(tokens[2] == Token(TokenType::OpenSquareBracket, "["));
	assert(tokens[3] == Token(TokenType::CloseSquareBracket, "]"));
	assert(tokens[4] == Token(TokenType::OpenBrace, "{"));
	assert(tokens[5] == Token(TokenType::CloseBrace, "}"));
	assert(tokens[6] == Token(TokenType::OpenParenthese, "("));
	assert(tokens[7] == Token(TokenType::OpenBrace, "{"));
	assert(tokens[8] == Token(TokenType::CloseSquareBracket, "]"));
	assert(tokens[9] == Token(TokenType::OpenBrace, "{"));
	assert(tokens[10] == Token(TokenType::CloseParenthese, ")"));
	assert(tokens[11] == Token(TokenType::CloseBrace, "}"));
	assert(tokens[12] == Token(TokenType::CloseSquareBracket, "]"));
}

void byx::CLexerTest::OtherTest()
{
	vector<Token> tokens = CLexer::Lex(",,.;,,;.. ;,,;?");
	assert(tokens.size() == 14);
	assert(tokens[0] == Token(TokenType::Comma, ","));
	assert(tokens[1] == Token(TokenType::Comma, ","));
	assert(tokens[2] == Token(TokenType::Member, "."));
	assert(tokens[3] == Token(TokenType::Semicolon, ";"));
	assert(tokens[4] == Token(TokenType::Comma, ","));
	assert(tokens[5] == Token(TokenType::Comma, ","));
	assert(tokens[6] == Token(TokenType::Semicolon, ";"));
	assert(tokens[7] == Token(TokenType::Member, "."));
	assert(tokens[8] == Token(TokenType::Member, "."));
	assert(tokens[9] == Token(TokenType::Semicolon, ";"));
	assert(tokens[10] == Token(TokenType::Comma, ","));
	assert(tokens[11] == Token(TokenType::Comma, ","));
	assert(tokens[12] == Token(TokenType::Semicolon, ";"));
	assert(tokens[13] == Token(TokenType::Question, "?"));
}

void byx::CLexerTest::PreprocessTest()
{
	vector<Token> tokens = CLexer::Lex("#include <stdio.h>\n #define PI 3.14\n#if");
	assert(tokens.size() == 3);
	assert(tokens[0] == Token(TokenType::Preprocess, "#include <stdio.h>"));
	assert(tokens[1] == Token(TokenType::Preprocess, "#define PI 3.14"));
	assert(tokens[2] == Token(TokenType::Preprocess, "#if"));
}

void byx::CLexerTest::ArithExprTest()
{
	vector<Token> tokens = CLexer::Lex("1+2.34-5.6+(-367)*71.52/0.24%32");
	assert(tokens.size() == 16);
	assert(tokens[0] == Token(TokenType::Integer, "1"));
	assert(tokens[1] == Token(TokenType::Add, "+"));
	assert(tokens[2] == Token(TokenType::Decimal, "2.34"));
	assert(tokens[3] == Token(TokenType::Sub, "-"));
	assert(tokens[4] == Token(TokenType::Decimal, "5.6"));
	assert(tokens[5] == Token(TokenType::Add, "+"));
	assert(tokens[6] == Token(TokenType::OpenParenthese, "("));
	assert(tokens[7] == Token(TokenType::Sub, "-"));
	assert(tokens[8] == Token(TokenType::Integer, "367"));
	assert(tokens[9] == Token(TokenType::CloseParenthese, ")"));
	assert(tokens[10] == Token(TokenType::Mul, "*"));
	assert(tokens[11] == Token(TokenType::Decimal, "71.52"));
	assert(tokens[12] == Token(TokenType::Div, "/"));
	assert(tokens[13] == Token(TokenType::Decimal, "0.24"));
	assert(tokens[14] == Token(TokenType::Rem, "%"));
	assert(tokens[15] == Token(TokenType::Integer, "32"));
}

void byx::CLexerTest::BitExprTest()
{
	vector<Token> tokens = CLexer::Lex("1&21^(23|54) | ~(234^(432&0))");
	assert(tokens.size() == 20);
	assert(tokens[0] == Token(TokenType::Integer, "1"));
	assert(tokens[1] == Token(TokenType::BitAnd, "&"));
	assert(tokens[2] == Token(TokenType::Integer, "21"));
	assert(tokens[3] == Token(TokenType::BitXor, "^"));
	assert(tokens[4] == Token(TokenType::OpenParenthese, "("));
	assert(tokens[5] == Token(TokenType::Integer, "23"));
	assert(tokens[6] == Token(TokenType::BitOr, "|"));
	assert(tokens[7] == Token(TokenType::Integer, "54"));
	assert(tokens[8] == Token(TokenType::CloseParenthese, ")"));
	assert(tokens[9] == Token(TokenType::BitOr, "|"));
	assert(tokens[10] == Token(TokenType::BitReverse, "~"));
	assert(tokens[11] == Token(TokenType::OpenParenthese, "("));
	assert(tokens[12] == Token(TokenType::Integer, "234"));
	assert(tokens[13] == Token(TokenType::BitXor, "^"));
	assert(tokens[14] == Token(TokenType::OpenParenthese, "("));
	assert(tokens[15] == Token(TokenType::Integer, "432"));
	assert(tokens[16] == Token(TokenType::BitAnd, "&"));
	assert(tokens[17] == Token(TokenType::Integer, "0"));
	assert(tokens[18] == Token(TokenType::CloseParenthese, ")"));
	assert(tokens[19] == Token(TokenType::CloseParenthese, ")"));
}

void byx::CLexerTest::CmpExprTest()
{
	vector<Token> tokens = CLexer::Lex("(3.14 > 54) && 6213<4432 || !(5.6 >= 231<=47.34) == 0");
	assert(tokens.size() == 20);
	assert(tokens[0] == Token(TokenType::OpenParenthese, "("));
	assert(tokens[1] == Token(TokenType::Decimal, "3.14"));
	assert(tokens[2] == Token(TokenType::Larger, ">"));
	assert(tokens[3] == Token(TokenType::Integer, "54"));
	assert(tokens[4] == Token(TokenType::CloseParenthese, ")"));
	assert(tokens[5] == Token(TokenType::LogicAnd, "&&"));
	assert(tokens[6] == Token(TokenType::Integer, "6213"));
	assert(tokens[7] == Token(TokenType::Less, "<"));
	assert(tokens[8] == Token(TokenType::Integer, "4432"));
	assert(tokens[9] == Token(TokenType::LogicOr, "||"));
	assert(tokens[10] == Token(TokenType::LogicNot, "!"));
	assert(tokens[11] == Token(TokenType::OpenParenthese, "("));
	assert(tokens[12] == Token(TokenType::Decimal, "5.6"));
	assert(tokens[13] == Token(TokenType::LargerEqual, ">="));
	assert(tokens[14] == Token(TokenType::Integer, "231"));
	assert(tokens[15] == Token(TokenType::LessEqual, "<="));
	assert(tokens[16] == Token(TokenType::Decimal, "47.34"));
	assert(tokens[17] == Token(TokenType::CloseParenthese, ")"));
	assert(tokens[18] == Token(TokenType::Equal, "=="));
	assert(tokens[19] == Token(TokenType::Integer, "0"));
}

void byx::CLexerTest::DeclareStmtTest()
{
	vector<Token> tokens = CLexer::Lex("int i = 100;");
	assert(tokens.size() == 5);
	assert(tokens[0] == Token(TokenType::Keyword, "int"));
	assert(tokens[1] == Token(TokenType::Identifier, "i"));
	assert(tokens[2] == Token(TokenType::Assign, "="));
	assert(tokens[3] == Token(TokenType::Integer, "100"));
	assert(tokens[4] == Token(TokenType::Semicolon, ";"));

	tokens = CLexer::Lex("double sum = 3.14 * 2;");
	assert(tokens.size() == 7);
	assert(tokens[0] == Token(TokenType::Keyword, "double"));
	assert(tokens[1] == Token(TokenType::Identifier, "sum"));
	assert(tokens[2] == Token(TokenType::Assign, "="));
	assert(tokens[3] == Token(TokenType::Decimal, "3.14"));
	assert(tokens[4] == Token(TokenType::Mul, "*"));
	assert(tokens[5] == Token(TokenType::Integer, "2"));
	assert(tokens[6] == Token(TokenType::Semicolon, ";"));

	tokens = CLexer::Lex("char ch = 'a';");
	assert(tokens.size() == 5);
	assert(tokens[0] == Token(TokenType::Keyword, "char"));
	assert(tokens[1] == Token(TokenType::Identifier, "ch"));
	assert(tokens[2] == Token(TokenType::Assign, "="));
	assert(tokens[3] == Token(TokenType::Char, "a"));
	assert(tokens[4] == Token(TokenType::Semicolon, ";"));

	tokens = CLexer::Lex("const char* str = \"hello!\";");
	assert(tokens.size() == 7);
	assert(tokens[0] == Token(TokenType::Keyword, "const"));
	assert(tokens[1] == Token(TokenType::Keyword, "char"));
	assert(tokens[2] == Token(TokenType::Mul, "*"));
	assert(tokens[3] == Token(TokenType::Identifier, "str"));
	assert(tokens[4] == Token(TokenType::Assign, "="));
	assert(tokens[5] == Token(TokenType::String, "hello!"));
	assert(tokens[6] == Token(TokenType::Semicolon, ";"));
}
