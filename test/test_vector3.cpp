#include <gtest/gtest.h>

#include "../src/math/vector3.hpp"
#include "../src/math/vector2.hpp"
#include "../src/math/vector1.hpp"

TEST(TestVector3, TestBasic)
{
	using namespace gal::test;

	static_assert(vector3<int>::data_size == 3);
	static_assert(vector3<int>::data_index == 2);
	static_assert(vector3<int>::size() == 3);

	{
		// default
		constexpr vector3<int> vec{};

		static_assert(vec[0] == 0);
		static_assert(vec[1] == 0);
		static_assert(vec[2] == 0);
	}
	{
		constexpr vector1<float>  vec_f{3.14f};
		constexpr vector2<double> vec_d{3.14, 6.28};

		constexpr vector3<unsigned int> vec_u{10};
		constexpr vector3<int>          vec_i1{10};
		constexpr vector3<int>          vec_i2{10, 20, 30};
		constexpr vector3<int>          vec_i3{-10, -20, -30};
		constexpr vector3<double>       vec_d0{vec_f};
		constexpr vector3<double>       vec_d1{vec_f, vec_d};
		constexpr vector3<double>       vec_d2{vec_d, vec_f};
		constexpr vector3<double>       vec_d3{vec_d0};
		constexpr vector3<double>       vec_d4{vec_d1, vec_f};
		constexpr vector3<double>       vec_d5{vec_f, vec_d2};
		constexpr vector3<double>       vec_d6{vec_d1, vec_d};
		constexpr vector3<double>       vec_d7{vec_d, vec_d2};
		constexpr vector3<double>       vec_d8{vec_f, vec_d, vec_d3};
		constexpr vector3<double>       vec_d9{vec_f, vec_d4, vec_d};
		constexpr vector3<double>       vec_d10{vec_d5, vec_f, vec_d};

		static_assert(vec_u[0] == 10);
		static_assert(vec_u[1] == 10);
		static_assert(vec_u[2] == 10);
		static_assert((-vec_u)[0] == -10);
		static_assert((-vec_u)[1] == -10);
		static_assert((-vec_u)[2] == -10);

		static_assert(vec_i1[0] == 10);
		static_assert(vec_i1[1] == 10);
		static_assert(vec_i1[2] == 10);

		static_assert(vec_i2[0] == 10);
		static_assert(vec_i2[1] == 20);
		static_assert(vec_i2[2] == 30);

		static_assert(vec_i3[0] == -10);
		static_assert(vec_i3[1] == -20);
		static_assert(vec_i3[2] == -30);

		static_assert(vec_d0[0] == vec_f[0]);
		static_assert(vec_d0[1] == vec_f[0]);
		static_assert(vec_d0[2] == vec_f[0]);

		static_assert(vec_d1[0] == vec_f[0]);
		static_assert(vec_d1[1] == vec_d[0]);
		static_assert(vec_d1[2] == vec_d[1]);

		static_assert(vec_d2[0] == vec_d[0]);
		static_assert(vec_d2[1] == vec_d[1]);
		static_assert(vec_d2[2] == vec_f[0]);

		static_assert(vec_d3[0] == vec_d0[0]);
		static_assert(vec_d3[1] == vec_d0[1]);
		static_assert(vec_d3[2] == vec_d0[2]);

		static_assert(vec_d4[0] == vec_d1[0]);
		static_assert(vec_d4[1] == vec_d1[1]);
		static_assert(vec_d4[2] == vec_f[0]);

		static_assert(vec_d5[0] == vec_f[0]);
		static_assert(vec_d5[1] == vec_d2[0]);
		static_assert(vec_d5[2] == vec_d2[1]);

		static_assert(vec_d6[0] == vec_d1[0]);
		static_assert(vec_d6[1] == vec_d[0]);
		static_assert(vec_d6[2] == vec_d[1]);

		static_assert(vec_d7[0] == vec_d[0]);
		static_assert(vec_d7[1] == vec_d[1]);
		static_assert(vec_d7[2] == vec_d2[0]);

		static_assert(vec_d8[0] == vec_f[0]);
		static_assert(vec_d8[1] == vec_d[0]);
		static_assert(vec_d8[2] == vec_d3[0]);

		static_assert(vec_d9[0] == vec_f[0]);
		static_assert(vec_d9[1] == vec_d4[0]);
		static_assert(vec_d9[2] == vec_d[0]);

		static_assert(vec_d10[0] == vec_d5[0]);
		static_assert(vec_d10[1] == vec_f[0]);
		static_assert(vec_d10[2] == vec_d[0]);
	}
}

