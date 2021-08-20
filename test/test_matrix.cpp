#include <gtest/gtest.h>

#include "../src/math/matrix.hpp"
#include "../src/iterator/stride_iterator.hpp"


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
		static_assert(row1_view[0] == 5);
		static_assert(row1_view[1] == 7);

		constexpr auto row0 = mat1.get_row(0);
		static_assert(row0[0] == 1);
		static_assert(row0[1] == 3);

		constexpr auto col0_view_cannot_use = mat1.get_column_view(0);
		static_assert(col0_view_cannot_use[0] == 1);
		static_assert(col0_view_cannot_use[1] == 5);
		
		constexpr auto col1 = mat1.get_column(1);
		static_assert(col1[0] == 3);
		static_assert(col1[1] == 7);
	}
}
