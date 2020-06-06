/******************************************************
 *	Author: byx
 *	Date: 2020/6/4
******************************************************/

#pragma once

#include <vector>
#include <stack>

namespace byx
{
	// Scanner
	//  ‰»Î¡˜…®√Ë∆˜

	template<typename T>
	class Scanner
	{
	public:
		Scanner(const std::vector<T>& elements) : elements(elements), curIndex(0) {}

		const std::vector<T>& getElements() const
		{
			return elements;
		}

		size_t getIndex() const
		{
			return curIndex;
		}

		void setIndex(size_t val)
		{
			curIndex = val;
		}

		T next()
		{
			return elements[curIndex++];
		}

		bool end() const
		{
			return curIndex == elements.size();
		}

	private:
		std::vector<T> elements;
		size_t curIndex;
	};
}