TEST(TestVector3, TestSelfOperator)
{
	using namespace gal::test;

	{
		constexpr vector1<double> vec0{3.14};
		constexpr vector2<int>    vec1{42, 1};
		constexpr vector3<long>   vec2{100, 200, 300};
		vector3<unsigned long>    vec3{123, 456, 789};

		vec3 += vec0;
		ASSERT_EQ(vec3[0], 123 + vec0);
		ASSERT_EQ(vec3[1], 456 + vec0);
		ASSERT_EQ(vec3[2], 789 + vec0);

		vec3 -= vec2;
		ASSERT_EQ(vec3[0], 126 - vec2[0]);
		ASSERT_EQ(vec3[1], 459 - vec2[1]);
		ASSERT_EQ(vec3[2], 792 - vec2[2]);

		vec3 *= vec0 + vec2;
		ASSERT_EQ(vec3[0], 26 * 103);
		ASSERT_EQ(vec3[1], 259 * 203);
		ASSERT_EQ(vec3[2], 492 * 303);

		vec3 /= vec1[0] + vec2;
		ASSERT_EQ(vec3[0], 2678 / 142);
		ASSERT_EQ(vec3[1], 52577 / 242);
		ASSERT_EQ(vec3[2], 149076 / 342);

		vec3 %= vec2;
		ASSERT_EQ(vec3[0], 18 % 100);
		ASSERT_EQ(vec3[1], 217 % 200);
		ASSERT_EQ(vec3[2], 435 % 300);

		vec3 &= (vec0 + vec1)[1];
		ASSERT_EQ(vec3[0], 18 & 4);
		ASSERT_EQ(vec3[1], 17 & 4);
		ASSERT_EQ(vec3[2], 135 & 4);

		vec3 |= vec0 + vec2;
		ASSERT_EQ(vec3[0], 0 | 103);
		ASSERT_EQ(vec3[1], 0 | 203);
		ASSERT_EQ(vec3[2], 4 | 303);

		vec3 ^= 01010101; // 266305
		ASSERT_EQ(vec3[0], 103 ^ 01010101);
		ASSERT_EQ(vec3[0], 266278);
		ASSERT_EQ(vec3[1], 203 ^ 01010101);
		ASSERT_EQ(vec3[1], 266378);
		ASSERT_EQ(vec3[2], 303 ^ 01010101);
		ASSERT_EQ(vec3[2], 266606);

		vec3 >>= 4;
		ASSERT_EQ(vec3[0], 266278 >> 4);
		ASSERT_EQ(vec3[1], 266378 >> 4);
		ASSERT_EQ(vec3[2], 266606 >> 4);

		vec3 <<= vec0;
		ASSERT_EQ(vec3[0], (266278 >> 4) << 3);
		ASSERT_EQ(vec3[1], (266378 >> 4) << 3);
		ASSERT_EQ(vec3[2], (266606 >> 4) << 3);
	}
}

