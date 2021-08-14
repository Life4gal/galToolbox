#include <gtest/gtest.h>

#include "../src/math/vector2.hpp"
#include "../src/math/vector1.hpp"

TEST(TestVector2, TestBasic)
{
	using namespace gal::test;

	static_assert(vector2<int>::data_size == 2);
	static_assert(vector2<int>::data_index == 1);
	static_assert(vector2<int>::size() == 2);

	{
		// default
		constexpr vector2<int> vec{};

		static_assert(vec[0] == 0);
		static_assert(vec[1] == 0);
	}
	{
		constexpr vector1<float> vec_f{ 3.14f };

		constexpr vector2<unsigned int> vec_u{ 10 };
		constexpr vector2<int> vec_i1{ 10 };
		constexpr vector2<int> vec_i2{ 10, 20 };
		constexpr vector2<int> vec_i3{ -10, -20 };
		constexpr vector2<double> vec_d0{ vec_f };
		constexpr vector2<double> vec_d1{ vec_f, 123 };
		constexpr vector2<double> vec_d2{ 456, vec_f };
		constexpr vector2<double> vec_d3{ vec_f, vec_f };

		static_assert(vec_u[0] == 10);
		static_assert(vec_u[1] == 10);
		static_assert((-vec_u)[0] == -10);
		static_assert((-vec_u)[1] == -10);

		static_assert(vec_i1[0] == 10);
		static_assert(vec_i1[1] == 10);

		static_assert(vec_i2[0] == 10);
		static_assert(vec_i2[1] == 20);

		static_assert(vec_i3[0] == -10);
		static_assert(vec_i3[1] == -20);

		static_assert(vec_d0[0] == vec_f[0]);
		static_assert(vec_d0[1] == vec_f[0]);
		
		static_assert(vec_d1[0] == vec_f[0]);
		static_assert(vec_d1[1] == 123);

		static_assert(vec_d2[0] == 456);
		static_assert(vec_d2[1] == vec_f[0]);

		static_assert(vec_d3[0] == vec_f[0]);
		static_assert(vec_d3[1] == vec_f[0]);

		static_assert(vec_i1 != vec_i2);
		static_assert((+vec_i2) == (-vec_i3));
		static_assert(vec_u == vec_i1);

		static_assert((~vec_i2)[0] == ~10);
		static_assert((~vec_i2)[1] == ~20);
		static_assert((~vec_d0)[0] == ~vec_f);
		static_assert((~vec_d0)[1] == ~vec_f);
	}
}

TEST(TestVector2, TestSelfOperator)
{
	using namespace gal::test;

	{
		constexpr vector1<float> vec0{ 3.14f };
		constexpr vector1<int> vec1{ 10 };
		vector2<unsigned long> vec2{ 42, 128 };

		vec2 += vec0;
		ASSERT_EQ(vec2[0], 42 + vec0);
		ASSERT_EQ(vec2[1], 128 + vec0);
		
		vec2 -= vec1;
		ASSERT_EQ(vec2[0], 45 - vec1);
		ASSERT_EQ(vec2[1], 131 - vec1);

		vec2 *= vec1 + vec0; // *= 13
		ASSERT_EQ(vec2[0], 35 * (vec1 + vec0));
		ASSERT_EQ(vec2[1], 121 * (vec1 + vec0));

		vec2 /= (vec1 - 8);
		ASSERT_EQ(vec2[0], 455 / 2);
		ASSERT_EQ(vec2[1], 1573 / 2);

		vec2 %= (vec1 + 10);
		ASSERT_EQ(vec2[0], 7);
		ASSERT_EQ(vec2[1], 6);

		vec2 += vec0;
		ASSERT_EQ(vec2[0], 10);
		ASSERT_EQ(vec2[1], 9);

		vec2 &= 7;
		ASSERT_EQ(vec2[0], 2);
		ASSERT_EQ(vec2[1], 1);

		vec2 |= 22;
		ASSERT_EQ(vec2[0], 22);
		ASSERT_EQ(vec2[1], 23);

		vec2 ^= 13;
		ASSERT_EQ(vec2[0], 27);
		ASSERT_EQ(vec2[1], 26);

		vec2 >>= 2;
		ASSERT_EQ(vec2[0], 27 >> 2);
		ASSERT_EQ(vec2[1], 26 >> 2);

		vec2 <<= vec1;
		ASSERT_EQ(vec2[0], 6 << vec1);
		ASSERT_EQ(vec2[1], 6 << vec1);
	}
}

