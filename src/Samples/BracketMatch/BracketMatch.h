#pragma once

#include "../../Framwork/Rule.h"

#include <string>

namespace byx
{
	// Ê¾Àý£ºÀ¨ºÅÆ¥Åä¼ì²â
	class BracketMatch
	{
	public:
		static bool Match(const std::string& s);
	private:
		static Rule<char> lp, rp, empty, term, expr, lazyExpr;
		static Rule<char> GetExpr();
	};
}