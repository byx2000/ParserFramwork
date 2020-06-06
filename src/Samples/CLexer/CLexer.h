/******************************************************
 *	Author: byx
 *	Date: 2020/6/5
******************************************************/

#pragma once

#include "../../Framwork/Rule.h"

namespace byx
{
	using Re = Rule<char>;

	// 示例：C语言词法分析器
	class CLexer
	{
	public:
        // 单词类型
        enum class TokenType
        {
            Unknown,
            Integer,
            Decimal,
            String,
            Char,
            Identifier,
            Keyword,
            Add,
            Inc,
            AddAssign,
            Assign,
            Equal,
            OpenParenthese,
            CloseParenthese,
            OpenBrace,
            CloseBrace,
            OpenSquareBracket,
            CloseSquareBracket,
            Comma,
            Semicolon,
            Sub,
            Dec,
            SubAssign,
            Mul,
            MulAssign,
            Div,
            DivAssign,
            Rem,
            RemAssign,
            LogicNot,
            NotEqual,
            BitReverse,
            Less,
            Larger,
            LessEqual,
            LargerEqual,
            BitAnd,
            LogicAnd,
            BitAndAssign,
            BitOr,
            LogicOr,
            BitOrAssign,
            BitXor,
            BitXorAssign,
            LeftShift,
            LeftShiftAssign,
            RightShift,
            RightShiftAssign,
            Preprocess,
            Member,
            Colon,
            Question,
            PointerMember,
            End,
        };

        // 单词
		struct Token
		{
			TokenType type;
			std::string value;

			Token();
			Token(TokenType type, const std::string& val);
			bool operator==(const Token& token) const;
		};

		static std::vector<Token> Lex(const std::string& s);
	private:
		static Re blank, digit, alpha, decimalPoint, underline;
		static Re lineComment, blockComent;
		static Re preprocess;
		static Re integer, decimal, identifier, keywords, stringLiteral, charLiteral;
		static Re inc, addAssign, add;
		static Re dec, subAssign, pointerMember, sub;
		static Re mulAssign, mul;
		static Re divAssign, div;
		static Re remAssign, rem;
		static Re equal, assign;
		static Re bitAndAssign, logicAnd, bitAnd;
		static Re bitOrAssign, logicOr, bitOr;
		static Re bitXorAssign, bitXor;
		static Re notEqual, logicNot;
		static Re leftShiftAssign, leftShift, lessEqual, less;
		static Re rightShiftAssign, rightShift, largerEqual, larger;
		static Re openParenthese, closeParenthese, openSquareBracket, closeSquareBracket, openBrace, closeBrace;
		static Re comma, member, semicolon, colon, bitReverse, question;
		static Re rules;

		static std::vector<Token> tokens;

		static std::function<void(const std::vector<char>&, size_t, size_t)> AddToken(TokenType type);
	};
}