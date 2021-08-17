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
		constexpr vector1<double> vec3{ -10 };
		constexpr vector1<unsigned long> vec4{ 10 };
		
		// index is ignored
		static_assert(vec1[0] == 10);
		static_assert(vec1[1] == 10);
		static_assert(vec2[0] == -10);
		static_assert(vec2[1] == -10);
		static_assert(vec3[0] == -10);
		static_assert(vec3[1] == -10);
		static_assert(vec4[0] == 10);
		static_assert(vec4[1] == 10);

		static_assert(vec1 != vec2);
		static_assert(vec1 == (-vec2));
		static_assert(vec2 == vec3);
		static_assert(vec1 == vec4);

		static_assert((~vec1)[0] == ~10);
		static_assert((~vec2)[0] == ~-10);
		static_assert((~vec3)[0] == -10);
		static_assert((~vec4)[0] == ~10);
	}
}

TEST(TestVector1, TestSelfOperator)
{
	using namespace gal::test;
	
	{
		vector1<unsigned int> vec1{ 42 };
		vector1<double> vec2{ 42 };

		vec1 += 10;
		vec2 += 10;
		ASSERT_EQ(vec1, 52);
		ASSERT_EQ(vec2, 52);

		vec1 -= 20;
		vec2 -= 20;
		ASSERT_EQ(vec1, 32);
		ASSERT_EQ(vec2, 32);

		vec1 *= 2;
		vec2 *= 2;
		ASSERT_EQ(vec1, 64);
		ASSERT_EQ(vec2, 64);

		vec1 /= 32;
		vec2 /= 32;
		ASSERT_EQ(vec1, 2);
		ASSERT_EQ(vec2, 2);

		vec1 %= 2;
		vec2 %= 2;
		ASSERT_EQ(vec1, 0);
		ASSERT_EQ(vec2, 0);

		vec1 += 15;
		vec2 += 10;
		ASSERT_EQ(vec1, 15);
		ASSERT_EQ(vec2, 10);

		vec1 &= 7;
		vec2 &= 7;
		ASSERT_EQ(vec1, 7);
		ASSERT_EQ(vec2, 10);

		vec1 |= 15;
		vec2 |= 15;
		ASSERT_EQ(vec1, 15);
		ASSERT_EQ(vec2, 10);

		vec1 ^= 24;
		vec2 ^= 24;
		ASSERT_EQ(vec1, 23);
		ASSERT_EQ(vec2, 10);

		vec1 >>= 2;
		vec2 >>= 2;
		ASSERT_EQ(vec1, 5);
		ASSERT_EQ(vec2, 2.5);

		vec1 <<= 3;
		vec2 <<= 3;
		ASSERT_EQ(vec1, 40);
		ASSERT_EQ(vec2, 20);
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
			static_assert(vec1 == vec_ull_value + vec_ll_value);

			constexpr auto vec2 = vec_ull + vec_ll_value;
			static_assert(vec2 == vec_ull_value + vec_ll_value);

			constexpr auto vec3 =  vec_ull_value + vec_ll;
			static_assert(vec3 == vec_ull_value + vec_ll_value);
		}
		{
			constexpr auto vec1 = vec_ull - vec_ll;
			static_assert(vec1 == vec_ull_value - vec_ll_value);

			constexpr auto vec2 = vec_ull - vec_ll_value;
			static_assert(vec2 == vec_ull_value - vec_ll_value);

			constexpr auto vec3 = vec_ull_value - vec_ll;
			static_assert(vec3 == vec_ull_value - vec_ll_value);
		}
		{
			constexpr auto vec1 = vec_ull * vec_ll;
			static_assert(vec1 == vec_ull_value * vec_ll_value);

			constexpr auto vec2 = vec_ull * vec_ll_value;
			static_assert(vec2 == vec_ull_value * vec_ll_value);

			constexpr auto vec3 = vec_ull_value * vec_ll;
			static_assert(vec3 == vec_ull_value * vec_ll_value);
		}
		{
			constexpr auto vec1 = vec_ull / vec_ll;
			static_assert(vec1 == vec_ull_value / vec_ll_value);

			constexpr auto vec2 = vec_ull / vec_ll_value;
			static_assert(vec2 == vec_ull_value / vec_ll_value);

			constexpr auto vec3 = vec_ull_value / vec_ll;
			static_assert(vec3 == vec_ull_value / vec_ll_value);
		}
		{
			constexpr auto vec1 = vec_ull % vec_ll;
			static_assert(vec1 == vec_ull_value % vec_ll_value);

			constexpr auto vec2 = vec_ull % vec_ll_value;
			static_assert(vec2 == vec_ull_value % vec_ll_value);

			constexpr auto vec3 = vec_ull_value % vec_ll;
			static_assert(vec3 == vec_ull_value % vec_ll_value);
		}
		{
			constexpr auto vec1 = vec_ull & vec_ll;
			static_assert(vec1 == (vec_ull_value & vec_ll_value));

			constexpr auto vec2 = vec_ull & vec_ll_value;
			static_assert(vec2 == (vec_ull_value & vec_ll_value));

			constexpr auto vec3 = vec_ull_value & vec_ll;
			static_assert(vec3 == (vec_ull_value & vec_ll_value));
		}
		{
			constexpr auto vec1 = vec_ull | vec_ll;
			static_assert(vec1 == (vec_ull_value | vec_ll_value));

			constexpr auto vec2 = vec_ull | vec_ll_value;
			static_assert(vec2 == (vec_ull_value | vec_ll_value));

			constexpr auto vec3 = vec_ull_value | vec_ll;
			static_assert(vec3 == (vec_ull_value | vec_ll_value));
		}
		{
			constexpr auto vec1 = vec_ull ^ vec_ll;
			static_assert(vec1 == (vec_ull_value ^ vec_ll_value));

			constexpr auto vec2 = vec_ull ^ vec_ll_value;
			static_assert(vec2 == (vec_ull_value ^ vec_ll_value));

			constexpr auto vec3 = vec_ull_value ^ vec_ll;
			static_assert(vec3 == (vec_ull_value ^ vec_ll_value));
		}
		{
			constexpr auto vec = vec_ull << 12;
			static_assert(vec == (vec_ull_value << 12));
		}
		{
			constexpr auto vec = vec_ll >> 15;
			static_assert(vec == (vec_ll_value >>15));
		}
	}
	{
		constexpr double vec_d_value = 3.1415926;
		constexpr unsigned int vec_u_value = 1010101;

		constexpr vector1<double> vec_d{ vec_d_value };
		constexpr vector1<unsigned int> vec_u{ vec_u_value };

		{
			constexpr auto vec1 = vec_d + vec_u;
			static_assert(vec1 == vec_d_value + vec_u_value);

			constexpr auto vec2 = vec_d + vec_u_value;
			static_assert(vec2 == vec_d_value + vec_u_value);

			constexpr auto vec3 = vec_d_value + vec_u;
			static_assert(vec3 == vec_d_value + vec_u_value);
		}
		{
			constexpr auto vec1 = vec_d - vec_u;
			static_assert(vec1 == vec_d_value - vec_u_value);

			constexpr auto vec2 = vec_d - vec_u_value;
			static_assert(vec2 == vec_d_value - vec_u_value);

			constexpr auto vec3 = vec_d_value - vec_u;
			static_assert(vec3 == vec_d_value - vec_u_value);
		}
		{
			constexpr auto vec1 = vec_d * vec_u;
			static_assert(vec1 == vec_d_value * vec_u_value);

			constexpr auto vec2 = vec_d * vec_u_value;
			static_assert(vec2 == vec_d_value * vec_u_value);

			constexpr auto vec3 = vec_d_value * vec_u;
			static_assert(vec3 == vec_d_value * vec_u_value);
		}
		{
			constexpr auto vec1 = vec_d / vec_u;
			static_assert(vec1 == vec_d_value / vec_u_value);

			constexpr auto vec2 = vec_d / vec_u_value;
			static_assert(vec2 == vec_d_value / vec_u_value);

			constexpr auto vec3 = vec_d_value / vec_u;
			static_assert(vec3 == vec_d_value / vec_u_value);
		}
		{
			constexpr vector1<double> v{ -123.456 };
			static_assert((v % -100)[0] <= -23.456 + 0.00000000000001);
			static_assert((v % -100)[0] >= -23.456 - 0.00000000000001);
			
			constexpr auto vec1 = vec_d % vec_u;
			static_assert(vec1 == 3.1415926);

			constexpr auto vec2 = vec_d % vec_u_value;
			static_assert(vec2 == 3.1415926);

			constexpr auto vec3 = vec_d_value % vec_u;
			static_assert(vec3 == 3.1415926);
		}
		{
			constexpr auto vec1 = vec_d & vec_u;
			static_assert(vec1 == (vec_d_value));

			constexpr auto vec2 = vec_d & vec_u_value;
			static_assert(vec2 == (vec_d_value));

			constexpr auto vec3 = vec_d_value & vec_u;
			static_assert(vec3 == (vec_d_value));
		}
		{
			constexpr auto vec1 = vec_d | vec_u;
			static_assert(vec1 == (vec_d_value));

			constexpr auto vec2 = vec_d | vec_u_value;
			static_assert(vec2 == (vec_d_value));

			constexpr auto vec3 = vec_d_value | vec_u;
			static_assert(vec3 == (vec_d_value));
		}
		{
			constexpr auto vec1 = vec_d ^ vec_u;
			static_assert(vec1 == (vec_d_value));

			constexpr auto vec2 = vec_d ^ vec_u_value;
			static_assert(vec2 == (vec_d_value));

			constexpr auto vec3 = vec_d_value ^ vec_u;
			static_assert(vec3 == (vec_d_value));
		}
		{
			constexpr auto pow = [](auto value, int p) constexpr -> auto
			{
				auto ret = value;
				while (--p)
				{
					ret *= value;
				}
				return ret;
			};
			
			constexpr auto vec = vec_d << 12;
			static_assert(vec == vec_d_value * pow(2, 12));
		}
		{
			constexpr auto vec = vec_u >> 15;
			static_assert(vec == (vec_u_value >> 15));
		}
	}
}