TEST(TestVector3, TestOperator)
{
	using namespace gal::test;

	{
		constexpr double vec_d_value1 = 123.456;
		constexpr double vec_d_value2 = 1234.56;
		constexpr double vec_d_value3 = 12345.6;
		constexpr float  vec_f_value1 = 12.3f;
		constexpr float  vec_f_value2 = 1.23f;
		constexpr float  vec_f_value3 = 0.123f;

		constexpr vector3<double> vec_d{vec_d_value1, vec_d_value2, vec_d_value3};
		constexpr vector3<float>  vec_f{vec_f_value1, vec_f_value2, vec_f_value3};

		{
			constexpr auto vec1 = vec_d + vec_f;
			static_assert(vec1[0] == vec_d_value1 + vec_f_value1);
			static_assert(vec1[1] == vec_d_value2 + vec_f_value2);
			static_assert(vec1[2] == vec_d_value3 + vec_f_value3);

			constexpr auto vec2 = vec_d + vec_f_value1;
			static_assert(vec2[0] == vec_d_value1 + vec_f_value1);
			static_assert(vec2[1] == vec_d_value2 + vec_f_value1);
			static_assert(vec2[2] == vec_d_value3 + vec_f_value1);

			constexpr auto vec3 = vec_d_value1 + vec_f;
			static_assert(vec3[0] == vec_d_value1 + vec_f_value1);
			static_assert(vec3[1] == vec_d_value1 + vec_f_value2);
			static_assert(vec3[2] == vec_d_value1 + vec_f_value3);
		}
		{
			constexpr auto vec1 = vec_d - vec_f;
			static_assert(vec1[0] == vec_d_value1 - vec_f_value1);
			static_assert(vec1[1] == vec_d_value2 - vec_f_value2);
			static_assert(vec1[2] == vec_d_value3 - vec_f_value3);

			constexpr auto vec2 = vec_d - vec_f_value1;
			static_assert(vec2[0] == vec_d_value1 - vec_f_value1);
			static_assert(vec2[1] == vec_d_value2 - vec_f_value1);
			static_assert(vec2[2] == vec_d_value3 - vec_f_value1);

			constexpr auto vec3 = vec_d_value1 - vec_f;
			static_assert(vec3[0] == vec_d_value1 - vec_f_value1);
			static_assert(vec3[1] == vec_d_value1 - vec_f_value2);
			static_assert(vec3[2] == vec_d_value1 - vec_f_value3);
		}
		{
			constexpr auto vec1 = vec_d * vec_f;
			static_assert(vec1[0] == vec_d_value1 * vec_f_value1);
			static_assert(vec1[1] == vec_d_value2 * vec_f_value2);
			static_assert(vec1[2] == vec_d_value3 * vec_f_value3);

			constexpr auto vec2 = vec_d * vec_f_value1;
			static_assert(vec2[0] == vec_d_value1 * vec_f_value1);
			static_assert(vec2[1] == vec_d_value2 * vec_f_value1);
			static_assert(vec2[2] == vec_d_value3 * vec_f_value1);

			constexpr auto vec3 = vec_d_value1 * vec_f;
			static_assert(vec3[0] == vec_d_value1 * vec_f_value1);
			static_assert(vec3[1] == vec_d_value1 * vec_f_value2);
			static_assert(vec3[2] == vec_d_value1 * vec_f_value3);
		}
		{
			constexpr auto vec1 = vec_d / vec_f;
			static_assert(vec1[0] == vec_d_value1 / vec_f_value1);
			static_assert(vec1[1] == vec_d_value2 / vec_f_value2);
			static_assert(vec1[2] == vec_d_value3 / vec_f_value3);

			constexpr auto vec2 = vec_d / vec_f_value1;
			static_assert(vec2[0] == vec_d_value1 / vec_f_value1);
			static_assert(vec2[1] == vec_d_value2 / vec_f_value1);
			static_assert(vec2[2] == vec_d_value3 / vec_f_value1);

			constexpr auto vec3 = vec_d_value1 / vec_f;
			static_assert(vec3[0] == vec_d_value1 / vec_f_value1);
			static_assert(vec3[1] == vec_d_value1 / vec_f_value2);
			static_assert(vec3[2] == vec_d_value1 / vec_f_value3);
		}
		{
			constexpr auto f = []<typename T>(T v, auto s) -> auto
			{
				return static_cast<std::conditional_t<
					(sizeof(T) > sizeof(std::uint32_t)),
					std::uint64_t,
					std::uint32_t>>(v / static_cast<T>(s));
			};
			
			constexpr auto vec1 = vec_d % vec_f;
			static_assert(f(vec_d_value1, vec_f_value1) == 10);
			static_assert(f(vec_d_value2, vec_f_value2) == 1003);
			static_assert(f(vec_d_value3, vec_f_value3) == 100370);

			// EXPECT_DOUBLE_EQ(vec1[0], vec_d_value1 - vec_f_value1 * 10);
			// EXPECT_DOUBLE_EQ(vec1[1], vec_d_value2 - vec_f_value2 * 1003);
			// EXPECT_DOUBLE_EQ(vec1[2], vec_d_value3 - vec_f_value3 * 100370);

			constexpr auto vec2 = vec_d % vec_f_value1;
			static_assert(f(vec_d_value1, vec_f_value1) == 10);
			static_assert(f(vec_d_value2, vec_f_value1) == 100);
			static_assert(f(vec_d_value3, vec_f_value1) == 1003);
			// EXPECT_DOUBLE_EQ(vec2[0], vec_d_value1 - vec_f_value1 * 10);
			// EXPECT_DOUBLE_EQ(vec2[1], vec_d_value2 - vec_f_value1 * 100);
			// EXPECT_DOUBLE_EQ(vec2[2], vec_d_value3 - vec_f_value1 * 1003);
			
			constexpr auto vec3 = vec_d_value1 % vec_f;
			static_assert(f(vec_d_value1, vec_f_value1) == 10);
			static_assert(f(vec_d_value1, vec_f_value2) == 100);
			static_assert(f(vec_d_value1, vec_f_value3) == 1003);
			// EXPECT_DOUBLE_EQ(vec3[0], vec_d_value1 - vec_f_value1 * 10);
			// EXPECT_DOUBLE_EQ(vec3[1], vec_d_value1 - vec_f_value2 * 100);
			// EXPECT_DOUBLE_EQ(vec3[2], vec_d_value1 - vec_f_value3 * 1003);
		}
		{
			constexpr auto vec1 = vec_d & vec_f;
			static_assert(vec1[0] == vec_d_value1);
			static_assert(vec1[1] == vec_d_value2);
			static_assert(vec1[2] == vec_d_value3);

			constexpr auto vec2 = vec_d & vec_f_value1;
			static_assert(vec2[0] == vec_d_value1);
			static_assert(vec2[1] == vec_d_value2);
			static_assert(vec2[2] == vec_d_value3);

			constexpr auto vec3 = vec_d_value1 & vec_f;
			static_assert(vec3[0] == vec_d_value1);
			static_assert(vec3[1] == vec_d_value1);
			static_assert(vec3[2] == vec_d_value1);
		}
		{
			constexpr auto vec1 = vec_d | vec_f;
			static_assert(vec1[0] == vec_d_value1);
			static_assert(vec1[1] == vec_d_value2);
			static_assert(vec1[2] == vec_d_value3);

			constexpr auto vec2 = vec_d | vec_f_value1;
			static_assert(vec2[0] == vec_d_value1);
			static_assert(vec2[1] == vec_d_value2);
			static_assert(vec2[2] == vec_d_value3);

			constexpr auto vec3 = vec_d_value1 | vec_f;
			static_assert(vec3[0] == vec_d_value1);
			static_assert(vec3[1] == vec_d_value1);
			static_assert(vec3[2] == vec_d_value1);
		}
		{
			constexpr auto vec1 = vec_d ^ vec_f;
			static_assert(vec1[0] == vec_d_value1);
			static_assert(vec1[1] == vec_d_value2);
			static_assert(vec1[2] == vec_d_value3);

			constexpr auto vec2 = vec_d ^ vec_f_value1;
			static_assert(vec2[0] == vec_d_value1);
			static_assert(vec2[1] == vec_d_value2);
			static_assert(vec2[2] == vec_d_value3);

			constexpr auto vec3 = vec_d_value1 ^ vec_f;
			static_assert(vec3[0] == vec_d_value1);
			static_assert(vec3[1] == vec_d_value1);
			static_assert(vec3[2] == vec_d_value1);
		}
		{
			constexpr auto f = []<typename T>(T value, auto scalar) constexpr -> T
			{
				while (true)
				{
					scalar -= 1;
					value *= 2;
					if (scalar < 1)
					{
						break;
					}
				}
				return value;
			};

			constexpr vector3<double> vec{4, 5, 6};

			constexpr auto vec1 = vec_d << vec;
			static_assert(vec1[0] == f(vec_d_value1, vec[0]));
			static_assert(vec1[1] == f(vec_d_value2, vec[1]));
			static_assert(vec1[2] == f(vec_d_value3, vec[2]));

			constexpr auto vec2 = vec_d << vec;
			static_assert(vec2[0] == f(vec_d_value1, vec[0]));
			static_assert(vec2[1] == f(vec_d_value2, vec[1]));
			static_assert(vec2[2] == f(vec_d_value3, vec[2]));

			constexpr auto vec3 = vec_d_value1 << vec;
			static_assert(vec3[0] == f(vec_d_value1, vec[0]));
			static_assert(vec3[1] == f(vec_d_value1, vec[1]));
			static_assert(vec3[2] == f(vec_d_value1, vec[2]));
		}
		{
			constexpr auto f = []<typename T>(T value, auto scalar) constexpr -> T
			{
				while (true)
				{
					scalar -= 1;
					value /= 2;
					if (scalar < 1)
					{
						break;
					}
				}
				return value;
			};

			constexpr vector3<double> vec{4, 5, 6};

			constexpr auto vec1 = vec_d >> vec;
			static_assert(vec1[0] == f(vec_d_value1, vec[0]));
			static_assert(vec1[1] == f(vec_d_value2, vec[1]));
			static_assert(vec1[2] == f(vec_d_value3, vec[2]));

			constexpr auto vec2 = vec_d >> vec;
			static_assert(vec2[0] == f(vec_d_value1, vec[0]));
			static_assert(vec2[1] == f(vec_d_value2, vec[1]));
			static_assert(vec2[2] == f(vec_d_value3, vec[2]));

			constexpr auto vec3 = vec_d_value1 >> vec;
			static_assert(vec3[0] == f(vec_d_value1, vec[0]));
			static_assert(vec3[1] == f(vec_d_value1, vec[1]));
			static_assert(vec3[2] == f(vec_d_value1, vec[2]));
		}
	}
}
