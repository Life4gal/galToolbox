#include <gtest/gtest.h>

#include "../src/math/vector.hpp"

TEST(TestVector, TestBasic)
{
	using namespace gal::test::math;
	using namespace gal::test::utils;

	static_assert(vector1<int>::data_size == 1);
	static_assert(vector2<int>::data_size == 2);
	static_assert(vector3<int>::data_size == 3);

	{
		constexpr vector1<int> vec{};
		static_assert(vec[0] == 0);
	}
	{
		constexpr auto         tuple1 = std::make_tuple(1, 2, 3, 4, 5, 6, 7, 8, 9);
		constexpr vector3<int> vec1{ tuple1, std::index_sequence<0, 2, 4>{} };
		static_assert(vec1[0] == 1);
		static_assert(vec1[1] == 3);
		static_assert(vec1[2] == 5);

		constexpr auto tuple2 = tuple_maker::duplicate<5>(3);
		static_assert(std::get<0>(tuple2) == 3);
		static_assert(std::get<1>(tuple2) == 3);
		static_assert(std::get<2>(tuple2) == 3);
		static_assert(std::get<3>(tuple2) == 3);
		static_assert(std::get<4>(tuple2) == 3);

		constexpr vector3<int> vec2{ tuple2, std::index_sequence<0, 2, 4>{} };
		static_assert(vec2[0] == 3);
		static_assert(vec2[1] == 3);
		static_assert(vec2[2] == 3);

		constexpr auto tuple3 = tuple_maker::duplicate<5>(vector1<int>{456});
		static_assert(std::get<0>(tuple3) == 456);
		static_assert(std::get<1>(tuple3) == 456);
		static_assert(std::get<2>(tuple3) == 456);
		static_assert(std::get<3>(tuple3) == 456);
		static_assert(std::get<4>(tuple3) == 456);

		constexpr auto tuple4 = tuple_maker::duplicate<5>(vector2<int>{123, 456});
		static_assert(std::get<0>(tuple4) == 123);
		static_assert(std::get<1>(tuple4) == 456);
		static_assert(std::get<2>(tuple4) == 123);
		static_assert(std::get<3>(tuple4) == 456);
		static_assert(std::get<4>(tuple4) == 123);

		constexpr auto tuple5 = tuple_maker::duplicate<5>(vec1);
		static_assert(std::get<0>(tuple5) == 1);
		static_assert(std::get<1>(tuple5) == 3);
		static_assert(std::get<2>(tuple5) == 5);
		static_assert(std::get<3>(tuple5) == 1);
		static_assert(std::get<4>(tuple5) == 3);

		constexpr auto tuple6 = tuple_maker::duplicate<5>(vector4<int>{12, 34, 56, 78});
		static_assert(std::get<0>(tuple6) == 12);
		static_assert(std::get<1>(tuple6) == 34);
		static_assert(std::get<2>(tuple6) == 56);
		static_assert(std::get<3>(tuple6) == 78);
		static_assert(std::get<4>(tuple6) == 12);

		constexpr auto tuple7 = tuple_maker::duplicate<5>(vector<int, 5>{12, 34, 56, 78, 90});
		static_assert(std::get<0>(tuple7) == 12);
		static_assert(std::get<1>(tuple7) == 34);
		static_assert(std::get<2>(tuple7) == 56);
		static_assert(std::get<3>(tuple7) == 78);
		static_assert(std::get<4>(tuple7) == 90);

		constexpr auto tuple8 = tuple_maker::duplicate<5>(vector<int, 6>{12, 34, 56, 78, 90, 100});
		static_assert(std::get<0>(tuple8) == 12);
		static_assert(std::get<1>(tuple8) == 34);
		static_assert(std::get<2>(tuple8) == 56);
		static_assert(std::get<3>(tuple8) == 78);
		static_assert(std::get<4>(tuple8) == 90);
		static_assert(std::get<5>(tuple8) == 100);
	}
	{
		constexpr vector1<int>      vec1{ 123 };
		constexpr vector2<long>     vec2{ 123, 456 };
		constexpr vector3<unsigned> vec3{ 123, 456, 789 };

		static_assert(vec1[0] == 123);

		static_assert(vec2[0] == 123);
		static_assert(vec2[1] == 456);

		static_assert(vec3[0] == 123);
		static_assert(vec3[1] == 456);
		static_assert(vec3[2] == 789);

		constexpr vector3<int> vec4{ vec1, vec2, vec3, vec1, vec2, vec3 };
		static_assert(decltype(vec4)::size() == 3);
		static_assert(vec4[0] == 123);
		static_assert(vec4[1] == 123);
		static_assert(vec4[2] == 456);

		constexpr vector2<unsigned> vec5{ vec1 };
		static_assert(vec5[0] == 123);
		static_assert(vec5[1] == 0);

		constexpr vector3<long> vec6{ vec4 };
		static_assert(vec6[0] == 123);
		static_assert(vec6[1] == 123);
		static_assert(vec6[2] == 456);
	}
}

