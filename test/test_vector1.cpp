#include <gtest/gtest.h>

#include "../src/math/vector1.hpp"

TEST(TestVector1, TestBasic)
{
	using namespace gal::test;

	static_assert(vector1<int>::data_size == 1);
	static_assert(vector1<int>::data_index == 0);
	static_assert(vector1<int>::size() == 1);

	{
		// default
		constexpr vector1<int> vec{};
		
		static_assert(vec[0] == 0);
	}
	{
		constexpr vector1<int> vec1{ 10 };
		constexpr vector1<long> vec2{ -10 };
		
		// index is ignored
		static_assert(vec1[0] == 10);
		static_assert(vec1[1] == 10);
		static_assert(vec2[0] == -10);
		static_assert(vec2[1] == -10);

		static_assert(vec1 != vec2);
		static_assert(+vec1 == -vec2);

		static_assert(~vec1[0] == ~10);
		static_assert(~vec2[0] == ~-10);
	}
}

TEST(TestVector1, TestSelfOperator)
{
	using namespace gal::test;
	
	{
		vector1<unsigned int> vec{ 42 };

		vec += 10;
		ASSERT_EQ(vec.x, 52);

		vec -= 20;
		ASSERT_EQ(vec.x, 32);

		vec *= 2;
		ASSERT_EQ(vec.x, 64);

		vec /= 32;
		ASSERT_EQ(vec.x, 2);

		vec %= 2;
		ASSERT_EQ(vec.x, 0);

		vec += 15;
		ASSERT_EQ(vec.x, 15);

		vec &= 7;
		ASSERT_EQ(vec.x, 7);

		vec |= 15;
		ASSERT_EQ(vec.x, 15);

		vec ^= 24;
		ASSERT_EQ(vec.x, 23);

		vec >>= 2;
		ASSERT_EQ(vec.x, 5);

		vec <<= 3;
		ASSERT_EQ(vec.x, 40);
	}
}

TEST(TestVector1, TestOperator)
{
	using namespace gal::test;

	{
		constexpr unsigned long long vec_ull_value = 80808080;
		constexpr long long vec_ll_value = 8080808;
		
		constexpr vector1<unsigned long long> vec_ull{ vec_ull_value };
		constexpr vector1<long long> vec_ll{ vec_ll_value };

		{
			constexpr auto vec1 = vec_ull + vec_ll;
			static_assert(vec1.x == vec_ull_value + vec_ll_value);

			constexpr auto vec2 = vec_ull + vec_ll_value;
			static_assert(vec2.x == vec_ull_value + vec_ll_value);

			constexpr auto vec3 =  vec_ull_value + vec_ll;
			static_assert(vec3.x == vec_ull_value + vec_ll_value);
		}
		{
			constexpr auto vec1 = vec_ull - vec_ll;
			static_assert(vec1.x == vec_ull_value - vec_ll_value);

			constexpr auto vec2 = vec_ull - vec_ll_value;
			static_assert(vec2.x == vec_ull_value - vec_ll_value);

			constexpr auto vec3 = vec_ull_value - vec_ll;
			static_assert(vec3.x == vec_ull_value - vec_ll_value);
		}
		{
			constexpr auto vec1 = vec_ull * vec_ll;
			static_assert(vec1.x == vec_ull_value * vec_ll_value);

			constexpr auto vec2 = vec_ull * vec_ll_value;
			static_assert(vec2.x == vec_ull_value * vec_ll_value);

			constexpr auto vec3 = vec_ull_value * vec_ll;
			static_assert(vec3.x == vec_ull_value * vec_ll_value);
		}
		{
			constexpr auto vec1 = vec_ull / vec_ll;
			static_assert(vec1.x == vec_ull_value / vec_ll_value);

			constexpr auto vec2 = vec_ull / vec_ll_value;
			static_assert(vec2.x == vec_ull_value / vec_ll_value);

			constexpr auto vec3 = vec_ull_value / vec_ll;
			static_assert(vec3.x == vec_ull_value / vec_ll_value);
		}
		{
			constexpr auto vec1 = vec_ull % vec_ll;
			static_assert(vec1.x == vec_ull_value % vec_ll_value);

			constexpr auto vec2 = vec_ull % vec_ll_value;
			static_assert(vec2.x == vec_ull_value % vec_ll_value);

			constexpr auto vec3 = vec_ull_value % vec_ll;
			static_assert(vec3.x == vec_ull_value % vec_ll_value);
		}
		{
			constexpr auto vec1 = vec_ull & vec_ll;
			static_assert(vec1.x == (vec_ull_value & vec_ll_value));

			constexpr auto vec2 = vec_ull & vec_ll_value;
			static_assert(vec2.x == (vec_ull_value & vec_ll_value));

			constexpr auto vec3 = vec_ull_value & vec_ll;
			static_assert(vec3.x == (vec_ull_value & vec_ll_value));
		}
		{
			constexpr auto vec1 = vec_ull | vec_ll;
			static_assert(vec1.x == (vec_ull_value | vec_ll_value));

			constexpr auto vec2 = vec_ull | vec_ll_value;
			static_assert(vec2.x == (vec_ull_value | vec_ll_value));

			constexpr auto vec3 = vec_ull_value | vec_ll;
			static_assert(vec3.x == (vec_ull_value | vec_ll_value));
		}
		{
			constexpr auto vec1 = vec_ull ^ vec_ll;
			static_assert(vec1.x == (vec_ull_value ^ vec_ll_value));

			constexpr auto vec2 = vec_ull ^ vec_ll_value;
			static_assert(vec2.x == (vec_ull_value ^ vec_ll_value));

			constexpr auto vec3 = vec_ull_value ^ vec_ll;
			static_assert(vec3.x == (vec_ull_value ^ vec_ll_value));
		}
		{
			constexpr auto vec = vec_ull << 12;
			static_assert(vec.x == (vec_ull_value << 12));
		}
		{
			constexpr auto vec = vec_ll >> 15;
			static_assert(vec.x == (vec_ll_value >>15));
		}
	}
}
