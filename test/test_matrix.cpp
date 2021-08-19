#include <gtest/gtest.h>

#include "../src/math/matrix.hpp"
#include <ranges>
TEST(TestMatrix, TestBasic)
{
	using namespace gal::test::math;

	{
		constexpr auto           tuple1 = std::make_tuple(1, 2, 3, 4, 5, 6, 7, 8, 9);
		constexpr matrix2x2<int> mat1{tuple1, std::index_sequence<0, 2, 4, 6>{}};
		static_assert(mat1[0] == 1);
		static_assert(mat1[1] == 3);
		static_assert(mat1[2] == 5);
		static_assert(mat1[3] == 7);

		constexpr auto row1_view = mat1.get_row_view(1);
		static_assert(std::ranges::count_if(row1_view,
											[](auto) constexpr-> bool
											{
												return true;
											}) == 2);
		static_assert(row1_view[0] == 5);
		static_assert(row1_view[1] == 7);
		static_assert(*row1_view.begin() == 5);
		static_assert(*(row1_view.begin() + 1) == 7);

		constexpr auto row0 = mat1.get_row(0);
		static_assert(row0[0] == 1);
		static_assert(row0[1] == 3);

		// we can get a view after a filter modified by const but can't do anything
		// https://stackoverflow.com/questions/66366084/why-cant-i-call-rangesbegin-on-a-const-filter-view
		constexpr auto col0_view_cannot_use = mat1.get_column_view(0);
		// static_assert(std::ranges::count_if(col0_view_cannot_use, [](const auto&) constexpr->bool { return true; }) == 2);
		// std::views::filter does not provide operator[]
		// static_assert(col0_view_cannot_use[0] == 1);
		// static_assert(col0_view_cannot_use[1] == 5);
		// static_assert(*col0_view_cannot_use.begin() == 1);
		// static_assert(*(col0_view_cannot_use.begin() + 1) == 5);

		auto col0_view = mat1.get_column_view(0);
		ASSERT_EQ(std::ranges::count_if(col0_view, [](const auto&) constexpr->bool { return true; }), 2);
		// std::views::filter does not provide operator[]
		// ASSERT_EQ(*col0_view[0], 1);
		// ASSERT_EQ(*col0_view[1], 5);
		ASSERT_EQ(*col0_view.begin(), 1);
		ASSERT_EQ(*(std::ranges::next(col0_view.begin())), 5);

		constexpr auto col1 = mat1.get_column(1);
		static_assert(col1[0] == 3);
		static_assert(col1[1] == 7);
	}
}