TEST(TestVector, TestSelfOperator)
{
	using namespace gal::test::math;

	{
		constexpr vector1<double> vec0{3.14};
		constexpr vector2<int>    vec1{42, 1};
		constexpr vector3<long>   vec2{100, 200, 300};
		vector3<unsigned long>    vec3{123, 456, 789};
		auto vec3_view = vec3.to_view();

		vec3_view += vec0;
		ASSERT_EQ(vec3[0], 123 + vec0.to_view());
		ASSERT_EQ(vec3[1], 456 + vec0.to_view());
		ASSERT_EQ(vec3[2], 789 + vec0.to_view());

		vec3_view -= vec2;
		ASSERT_EQ(vec3[0], 126 - vec2[0]);
		ASSERT_EQ(vec3[1], 459 - vec2[1]);
		ASSERT_EQ(vec3[2], 792 - vec2[2]);

		vec3_view *= vec0.to_view() + vec2;
		ASSERT_EQ(vec3[0], 26 * static_cast<decltype(vec3)::value_type>(103.14));
		ASSERT_EQ(vec3[1], 259 * static_cast<decltype(vec3)::value_type>(103.14));
		ASSERT_EQ(vec3[2], 492 * static_cast<decltype(vec3)::value_type>(103.14));

		vec3_view /= vec1[0] + vec2.to_view();
		ASSERT_EQ(vec3[0], 2678 / 142);
		ASSERT_EQ(vec3[1], 26677 / 242);
		ASSERT_EQ(vec3[2], 50676 / 342);

		vec3_view %= vec2;
		ASSERT_EQ(vec3[0], 18 % 100);
		ASSERT_EQ(vec3[1], 110 % 200);
		ASSERT_EQ(vec3[2], 148 % 300);

		vec3_view &= vec0.to_view() + vec1;
		ASSERT_EQ(vec3[0], 18 & 45);
		ASSERT_EQ(vec3[1], 110 & 45);
		ASSERT_EQ(vec3[2], 148 & 45);

		vec3_view |= vec0.to_view() + vec2;
		ASSERT_EQ(vec3[0], 0 | 103);
		ASSERT_EQ(vec3[1], 44 | 103);
		ASSERT_EQ(vec3[2], 4 | 103);

		vec3_view ^= 01010101; // 266305
		ASSERT_EQ(vec3[0], 103 ^ 01010101);
		ASSERT_EQ(vec3[0], 266278);
		ASSERT_EQ(vec3[1], 111 ^ 01010101);
		ASSERT_EQ(vec3[1], 266286);
		ASSERT_EQ(vec3[2], 103 ^ 01010101);
		ASSERT_EQ(vec3[2], 266278);

		vec3_view >>= 4;
		ASSERT_EQ(vec3[0], 266278 >> 4);
		ASSERT_EQ(vec3[1], 266286 >> 4);
		ASSERT_EQ(vec3[2], 266278 >> 4);

		vec3_view <<= vec0;
		ASSERT_EQ(vec3[0], (266278 >> 4) << 3);
		ASSERT_EQ(vec3[1], (266286 >> 4) << 3);
		ASSERT_EQ(vec3[2], (266278 >> 4) << 3);
	}
}

