#include <gtest/gtest.h>

#include <bit>
#include <compare>
#include <cstdint>

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
		auto to_hex = []<typename T, std::endian endian = std::endian::native>(const T& value, size_t value_size = sizeof(T))->std::string {
			std::string converted = std::is_signed_v<T> ? "-" : "";

			converted.append(value_size * 2 + 1, '\0');
			auto p		= converted.data() + (std::is_signed_v<T> ? sizeof('-') : 0);

			auto buffer = reinterpret_cast<const uint8_t*>(&value);
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
			ASSERT_STREQ(to_hex(n8).c_str(), "12");
			uint16_t n16 = 0x1213;
			ASSERT_STREQ(to_hex(n16).c_str(), "1213");
			uint32_t n32 = 0x1213'1415;
			ASSERT_STREQ(to_hex(n32).c_str(), "12131415");
			uint64_t n64 = 0x1213'1415'1617'1819;
			ASSERT_STREQ(to_hex(n64).c_str(), "1213141516171819");
		}
		{
			int8_t n8 = -128;
			ASSERT_STREQ(to_hex(n8).c_str(), "-80");
			int16_t n16 = -32768;
			ASSERT_STREQ(to_hex(n16).c_str(), "-8000");
			int32_t n32 = -2147483648;
			ASSERT_STREQ(to_hex(n32).c_str(), "-80000000");
			int64_t n64 = -9223372036854775808;// Clang-Tidy: Narrowing conversion from constant value 9223372036854775808 (0x8000000000000000) of type 'unsigned long long' to signed type 'int64_t' (aka 'long') is implementation-defined
			ASSERT_STREQ(to_hex(n64).c_str(), "-8000000000000000");
		}
	}

	TEST(MISC_TEST, SPACESHIP_OPERATOR) {
		struct Area {
			[[nodiscard]] int get_area() const noexcept {
				return x * y;
			}

			std::strong_ordering operator<=>(const Area& other) const {
				if (get_area() == other.get_area() && x <=> other.x == nullptr && y <=> other.y == nullptr) {
					return std::strong_ordering::equal;
				}

				if (x <=> other.x == nullptr) {
					return y <=> other.y;
				}

				return x <=> other.x;
			}

			int x;
			int y;
		};

		Area a1{1, 2};
		Area a2{2, 1};
		Area a3{1, 2};
		Area a4{2, 2};

		std::cout << std::boolalpha;
		std::cout << "a1 == a2 --> " << (a1 <=> a2 == nullptr) << " a1 > a2 --> " << (a1 <=> a2 > nullptr) << " a1 < a2 --> " << (a1 <=> a2 < nullptr) << std::endl;
		std::cout << "a1 == a3 --> " << (a1 <=> a3 == nullptr) << " a1 > a3 --> " << (a1 <=> a2 > nullptr) << " a1 < a3 --> " << (a1 <=> a3 < nullptr) << std::endl;
		std::cout << "a1 == a4 --> " << (a1 <=> a4 == nullptr) << " a1 > a4 --> " << (a1 <=> a4 > nullptr) << " a1 < a4 --> " << (a1 <=> a4 < nullptr) << std::endl;
	}
}// namespace
