#include <gtest/gtest.h>

#include "../src/iterator/stride_iterator.hpp"

#include <ranges>
#include <array>

TEST(TestStrideIterator, TestAll)
{
	using namespace gal::test::iterator;

	{
		constexpr std::array<int, 10> arr{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		constexpr auto it = make_stride_iterator<2>(arr.cbegin() + 1, 4);
		static_assert(it[0] == 2);
		static_assert(it[1] == 4);
		static_assert(it[2] == 6);
		static_assert(it[3] == 8);

		constexpr auto view = std::views::counted(it, 4);
		static_assert(view[0] == 2);
		static_assert(view[1] == 4);
		static_assert(view[2] == 6);
		static_assert(view[3] == 8);

		for(auto i : view)
		{
			// ensure that only 4 values are output (2, 4, 6, 8)
			std::cout << i << '\t';
		}
	}
}
