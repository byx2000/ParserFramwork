#pragma once

namespace byx
{
	class CLexerTest
	{
	public:
		void static Run();
	private:
		void static IntegerTest();
		void static DecimalTest();
		void static IntegerDecimalTest();
		void static IdentifierTest();
		void static IntegerDecimalIdentTest();
		void static KeywordTest();
		void static StringTest();
		void static CharTest();
		void static LineComment();
		void static BlockComment();
		void static AddTest();
		void static SubTest();
		void static MulTest();
		void static DivTest();
		void static RemTest();
		void static EqualTest();
		void static LogicAndTest();
		void static LogicOrTest();
		void static BitXorTest();
		void static LogicNotTest();
		void static LessTest();
		void static LargerTest();
		void static BracketTest();
		void static OtherTest();

		void static PreprocessTest();
		void static ArithExprTest();
		void static BitExprTest();
		void static CmpExprTest();
		void static DeclareStmtTest();
	};
}