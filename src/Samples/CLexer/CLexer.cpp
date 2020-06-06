#include "CLexer.h"

using namespace std;
using namespace byx;

// Token

CLexer::Token::Token() : type(TokenType::Unknown), value("") {}

CLexer::Token::Token(TokenType type, const std::string& val) : type(type), value(val) {}

bool CLexer::Token::operator==(const Token& token) const
{
    return type == token.type && value == token.value;
}

// CLexer

// 空白符
Re CLexer::blank = Re::SymbolSet({ ' ', '\t', '\r', '\n' });

// 单个数字
Re CLexer::digit = Re(make_shared<Range>('0', '9'));

// 单个字母
Re CLexer::alpha = Re(make_shared<Range>('a', 'z')) | Re(make_shared<Range>('A', 'Z'));

// 小数点
Re CLexer::decimalPoint = Re::Symbol('.');

// 下划线
Re CLexer::underline = Re::Symbol('_');

// 行注释
Re CLexer::lineComment = Re(make_shared<Prefix>("//")) + Re::Exclude('\n').zeroOrMore() + Re::Symbol('\n');

// 块注释
Re CLexer::blockComent = Re(make_shared<Prefix>("/*")) + (Re::Exclude('*') | (Re::Symbol('*') + Re::Exclude('/'))).zeroOrMore() + Re(make_shared<Prefix>("*/"));

// 预处理命令
Re CLexer::preprocess = (Re::Symbol('#') + Re::Exclude('\n').zeroOrMore()).setCallback(AddToken(TokenType::Preprocess)) + Re::Symbol('\n');

// 整数
Re CLexer::integer = digit.oneOrMore().setCallback(AddToken(TokenType::Integer));

// 小数
Re CLexer::decimal = (digit.oneOrMore() + decimalPoint + digit.oneOrMore()).setCallback(AddToken(TokenType::Decimal));

// 标识符
Re CLexer::identifier = ((alpha | underline).oneOrMore() + (digit | alpha | underline).zeroOrMore()).setCallback(AddToken(TokenType::Identifier));

// 关键字
Re CLexer::keywords =
(
	Re(make_shared<Prefix>("int")) |
	Re(make_shared<Prefix>("double")) |
	Re(make_shared<Prefix>("char")) |
	Re(make_shared<Prefix>("long")) |
	Re(make_shared<Prefix>("unsigned")) |
	Re(make_shared<Prefix>("short")) |
	Re(make_shared<Prefix>("void")) |
	Re(make_shared<Prefix>("if")) |
	Re(make_shared<Prefix>("switch")) |
	Re(make_shared<Prefix>("case")) |
	Re(make_shared<Prefix>("default")) |
	Re(make_shared<Prefix>("while")) |
	Re(make_shared<Prefix>("for")) |
	Re(make_shared<Prefix>("do")) |
	Re(make_shared<Prefix>("continue")) |
	Re(make_shared<Prefix>("break")) |
	Re(make_shared<Prefix>("const")) |
	Re(make_shared<Prefix>("return")) |
	Re(make_shared<Prefix>("struct")) |
	Re(make_shared<Prefix>("static")) |
	Re(make_shared<Prefix>("register")) |
	Re(make_shared<Prefix>("volatile")) |
	Re(make_shared<Prefix>("sozeof"))
).setCallback(AddToken(TokenType::Keyword));

// 字符串字面量
Re CLexer::stringLiteral = Re::Symbol('"') + Re::Exclude('"').zeroOrMore().setCallback(AddToken(TokenType::String)) + Re::Symbol('"');

// 字符字面量
Re CLexer::charLiteral = Re::Symbol('\'') + Re::Exclude('\'').setCallback(AddToken(TokenType::Char)) + Re::Symbol('\'');

