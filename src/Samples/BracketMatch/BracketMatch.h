#pragma once

#include "../../Framwork/Rule.h"

#include <string>

namespace byx
{
	// ʾ��������ƥ����
	class BracketMatch
	{
	public:
		static bool Match(const std::string& s);
	private:
		static Rule<char> lp, rp, empty, term, expr, lazyExpr;
		static Rule<char> GetExpr();
	};
}