TEST(TestVector, TestOperator)
{
	using namespace gal::test::math;

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
			constexpr auto vec1 = vec_d.to_view() + vec_f;
			static_assert(vec1[0] == vec_d_value1 + vec_f_value1);
			static_assert(vec1[1] == vec_d_value2 + vec_f_value2);
			static_assert(vec1[2] == vec_d_value3 + vec_f_value3);

			constexpr auto vec2 = vec_d.to_view() + vec_f_value1;
			static_assert(vec2[0] == vec_d_value1 + vec_f_value1);
			static_assert(vec2[1] == vec_d_value2 + vec_f_value1);
			static_assert(vec2[2] == vec_d_value3 + vec_f_value1);

			constexpr auto vec3 = vec_d_value1 + vec_f.to_view();
			static_assert(vec3[0] == vec_d_value1 + vec_f_value1);
			static_assert(vec3[1] == vec_d_value1 + vec_f_value2);
			static_assert(vec3[2] == vec_d_value1 + vec_f_value3);
		}
		{
			constexpr auto vec1 = vec_d.to_view() - vec_f;
			static_assert(vec1[0] == vec_d_value1 - vec_f_value1);
			static_assert(vec1[1] == vec_d_value2 - vec_f_value2);
			static_assert(vec1[2] == vec_d_value3 - vec_f_value3);

			constexpr auto vec2 = vec_d.to_view() - vec_f_value1;
			static_assert(vec2[0] == vec_d_value1 - vec_f_value1);
			static_assert(vec2[1] == vec_d_value2 - vec_f_value1);
			static_assert(vec2[2] == vec_d_value3 - vec_f_value1);

			constexpr auto vec3 = vec_d_value1 - vec_f.to_view();
			static_assert(vec3[0] == vec_d_value1 - vec_f_value1);
			static_assert(vec3[1] == vec_d_value1 - vec_f_value2);
			static_assert(vec3[2] == vec_d_value1 - vec_f_value3);
		}
		{
			constexpr auto vec1 = vec_d * vec_f.to_view();
			static_assert(vec1[0] == vec_d_value1 * vec_f_value1);
			static_assert(vec1[1] == vec_d_value2 * vec_f_value2);
			static_assert(vec1[2] == vec_d_value3 * vec_f_value3);

			constexpr auto vec2 = vec_d.to_view() * vec_f_value1;
			static_assert(vec2[0] == vec_d_value1 * vec_f_value1);
			static_assert(vec2[1] == vec_d_value2 * vec_f_value1);
			static_assert(vec2[2] == vec_d_value3 * vec_f_value1);

			constexpr auto vec3 = vec_d_value1 * vec_f.to_view();
			static_assert(vec3[0] == vec_d_value1 * vec_f_value1);
			static_assert(vec3[1] == vec_d_value1 * vec_f_value2);
			static_assert(vec3[2] == vec_d_value1 * vec_f_value3);
		}
		{
			constexpr auto vec1 = vec_d.to_view() / vec_f;
			static_assert(vec1[0] == vec_d_value1 / vec_f_value1);
			static_assert(vec1[1] == vec_d_value2 / vec_f_value2);
			static_assert(vec1[2] == vec_d_value3 / vec_f_value3);

			constexpr auto vec2 = vec_d.to_view() / vec_f_value1;
			static_assert(vec2[0] == vec_d_value1 / vec_f_value1);
			static_assert(vec2[1] == vec_d_value2 / vec_f_value1);
			static_assert(vec2[2] == vec_d_value3 / vec_f_value1);

			constexpr auto vec3 = vec_d_value1 / vec_f.to_view();
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

			constexpr auto vec1 = vec_d.to_view() % vec_f;
			static_assert(f(vec_d_value1, vec_f_value1) == 10);
			static_assert(f(vec_d_value2, vec_f_value2) == 1003);
			static_assert(f(vec_d_value3, vec_f_value3) == 100370);

			// EXPECT_DOUBLE_EQ(vec1[0], vec_d_value1 - vec_f_value1 * 10);
			// EXPECT_DOUBLE_EQ(vec1[1], vec_d_value2 - vec_f_value2 * 1003);
			// EXPECT_DOUBLE_EQ(vec1[2], vec_d_value3 - vec_f_value3 * 100370);

			constexpr auto vec2 = vec_d.to_view() % vec_f_value1;
			static_assert(f(vec_d_value1, vec_f_value1) == 10);
			static_assert(f(vec_d_value2, vec_f_value1) == 100);
			static_assert(f(vec_d_value3, vec_f_value1) == 1003);
			// EXPECT_DOUBLE_EQ(vec2[0], vec_d_value1 - vec_f_value1 * 10);
			// EXPECT_DOUBLE_EQ(vec2[1], vec_d_value2 - vec_f_value1 * 100);
			// EXPECT_DOUBLE_EQ(vec2[2], vec_d_value3 - vec_f_value1 * 1003);

			constexpr auto vec3 = vec_d_value1 % vec_f.to_view();
			static_assert(f(vec_d_value1, vec_f_value1) == 10);
			static_assert(f(vec_d_value1, vec_f_value2) == 100);
			static_assert(f(vec_d_value1, vec_f_value3) == 1003);
			// EXPECT_DOUBLE_EQ(vec3[0], vec_d_value1 - vec_f_value1 * 10);
			// EXPECT_DOUBLE_EQ(vec3[1], vec_d_value1 - vec_f_value2 * 100);
			// EXPECT_DOUBLE_EQ(vec3[2], vec_d_value1 - vec_f_value3 * 1003);
		}
		{
			// no longer allow bit operations on floating point numbers
			// constexpr auto vec1 = vec_d & vec_f.to_view();
			// static_assert(vec1[0] == vec_d_value1);
			// static_assert(vec1[1] == vec_d_value2);
			// static_assert(vec1[2] == vec_d_value3);
			//
			// constexpr auto vec2 = vec_d.to_view() & vec_f_value1;
			// static_assert(vec2[0] == vec_d_value1);
			// static_assert(vec2[1] == vec_d_value2);
			// static_assert(vec2[2] == vec_d_value3);
			//
			// constexpr auto vec3 = vec_d_value1 & vec_f.to_view();
			// static_assert(vec3[0] == vec_d_value1);
			// static_assert(vec3[1] == vec_d_value1);
			// static_assert(vec3[2] == vec_d_value1);
		}
		{
			// no longer allow bit operations on floating point numbers
			// constexpr auto vec1 = vec_d.to_view() | vec_f;
			// static_assert(vec1[0] == vec_d_value1);
			// static_assert(vec1[1] == vec_d_value2);
			// static_assert(vec1[2] == vec_d_value3);
			//
			// constexpr auto vec2 = vec_d.to_view() | vec_f_value1;
			// static_assert(vec2[0] == vec_d_value1);
			// static_assert(vec2[1] == vec_d_value2);
			// static_assert(vec2[2] == vec_d_value3);
			//
			// constexpr auto vec3 = vec_d_value1 | vec_f.to_view();
			// static_assert(vec3[0] == vec_d_value1);
			// static_assert(vec3[1] == vec_d_value1);
			// static_assert(vec3[2] == vec_d_value1);
		}
		{
			// no longer allow bit operations on floating point numbers
			// constexpr auto vec1 = vec_d.to_view() ^ vec_f;
			// static_assert(vec1[0] == vec_d_value1);
			// static_assert(vec1[1] == vec_d_value2);
			// static_assert(vec1[2] == vec_d_value3);
			//
			// constexpr auto vec2 = vec_d.to_view() ^ vec_f_value1;
			// static_assert(vec2[0] == vec_d_value1);
			// static_assert(vec2[1] == vec_d_value2);
			// static_assert(vec2[2] == vec_d_value3);
			//
			// constexpr auto vec3 = vec_d_value1 ^ vec_f.to_view();
			// static_assert(vec3[0] == vec_d_value1);
			// static_assert(vec3[1] == vec_d_value1);
			// static_assert(vec3[2] == vec_d_value1);
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

			constexpr auto vec1 = vec_d.to_view() << vec;
			static_assert(vec1[0] == f(vec_d_value1, vec[0]));
			static_assert(vec1[1] == f(vec_d_value2, vec[1]));
			static_assert(vec1[2] == f(vec_d_value3, vec[2]));

			constexpr auto vec2 = vec_d.to_view() << vec;
			static_assert(vec2[0] == f(vec_d_value1, vec[0]));
			static_assert(vec2[1] == f(vec_d_value2, vec[1]));
			static_assert(vec2[2] == f(vec_d_value3, vec[2]));

			constexpr auto vec3 = vec_d_value1 << vec.to_view();
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

			constexpr auto vec1 = vec_d.to_view() >> vec;
			static_assert(vec1[0] == f(vec_d_value1, vec[0]));
			static_assert(vec1[1] == f(vec_d_value2, vec[1]));
			static_assert(vec1[2] == f(vec_d_value3, vec[2]));

			constexpr auto vec2 = vec_d.to_view() >> vec;
			static_assert(vec2[0] == f(vec_d_value1, vec[0]));
			static_assert(vec2[1] == f(vec_d_value2, vec[1]));
			static_assert(vec2[2] == f(vec_d_value3, vec[2]));

			constexpr auto vec3 = vec_d_value1 >> vec.to_view();
			static_assert(vec3[0] == f(vec_d_value1, vec[0]));
			static_assert(vec3[1] == f(vec_d_value1, vec[1]));
			static_assert(vec3[2] == f(vec_d_value1, vec[2]));
		}
	}
}