// 运算符
Re CLexer::inc = Re(make_shared<Prefix>("++")).setCallback(AddToken(TokenType::Inc));
Re CLexer::addAssign = Re(make_shared<Prefix>("+=")).setCallback(AddToken(TokenType::AddAssign));
Re CLexer::add = Re(make_shared<Prefix>("+")).setCallback(AddToken(TokenType::Add));
Re CLexer::dec = Re(make_shared<Prefix>("--")).setCallback(AddToken(TokenType::Dec));
Re CLexer::subAssign = Re(make_shared<Prefix>("-=")).setCallback(AddToken(TokenType::SubAssign));
Re CLexer::pointerMember = Re(make_shared<Prefix>("->")).setCallback(AddToken(TokenType::PointerMember));
Re CLexer::sub = Re(make_shared<Prefix>("-")).setCallback(AddToken(TokenType::Sub));
Re CLexer::mulAssign = Re(make_shared<Prefix>("*=")).setCallback(AddToken(TokenType::MulAssign));
Re CLexer::mul = Re(make_shared<Prefix>("*")).setCallback(AddToken(TokenType::Mul));
Re CLexer::divAssign = Re(make_shared<Prefix>("/=")).setCallback(AddToken(TokenType::DivAssign));
Re CLexer::div = Re(make_shared<Prefix>("/")).setCallback(AddToken(TokenType::Div));
Re CLexer::remAssign = Re(make_shared<Prefix>("%=")).setCallback(AddToken(TokenType::RemAssign));
Re CLexer::rem = Re(make_shared<Prefix>("%")).setCallback(AddToken(TokenType::Rem));
Re CLexer::equal = Re(make_shared<Prefix>("==")).setCallback(AddToken(TokenType::Equal));
Re CLexer::assign = Re(make_shared<Prefix>("=")).setCallback(AddToken(TokenType::Assign));
Re CLexer::bitAndAssign = Re(make_shared<Prefix>("&=")).setCallback(AddToken(TokenType::BitAndAssign));
Re CLexer::logicAnd = Re(make_shared<Prefix>("&&")).setCallback(AddToken(TokenType::LogicAnd));
Re CLexer::bitAnd = Re(make_shared<Prefix>("&")).setCallback(AddToken(TokenType::BitAnd));
Re CLexer::bitOrAssign = Re(make_shared<Prefix>("|=")).setCallback(AddToken(TokenType::BitOrAssign));
Re CLexer::logicOr = Re(make_shared<Prefix>("||")).setCallback(AddToken(TokenType::LogicOr));
Re CLexer::bitOr = Re(make_shared<Prefix>("|")).setCallback(AddToken(TokenType::BitOr));
Re CLexer::bitXorAssign = Re(make_shared<Prefix>("^=")).setCallback(AddToken(TokenType::BitXorAssign));
Re CLexer::bitXor = Re(make_shared<Prefix>("^")).setCallback(AddToken(TokenType::BitXor));
Re CLexer::notEqual = Re(make_shared<Prefix>("!=")).setCallback(AddToken(TokenType::NotEqual));
Re CLexer::logicNot = Re(make_shared<Prefix>("!")).setCallback(AddToken(TokenType::LogicNot));
Re CLexer::leftShiftAssign = Re(make_shared<Prefix>("<<=")).setCallback(AddToken(TokenType::LeftShiftAssign));
Re CLexer::leftShift = Re(make_shared<Prefix>("<<")).setCallback(AddToken(TokenType::LeftShift));
Re CLexer::lessEqual = Re(make_shared<Prefix>("<=")).setCallback(AddToken(TokenType::LessEqual));
Re CLexer::less = Re(make_shared<Prefix>("<")).setCallback(AddToken(TokenType::Less));
Re CLexer::rightShiftAssign = Re(make_shared<Prefix>(">>=")).setCallback(AddToken(TokenType::RightShiftAssign));
Re CLexer::rightShift = Re(make_shared<Prefix>(">>")).setCallback(AddToken(TokenType::RightShift));
Re CLexer::largerEqual = Re(make_shared<Prefix>(">=")).setCallback(AddToken(TokenType::LargerEqual));
Re CLexer::larger = Re(make_shared<Prefix>(">")).setCallback(AddToken(TokenType::Larger));
Re CLexer::bitReverse = Re(make_shared<Prefix>("~")).setCallback(AddToken(TokenType::BitReverse));

// 括号
Re CLexer::openParenthese = Re(make_shared<Prefix>("(")).setCallback(AddToken(TokenType::OpenParenthese));
Re CLexer::closeParenthese = Re(make_shared<Prefix>(")")).setCallback(AddToken(TokenType::CloseParenthese));
Re CLexer::openSquareBracket = Re(make_shared<Prefix>("[")).setCallback(AddToken(TokenType::OpenSquareBracket));
Re CLexer::closeSquareBracket = Re(make_shared<Prefix>("]")).setCallback(AddToken(TokenType::CloseSquareBracket));
Re CLexer::openBrace = Re(make_shared<Prefix>("{")).setCallback(AddToken(TokenType::OpenBrace));
Re CLexer::closeBrace = Re(make_shared<Prefix>("}")).setCallback(AddToken(TokenType::CloseBrace));

// 其它符号
Re CLexer::comma = Re(make_shared<Prefix>(",")).setCallback(AddToken(TokenType::Comma));
Re CLexer::member = Re(make_shared<Prefix>(".")).setCallback(AddToken(TokenType::Member));
Re CLexer::semicolon = Re(make_shared<Prefix>(";")).setCallback(AddToken(TokenType::Semicolon));
Re CLexer::colon = Re(make_shared<Prefix>(":")).setCallback(AddToken(TokenType::Colon));
Re CLexer::question = Re(make_shared<Prefix>("?")).setCallback(AddToken(TokenType::Question));

// 总规则
Re CLexer::rules =
(
	blank | lineComment | blockComent | preprocess |
	decimal | integer | keywords | identifier | stringLiteral | charLiteral |
	openParenthese | closeParenthese | openSquareBracket | closeSquareBracket | openBrace | closeBrace |
	inc | addAssign | add |
	dec | subAssign | pointerMember | sub |
	mulAssign | mul |
	divAssign | div |
	remAssign | rem |
	equal | assign |
	bitAndAssign | logicAnd | bitAnd |
	bitOrAssign | logicOr | bitOr |
	bitXorAssign | bitXor |
	notEqual | logicNot |
	leftShiftAssign | leftShift | lessEqual | less |
	rightShiftAssign | rightShift | largerEqual | larger |
	comma | member | semicolon | colon | bitReverse | question
).oneOrMore();

vector<CLexer::Token> CLexer::tokens;

std::vector<CLexer::Token> byx::CLexer::Lex(const std::string& s)
{
	vector<char> v(s.begin(), s.end());
	v.push_back('\n');
	tokens.clear();
	rules.parse(v);
	return tokens;
}

std::function<void(const std::vector<char>&, size_t, size_t)> byx::CLexer::AddToken(TokenType type)
{
	return [type](const std::vector<char>& input, size_t begin, size_t end)
	{
		string value = "";
		for (size_t i = begin; i < end; ++i)
		{
			value.push_back(input[i]);
		}
		tokens.push_back(Token(type, value));
	};
}