TEST(TestVector2, TestOperator)
{
	using namespace gal::test;

	{
		constexpr unsigned long long vec_ull_value1 = 80808080;
		constexpr unsigned long long vec_ull_value2 = 8080808;

		constexpr long long vec_ll_value1 = 123456789;
		constexpr long long vec_ll_value2 = 987654321;

		constexpr vector2<unsigned long long> vec_ull{ vec_ull_value1, vec_ull_value2 };
		constexpr vector2<long long> vec_ll{ vec_ll_value1, vec_ll_value2 };

		{
			constexpr auto vec1 = vec_ull + vec_ll;
			static_assert(vec1[0] == vec_ull_value1 + vec_ll_value1);
			static_assert(vec1[1] == vec_ull_value2 + vec_ll_value2);

			constexpr auto vec21 = vec_ull + vec_ll_value1;
			constexpr auto vec22 = vec_ull + vec_ll_value2;
			static_assert(vec21[0] == vec_ull_value1 + vec_ll_value1);
			static_assert(vec21[1] == vec_ull_value2 + vec_ll_value1);
			static_assert(vec22[0] == vec_ull_value1 + vec_ll_value2);
			static_assert(vec22[1] == vec_ull_value2 + vec_ll_value2);

			constexpr auto vec31 = vec_ull_value1 + vec_ll;
			constexpr auto vec32 = vec_ull_value2 + vec_ll;
			static_assert(vec31[0] == vec_ull_value1 + vec_ll_value1);
			static_assert(vec31[1] == vec_ull_value1 + vec_ll_value2);
			static_assert(vec32[0] == vec_ull_value2 + vec_ll_value1);
			static_assert(vec32[1] == vec_ull_value2 + vec_ll_value2);
		}
		{
			constexpr auto vec1 = vec_ull - vec_ll;
			static_assert(vec1[0] == vec_ull_value1 - vec_ll_value1);
			static_assert(vec1[1] == vec_ull_value2 - vec_ll_value2);

			constexpr auto vec21 = vec_ull - vec_ll_value1;
			constexpr auto vec22 = vec_ull - vec_ll_value2;
			static_assert(vec21[0] == vec_ull_value1 - vec_ll_value1);
			static_assert(vec21[1] == vec_ull_value2 - vec_ll_value1);
			static_assert(vec22[0] == vec_ull_value1 - vec_ll_value2);
			static_assert(vec22[1] == vec_ull_value2 - vec_ll_value2);

			constexpr auto vec31 = vec_ull_value1 - vec_ll;
			constexpr auto vec32 = vec_ull_value2 - vec_ll;
			static_assert(vec31[0] == vec_ull_value1 - vec_ll_value1);
			static_assert(vec31[1] == vec_ull_value1 - vec_ll_value2);
			static_assert(vec32[0] == vec_ull_value2 - vec_ll_value1);
			static_assert(vec32[1] == vec_ull_value2 - vec_ll_value2);
		}
		{
			constexpr auto vec1 = vec_ull * vec_ll;
			static_assert(vec1[0] == vec_ull_value1 * vec_ll_value1);
			static_assert(vec1[1] == vec_ull_value2 * vec_ll_value2);

			constexpr auto vec21 = vec_ull * vec_ll_value1;
			constexpr auto vec22 = vec_ull * vec_ll_value2;
			static_assert(vec21[0] == vec_ull_value1 * vec_ll_value1);
			static_assert(vec21[1] == vec_ull_value2 * vec_ll_value1);
			static_assert(vec22[0] == vec_ull_value1 * vec_ll_value2);
			static_assert(vec22[1] == vec_ull_value2 * vec_ll_value2);

			constexpr auto vec31 = vec_ull_value1 * vec_ll;
			constexpr auto vec32 = vec_ull_value2 * vec_ll;
			static_assert(vec31[0] == vec_ull_value1 * vec_ll_value1);
			static_assert(vec31[1] == vec_ull_value1 * vec_ll_value2);
			static_assert(vec32[0] == vec_ull_value2 * vec_ll_value1);
			static_assert(vec32[1] == vec_ull_value2 * vec_ll_value2);
		}
		{
			constexpr auto vec1 = vec_ull / vec_ll;
			static_assert(vec1[0] == vec_ull_value1 / vec_ll_value1);
			static_assert(vec1[1] == vec_ull_value2 / vec_ll_value2);

			constexpr auto vec21 = vec_ull / vec_ll_value1;
			constexpr auto vec22 = vec_ull / vec_ll_value2;
			static_assert(vec21[0] == vec_ull_value1 / vec_ll_value1);
			static_assert(vec21[1] == vec_ull_value2 / vec_ll_value1);
			static_assert(vec22[0] == vec_ull_value1 / vec_ll_value2);
			static_assert(vec22[1] == vec_ull_value2 / vec_ll_value2);

			constexpr auto vec31 = vec_ull_value1 / vec_ll;
			constexpr auto vec32 = vec_ull_value2 / vec_ll;
			static_assert(vec31[0] == vec_ull_value1 / vec_ll_value1);
			static_assert(vec31[1] == vec_ull_value1 / vec_ll_value2);
			static_assert(vec32[0] == vec_ull_value2 / vec_ll_value1);
			static_assert(vec32[1] == vec_ull_value2 / vec_ll_value2);
		}
		{
			constexpr auto vec1 = vec_ull % vec_ll;
			static_assert(vec1[0] == vec_ull_value1 % vec_ll_value1);
			static_assert(vec1[1] == vec_ull_value2 % vec_ll_value2);

			constexpr auto vec21 = vec_ull % vec_ll_value1;
			constexpr auto vec22 = vec_ull % vec_ll_value2;
			static_assert(vec21[0] == vec_ull_value1 % vec_ll_value1);
			static_assert(vec21[1] == vec_ull_value2 % vec_ll_value1);
			static_assert(vec22[0] == vec_ull_value1 % vec_ll_value2);
			static_assert(vec22[1] == vec_ull_value2 % vec_ll_value2);

			constexpr auto vec31 = vec_ull_value1 % vec_ll;
			constexpr auto vec32 = vec_ull_value2 % vec_ll;
			static_assert(vec31[0] == vec_ull_value1 % vec_ll_value1);
			static_assert(vec31[1] == vec_ull_value1 % vec_ll_value2);
			static_assert(vec32[0] == vec_ull_value2 % vec_ll_value1);
			static_assert(vec32[1] == vec_ull_value2 % vec_ll_value2);
		}
		{
			constexpr auto vec1 = vec_ull &  vec_ll;
			static_assert(vec1[0] == (vec_ull_value1 & vec_ll_value1));
			static_assert(vec1[1] == (vec_ull_value2 & vec_ll_value2));

			constexpr auto vec21 = vec_ull & vec_ll_value1;
			constexpr auto vec22 = vec_ull & vec_ll_value2;
			static_assert(vec21[0] == (vec_ull_value1 & vec_ll_value1));
			static_assert(vec21[1] == (vec_ull_value2 & vec_ll_value1));
			static_assert(vec22[0] == (vec_ull_value1 & vec_ll_value2));
			static_assert(vec22[1] == (vec_ull_value2 & vec_ll_value2));

			constexpr auto vec31 = vec_ull_value1 & vec_ll;
			constexpr auto vec32 = vec_ull_value2 & vec_ll;
			static_assert(vec31[0] == (vec_ull_value1 & vec_ll_value1));
			static_assert(vec31[1] == (vec_ull_value1 & vec_ll_value2));
			static_assert(vec32[0] == (vec_ull_value2 & vec_ll_value1));
			static_assert(vec32[1] == (vec_ull_value2 & vec_ll_value2));
		}
		{
			constexpr auto vec1 = vec_ull | vec_ll;
			static_assert(vec1[0] == (vec_ull_value1 | vec_ll_value1));
			static_assert(vec1[1] == (vec_ull_value2 | vec_ll_value2));

			constexpr auto vec21 = vec_ull | vec_ll_value1;
			constexpr auto vec22 = vec_ull | vec_ll_value2;
			static_assert(vec21[0] == (vec_ull_value1 | vec_ll_value1));
			static_assert(vec21[1] == (vec_ull_value2 | vec_ll_value1));
			static_assert(vec22[0] == (vec_ull_value1 | vec_ll_value2));
			static_assert(vec22[1] == (vec_ull_value2 | vec_ll_value2));

			constexpr auto vec31 = vec_ull_value1 | vec_ll;
			constexpr auto vec32 = vec_ull_value2 | vec_ll;
			static_assert(vec31[0] == (vec_ull_value1 | vec_ll_value1));
			static_assert(vec31[1] == (vec_ull_value1 | vec_ll_value2));
			static_assert(vec32[0] == (vec_ull_value2 | vec_ll_value1));
			static_assert(vec32[1] == (vec_ull_value2 | vec_ll_value2));
		}
		{
			constexpr auto vec1 = vec_ull ^ vec_ll;
			static_assert(vec1[0] == (vec_ull_value1 ^ vec_ll_value1));
			static_assert(vec1[1] == (vec_ull_value2 ^ vec_ll_value2));

			constexpr auto vec21 = vec_ull ^ vec_ll_value1;
			constexpr auto vec22 = vec_ull ^ vec_ll_value2;
			static_assert(vec21[0] == (vec_ull_value1 ^ vec_ll_value1));
			static_assert(vec21[1] == (vec_ull_value2 ^ vec_ll_value1));
			static_assert(vec22[0] == (vec_ull_value1 ^ vec_ll_value2));
			static_assert(vec22[1] == (vec_ull_value2 ^ vec_ll_value2));

			constexpr auto vec31 = vec_ull_value1 ^ vec_ll;
			constexpr auto vec32 = vec_ull_value2 ^ vec_ll;
			static_assert(vec31[0] == (vec_ull_value1 ^ vec_ll_value1));
			static_assert(vec31[1] == (vec_ull_value1 ^ vec_ll_value2));
			static_assert(vec32[0] == (vec_ull_value2 ^ vec_ll_value1));
			static_assert(vec32[1] == (vec_ull_value2 ^ vec_ll_value2));
		}
		{
			constexpr auto vec = vec_ull << 12;
			static_assert(vec[0] == (vec_ull_value1 << 12));
			static_assert(vec[1] == (vec_ull_value2 << 12));
		}
		{
			constexpr auto vec = vec_ll >> 13;
			static_assert(vec[0] == (vec_ll_value1 >> 13));
			static_assert(vec[1] == (vec_ll_value2 >> 13));
		}
	}
}
