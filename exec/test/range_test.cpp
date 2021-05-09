#include <gtest/gtest.h>

#include <algorithm>
#include <optional>
#include <ranges>
#include <vector>

namespace {
	TEST(RANGE_TEST, BASE_TEST) {
		auto values = std::vector{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
		auto even	= [](const auto value) {
			  return value % 2 == 0;
		};
		auto square = [](const auto value) {
			return value * value;
		};

		auto results1 =
				values |
				std::views::filter(even) |
				std::views::reverse |
				std::views::take(4) |
				std::views::reverse;

		{
			decltype(values) copy;
			// todo: better way
			std::ranges::copy(results1, std::back_inserter(copy));
			ASSERT_EQ(copy.size(), 4);
			ASSERT_EQ(copy[0], 4);
			ASSERT_EQ(copy[1], 6);
			ASSERT_EQ(copy[2], 8);
			ASSERT_EQ(copy[3], 10);
		}

		auto results2 =
				values |
				std::views::transform(square) |
				std::views::reverse |
				std::views::take(4) |
				std::views::reverse;

		{
			decltype(values) copy;
			// todo: better way
			std::ranges::copy(results2, std::back_inserter(copy));
			ASSERT_EQ(copy.size(), 4);
			ASSERT_EQ(copy[0], 49);
			ASSERT_EQ(copy[1], 64);
			ASSERT_EQ(copy[2], 81);
			ASSERT_EQ(copy[3], 100);
		}

		//		auto results3 =
		//				values |
		//				std::views::filter(even) |
		//				std::views::transform(square) |
		//				std::views::reverse |
		//				std::views::take(4) |
		//				std::views::reverse;// Error happens on this line.


		auto odd_num1 =
				// 1 -> inf
				std::views::iota(1) |
				// 1 / 4 / 9 / 16 ......
				std::views::transform([](int i) constexpr->int { return i * i; }) |
				// 1 / 9 / 25 ......
				std::views::filter([](int i) constexpr->bool { return i % 2 != 0; }) |
				// 1 ~ 100
				std::views::take_while([](const int i) constexpr->bool { return i <= 100; });

		ASSERT_EQ(std::ranges::count_if(
						  odd_num1,
						  [](int i) constexpr->bool { return true; }),
				  5);
		{
			int count = 0;
			for (auto i = odd_num1.begin(); i != odd_num1.end(); ++i) {
				++count;
			}
			ASSERT_EQ(count, 5);
		}

		constexpr auto odd_num2 =
				// 1 -> inf
				std::views::iota(1) |
				// 1 / 4 / 9 / 16 ......
				std::views::transform([](int i) constexpr->int { return i * i; }) |
				// 1 / 9 / 25 ......
				std::views::filter([](int i) constexpr->bool { return i % 2 != 0; }) |
				// 1 ~ 100
				std::views::take_while([](const int i) constexpr->bool { return i <= 100; });

		// todo: wrong
		/**
			/usr/local/include/c++/11.0.1/bits/ranges_base.h:581:22: the required expression ‘std::ranges::__cust::begin(__t)’ is invalid
			  581 |         ranges::begin(__t);
			      |         ~~~~~~~~~~~~~^~~~~
			/usr/local/include/c++/11.0.1/bits/ranges_base.h:582:20: the required expression ‘std::ranges::__cust::end(__t)’ is invalid
			  582 |         ranges::end(__t);
			      |         ~~~~~~~~~~~^~~~~

			note: `(std::remove_cv_t<decltype(odd_num2)>)odd_num2` instead `odd_num2` is ok
		 */
		/**
			static_assert(std::ranges::count_if(
								  odd_num2,
								  [](int i) constexpr->bool { return true; }) == 5);
		*/
		// no match for ‘operator-’
		//		static_assert((odd_num2.end() - odd_num2.begin()) == 5);
		//		{
		//			int count = 0;
		//			/**
		//				passing ‘const std::ranges::take_while_view<std::ranges::filter_view<std::ranges::transform_view<std::ranges::iota_view<int, std::unreachable_sentinel_t>, {anonymous}::RANGE_TEST_BASE_TEST_Test::TestBody()::<lambda(int)> >, {anonymous}::RANGE_TEST_BASE_TEST_Test::TestBody()::<lambda(int)> >, {anonymous}::RANGE_TEST_BASE_TEST_Test::TestBody()::<lambda(int)> >’ as ‘this’ argument discards qualifiers [-fpermissive]
		//				when call `begin()` and `end()`
		//
		//				note: `((std::remove_cv_t<decltype(odd_num2)>)odd_num2).begin()` and `((std::remove_cv_t<decltype(odd_num2)>)odd_num2).end()` is ok
		//			 */
		//			for (auto i = odd_num2.begin(); i != odd_num2.end(); ++i) {
		//				++count;
		//			}
		//			ASSERT_EQ(count, 5);
		//		}

		// todo: totally wrong!!
		//		auto wrong1 = std::ranges::size(odd_num1);
		//		auto wrong2 = std::ranges::size(odd_num2);
		//		auto wrong3 = odd_num1.end() - odd_num1.begin();
		//		auto wrong4 = odd_num1.begin() - odd_num1.end();
		//		auto wrong5 = odd_num2.end() - odd_num2.begin();
		//		auto wrong6 = odd_num2.begin() - odd_num2.end();
	}

//	// maybe_view defines a view over zero or one objects.
//	template<std::semiregular T>
//	//	requires std::ranges::forward_range<T>
//	struct maybe_view : std::ranges::view_interface<maybe_view<T>> {
//		using value_type = T;
//
//		maybe_view()	 = default;
//		explicit maybe_view(value_type v) : data(std::move(v)) {
//		}
//
//		[[nodiscard]] const value_type* begin() const noexcept {
//			return data.has_value() ? &data.operator*() : nullptr;
//		}
//		[[nodiscard]] const value_type* end() const noexcept {
//			return data.has_value() ? &data.operator*() + 1 : nullptr;
//		}
//
//	private:
//		std::optional<value_type> data;
//	};
//
//	// "yield_if" takes a bool and a value and
//	// returns a view of zero or one elements.
//	constexpr auto yield_if =
//			[]<std::semiregular T>(bool b, T x) {
//				return b ? maybe_view{std::move(x)}
//						 : maybe_view<T>{};
//			};

	TEST(RANGE_TEST, TRIPLES) {
		// triples =[(x, y, z)|z <-[1..], x <-[1..z],y <-[x..z] ,x^2 + y^2 == z^2]
		//		auto triples = std::ranges::for_each(std::views::iota(1), [](int z) {
		//			return std::ranges::for_each(std::views::iota(1, z), [z](int x) {
		//				return std::ranges::for_each(std::views::iota(x, z), [z, x](int y) {
		//					return yield_if(true, std::make_tuple(x, y, z));
		//				});
		//			});
		//		}) ;//| std::views::take(10); //| std::views::take_while([](std::tuple<int, int, int> tri) { return std::get<0>(tri) < 100; });
		//					   |
		//					   std::views::take_while([](std::tuple<int, int, int> tri) { return std::get<0>(tri) < 100; }) | std::ranges::for_each([](std::tuple<int, int, int> tri) {
		//						   const auto& [x, y, z] = tri;
		//						   std::cout << x << ' ' << y << ' ' << z << std::endl;
		//					   });
	}
}// namespace
