#include <gtest/gtest.h>

#include "../src/math/matrix.hpp"
#include "../src/iterator/stride_iterator.hpp"

TEST(TestMatrix, TestBasic)
{
	using namespace gal::test::math;

	{
		constexpr auto           tuple = std::make_tuple(1, 2, 3, 4, 5, 6, 7, 8, 9);
		constexpr matrix2x2<int> matrix{tuple, std::index_sequence<0, 2, 4, 6>{}};
		static_assert(matrix[0] == 1);
		static_assert(matrix[1] == 3);
		static_assert(matrix[2] == 5);
		static_assert(matrix[3] == 7);

		constexpr auto row1_view = matrix.get_row_view(1);
		static_assert(row1_view[0] == 5);
		static_assert(row1_view[1] == 7);

		std::cout << "output row1(should be 5, 7): ";
		for (auto i : row1_view)
		{
			// ensure that only 2 values are output (5, 7)
			std::cout << i << '\t';
		}

		constexpr auto row0 = matrix.get_row(0);
		static_assert(row0[0] == 1);
		static_assert(row0[1] == 3);

		std::cout << "\noutput row0(should be 1, 3): ";
		for (auto i : row0)
		{
			// ensure that only 2 values are output (1, 3)
			std::cout << i << '\t';
		}


		constexpr auto col0_view = matrix.get_column_view(0);
		static_assert(col0_view[0] == 1);
		static_assert(col0_view[1] == 5);

		std::cout << "\noutput col0(should be 1, 5): ";
		for (auto i : col0_view)
		{
			// ensure that only 2 values are output (1, 5)
			std::cout << i << '\t';
		}

		constexpr auto col1 = matrix.get_column(1);
		static_assert(col1[0] == 3);
		static_assert(col1[1] == 7);

		std::cout << "\noutput col1(should be 3, 7): ";
		for (auto i : col1)
		{
			// ensure that only 2 values are output (3, 7)
			std::cout << i << '\t';
		}
		std::cout << '\n';
	}
	{
		constexpr auto           tuple = std::make_tuple(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);
		constexpr matrix2x3<int> matrix{ tuple, std::index_sequence<0, 2, 4, 6, 8, 10>{} };
		static_assert(matrix[0] == 1);
		static_assert(matrix[1] == 3);
		static_assert(matrix[2] == 5);
		static_assert(matrix[3] == 7);
		static_assert(matrix[4] == 9);
		static_assert(matrix[5] == 11);

		constexpr auto row1_view = matrix.get_row_view(1);
		static_assert(row1_view[0] == 7);
		static_assert(row1_view[1] == 9);
		static_assert(row1_view[2] == 11);

		std::cout << "output row1(should be 7, 9, 11): ";
		for (auto i : row1_view)
		{
			// ensure that only 3 values are output (7, 9, 11)
			std::cout << i << '\t';
		}

		constexpr auto row0 = matrix.get_row(0);
		static_assert(row0[0] == 1);
		static_assert(row0[1] == 3);
		static_assert(row0[2] == 5);

		std::cout << "\noutput row0(should be 1, 3, 5): ";
		for (auto i : row0)
		{
			// ensure that only 3 values are output (1, 3, 5)
			std::cout << i << '\t';
		}


		constexpr auto col0_view = matrix.get_column_view(0);
		static_assert(col0_view[0] == 1);
		static_assert(col0_view[1] == 7);

		std::cout << "\noutput col0(should be 1, 7): ";
		for (auto i : col0_view)
		{
			// ensure that only 2 values are output (1, 7)
			std::cout << i << '\t';
		}

		constexpr auto col1 = matrix.get_column(1);
		static_assert(col1[0] == 3);
		static_assert(col1[1] == 9);

		std::cout << "\noutput col1(should be 3, 9): ";
		for (auto i : col1)
		{
			// ensure that only 2 values are output (3, 9)
			std::cout << i << '\t';
		}
		std::cout << '\n';
	}
}
