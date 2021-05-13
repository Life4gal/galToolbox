#include <gtest/gtest.h>

#include <bit>

namespace {
	TEST(MISC_TEST, BIT_CAST) {
		static_assert(std::bit_cast<std::uint64_t>(19880124.0) == 0x4172'f58b'c000'0000);
		static_assert(std::bit_cast<double>(0x4172'f58b'c000'0000) == 19880124.0);

		static_assert(std::bit_cast<double>(0x3fe9'0000'0000'0000) == 0.781250);
		static_assert(std::bit_cast<std::uint64_t>(0.781250) == 0x3fe9'0000'0000'0000);
	}

	TEST(MISC_TEST, CONSTEVAL) {
		constexpr auto square = []<typename T>(T i) consteval {
			return i * i;
		};

		// nested call cause a error if use lambda, work well if use normal-function
		//		constexpr auto error_square = []<typename T>(T i) consteval
		//		{
		//						return square(square(i));
		//		};
		constexpr auto right = 123;
		std::cout << square(right) << '\n';
		//		std::cout << error_square(right) << '\n';

		//		auto error = 123;
		//		std::cout << square(error) << '\n';
		//		std::cout << error_square(error) << '\n';
	}

	TEST(MISC_TEST, ENDIAN) {
		auto to_hex = []<typename T, std::endian endian = std::endian::native>(const T& value, size_t value_size = sizeof(T)) -> std::string {
			auto		buffer = reinterpret_cast<const uint8_t*>(&value);
			std::string converted(value_size * 2 + 1, 0);
			auto		p = converted.data();

			if constexpr (endian == std::endian::big) {
				for (decltype(value_size) i = 0; i < value_size; ++i) {
					sprintf(p + (i * 2), "%02X", buffer[i]);
				}
			} else {
				for (decltype(value_size) i = 0; i < value_size; ++i) {
					sprintf(p + (i * 2), "%02X", buffer[value_size - i - 1]);
				}
			}

			return converted;
		};

		{
			uint8_t n8 = 0x12;
			ASSERT_STREQ(to_hex(n8).data(), "12");
			uint16_t n16 = 0x1213;
			ASSERT_STREQ(to_hex(n16).data(), "1213");
			uint32_t n32 = 0x1213'1415;
			ASSERT_STREQ(to_hex(n32).data(), "12131415");
			uint64_t n64 = 0x1213'1415'1617'1819;
			ASSERT_STREQ(to_hex(n64).data(), "1213141516171819");
		}
		{
			int8_t n8 = -128;
			std::cout << to_hex(n8) << '\n';
//			ASSERT_TRUE(to_hex(n8) == "")
		}
	}
}// namespace
