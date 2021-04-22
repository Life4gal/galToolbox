/**
 * Inspire form PRNG(https://prng.di.unimi.it/)(http://creativecommons.org/publicdomain/zero/1.0/)
 *
 * If there is no legal license conflict, the file will be distributed under the [see root project license] license.
 */

#ifndef GAL_TOOLBOX_RANDOM_HPP
#define GAL_TOOLBOX_RANDOM_HPP

#include <array>
#include <concepts>
#include <cstdint>
#include <limits>
#include <type_traits>

namespace gal::toolbox::random {
	namespace detail {
		using bits_64_type					= std::uint64_t;
		using bits_32_type					= std::uint32_t;

		constexpr bits_64_type default_seed = 1234567890ULL;

		template<std::unsigned_integral T, size_t N>
		struct type_generator {
			using result_type = std::conditional_t<
					std::is_same_v<T, bits_64_type> || std::numeric_limits<T>::digits == std::numeric_limits<bits_64_type>::digits,
					bits_64_type,
					std::conditional_t<std::is_same_v<T, bits_32_type> || std::numeric_limits<T>::digits == std::numeric_limits<bits_32_type>::digits, bits_32_type, nullptr_t>>;
			using state_type = std::array<result_type, N>;

			static_assert(!std::is_same_v<result_type, nullptr_t>, "Not supported type");
		};

		template<typename T>
		concept is_type_generator = requires {
			typename T::result_type;
			typename T::state_type;
		};

		template<size_t N, size_t Total, std::unsigned_integral T>
		constexpr T rotate_left(T value) noexcept {
			return (value << N) | (value >> (Total - N));
		}

		/**
		 * Output: 64 bits
		 * Period: 2^64
		 * Footprint: 8 bytes
		 * Original implementation: http://prng.di.unimi.it/splitmix64.c
		 */
		template<is_type_generator T>
		struct seed_generator64 {
			using result_type = typename T::result_type;
			using state_type  = typename T::state_type;

			result_type			  seed;

			constexpr result_type operator()() noexcept {
				seed += 0x9e3779b97f4a7c15;

				auto z = seed;
				z	   = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
				z	   = (z ^ (z >> 27)) * 0x94d049bb133111eb;
				return z ^ (z >> 31);
			}

			constexpr state_type generator_seeds() noexcept {
				state_type seeds{};

				for (auto& s: seeds) {
					s = operator()();
				}

				return seeds;
			}
		};

		template<is_type_generator T>
		class engineBase {
		public:
			using result_type				   = typename T::result_type;
			using state_type				   = typename T::state_type;

			constexpr static auto bits_of_this = std::numeric_limits<result_type>::digits;

			constexpr explicit engineBase(state_type state) noexcept : state(std::move(state)) {}

			constexpr explicit engineBase(result_type seed = default_seed) noexcept : state(seed_generator64<T>{seed}.template generator_seeds()) {}

			constexpr virtual result_type operator()() noexcept = 0;

			constexpr void				  jump() noexcept {
				   jump_support(jump_steps_generator());
			}

			constexpr void long_jump() noexcept {
				jump_support(long_jump_steps_generator());
			}

			[[nodiscard("If you do not receive the return value of engineBase::min, this call is meaningless overhead")]] constexpr static result_type min() noexcept {
				return std::numeric_limits<result_type>::lowest();
			}

			[[nodiscard("If you do not receive the return value of engineBase::max, this call is meaningless overhead")]] constexpr static result_type max() noexcept {
				return std::numeric_limits<result_type>::max();
			}

			[[nodiscard("If you do not receive the return value of engineBase::serialize, this call is meaningless overhead")]] constexpr state_type serialize() const noexcept {
				return state;
			}

			constexpr void deserialize(state_type new_state) noexcept {
				state.swap(new_state);
			}

		protected:
			state_type state;

		private:
			constexpr virtual state_type jump_steps_generator() noexcept	  = 0;
			constexpr virtual state_type long_jump_steps_generator() noexcept = 0;

			constexpr virtual void		 jump_support(const state_type& steps) noexcept {
				  state_type to{};

				  for (const auto& step: steps) {
					  for (auto bit = 0; bit < bits_of_this; ++bit) {
						  if (step & result_type{1} << bit) {
							  for (auto i = 0; i < to.size(); ++i) {
								  to[i] ^= state[i];
							  }
						  }
						  operator()();
					  }
				  }

				  state.swap(to);
			}
		};

		class engine64Plus4Base : public engineBase<type_generator<bits_64_type, 4>> {
		public:
			using engineBase::engineBase;

			constexpr result_type operator()() noexcept override {
				auto result = operator_call_result();
				auto t		= state[1] << 17;

				state[2] ^= state[0];
				state[3] ^= state[1];

				state[1] ^= state[2];
				state[0] ^= state[3];

				state[2] ^= t;
				state[3] = detail::rotate_left<45, bits_of_this>(state[3]);

				return result;
			}

		protected:
			using engineBase::state;

		private:
			constexpr virtual result_type operator_call_result() noexcept = 0;

			/**
			 * @brief This is the jump function for the generator. It is equivalent
			 * to 2^128 calls to operator(); it can be used to generate 2^128
			 * non-overlapping subsequences for parallel computations.
			 * @return generated jump steps
			 */
			constexpr state_type		  jump_steps_generator() noexcept override {
				 return {
						 0x180ec6d33cfd0aba,// 1100000001110110001101101001100111100111111010000101100000000
						 0xd5a61266f0c9392c,// 1101010110100110000100100110011011110000110010010011100000000000
						 0xa9582618e03fc9aa,// 1010100101011000001001100001100011100000001111111100100000000000
						 0x39abdc4529b1661c // 11100110101011110111000100010100101001101100010110011000000000
				 };
			}

			/**
			 * @brief This is the long-jump function for the generator. It is equivalent to
			 * 2^192 calls to operator(); it can be used to generate 2^64 starting points,
			 * from each of which jump() will generate 2^64 non-overlapping
			 * subsequences for parallel distributed computations.
			 * @return generated long jump steps
			 */
			constexpr state_type long_jump_steps_generator() noexcept override {
				return {
						0x76e15d3efefdcbbf,// 111011011100001010111010011111011111110111111011100110000000000
						0xc5004e441c522fb3,// 1100010100000000010011100100010000011100010100100011000000000000
						0x77710069854ee241,// 111011101110001000000000110100110000101010011101110010000000000
						0x39109bb02acbe635 // 11100100010000100110111011000000101010110010111110011000000000
				};
			}
		};

		class engine64Plus2Base : public engineBase<type_generator<bits_64_type, 2>> {
		public:
			using engineBase::engineBase;

		protected:
			using engineBase::state;
		};

		class engine32Plus4Base : public engineBase<type_generator<bits_32_type, 4>> {
		public:
			using engineBase::engineBase;

		protected:
			using engineBase::state;
		};
	}// namespace detail

	/**
	 * Output: 64 bits
	 * Period: 2^256 - 1
	 * Footprint: 32 bytes
	 * Original implementation: http://prng.di.unimi.it/xoshiro256plus.c
	 */
	class xorShiftRotate256PlusEngine : public detail::engine64Plus4Base {
	public:
		using engine64Plus4Base::engine64Plus4Base;

	private:
		constexpr result_type operator_call_result() noexcept override {
			return state[0] + state[3];
		}
	};

	/**
	 * Output: 64 bits
	 * Period: 2^256 - 1
	 * Footprint: 32 bytes
	 * Original implementation: http://prng.di.unimi.it/xoshiro256plusplus.c
	 */
	class xorShiftRotate256PlusPlusEngine : public detail::engine64Plus4Base {
	public:
		using engine64Plus4Base::engine64Plus4Base;

	private:
		constexpr result_type operator_call_result() noexcept override {
			return detail::rotate_left<23, bits_of_this>(state[0] + state[3]) + state[0];
		}
	};

	/**
	 * Output: 64 bits
	 * Period: 2^256 - 1
	 * Footprint: 32 bytes
	 * Original implementation: http://prng.di.unimi.it/xoshiro256starstar.c
	 */
	class xorShiftRotate256StarStarEngine : public detail::engine64Plus4Base {
	public:
		using engine64Plus4Base::engine64Plus4Base;

	private:
		constexpr result_type operator_call_result() noexcept override {
			return detail::rotate_left<7, bits_of_this>(state[1] * 5) * 9;
		}
	};

	/**
	 * Output: 64 bits
	 * Period: 2^128 - 1
	 * Footprint: 16 bytes
	 * Original implementation: http://prng.di.unimi.it/xoroshiro128plus.c
	 */
	class xorRotateShiftRotate128PlusEngine : public detail::engine64Plus2Base {
	public:
		constexpr result_type operator()() noexcept override {
			state_type to{state};

			auto	   result = to[0] + to[1];

			to[1] ^= to[0];

			state = {
					detail::rotate_left<24, bits_of_this>(to[0]) ^ to[1] ^ (to[1] << 16),
					detail::rotate_left<37, bits_of_this>(to[1])};

			return result;
		}

	private:
		/**
		 * @brief This is the jump function for the generator. It is equivalent
		 * to 2^64 calls to operator(); it can be used to generate 2^64
		 * non-overlapping subsequences for parallel computations.
		 * @return generated jump steps
		 */
		constexpr state_type jump_steps_generator() noexcept override {
			return {
					0xdf900294d8f554a5,// 1101111110010000000000101001010011011000111101010101100000000000
					0x170865df4b3201fc // 1011100001000011001011101111101001011001100100000001000000000
			};
		}

		/**
		 * @brief This is the long-jump function for the generator. It is equivalent to
		 * 2^96 calls to operator(); it can be used to generate 2^32 starting points,
		 * from each of which jump() will generate 2^32 non-overlapping
		 * subsequences for parallel distributed computations.
		 * @return generated long jump steps
		 */
		constexpr state_type long_jump_steps_generator() noexcept override {
			return {
					0xd2a98b26625eee7b,// 1101001010101001100010110010011001100010010111101111000000000000
					0xdddf9b1090aa7ac1 // 1101110111011111100110110001000010010000101010100111100000000000
			};
		}
	};

	/**
	 * Output: 64 bits
	 * Period: 2^128 - 1
	 * Footprint: 16 bytes
	 * Original implementation: http://prng.di.unimi.it/xoroshiro128plusplus.c
	 */
	class xorRotateShiftRotatePlusPlusEngine : public detail::engine64Plus2Base {
	public:
		constexpr result_type operator()() noexcept override {
			state_type to{state};

			auto	   result = detail::rotate_left<17, bits_of_this>(to[0] + to[1]) + to[0];

			to[1] ^= to[0];

			state = {
					detail::rotate_left<49, bits_of_this>(to[0]) ^ to[1] ^ (to[1] << 21),
					detail::rotate_left<28, bits_of_this>(to[1])};

			return result;
		}

	private:
		/**
		 * @brief This is the jump function for the generator. It is equivalent
		 * to 2^64 calls to operator(); it can be used to generate 2^64
		 * non-overlapping subsequences for parallel computations.
		 * @return generated jump steps
		 */
		constexpr state_type jump_steps_generator() noexcept override {
			return {
					0x2bd7a6a6e99c2ddc,// 10101111010111101001101010011011101001100111000010111000000000
					0x0992ccaf6a6fca05 // 100110010010110011001010111101101010011011111100101000000000
			};
		}

		/**
		 * @brief This is the long-jump function for the generator. It is equivalent to
		 * 2^96 calls to operator(); it can be used to generate 2^32 starting points,
		 * from each of which jump() will generate 2^32 non-overlapping
		 * subsequences for parallel distributed computations.
		 * @return generated jump steps
		 */
		constexpr state_type long_jump_steps_generator() noexcept override {
			return {
					0x360fd5f2cf8d5d99,// 11011000001111110101011111001011001111100011010101111000000000
					0x9c6e6877736c46e3 // 1001110001101110011010000111011101110011011011000100100000000000
			};
		}
	};

	/**
	 * Output: 64 bits
	 * Period: 2^128 - 1
	 * Footprint: 16 bytes
	 * Original implementation: http://prng.di.unimi.it/xoroshiro128starstar.c
	 */
	class xorRotateShiftRotateStarStarEngine : public detail::engine64Plus2Base {
	public:
		constexpr result_type operator()() noexcept override {
			state_type to{state};

			auto	   result = detail::rotate_left<7, bits_of_this>(to[0] * 5) * 9;

			to[1] ^= to[0];

			state = {
					detail::rotate_left<24, bits_of_this>(to[0]) ^ to[1] ^ (to[1] << 16),
					detail::rotate_left<37, bits_of_this>(to[1])};

			return result;
		}

	private:
		/**
		 * @brief This is the jump function for the generator. It is equivalent
		 * to 2^64 calls to operator(); it can be used to generate 2^64
		 * non-overlapping subsequences for parallel computations.
		 * @return generated jump steps
		 */
		constexpr state_type jump_steps_generator() noexcept override {
			return {
					0xdf900294d8f554a5,// 1101111110010000000000101001010011011000111101010101100000000000
					0x170865df4b3201fc // 1011100001000011001011101111101001011001100100000001000000000
			};
		}

		/**
		 * @brief This is the long-jump function for the generator. It is equivalent to
		 * 2^96 calls to operator(); it can be used to generate 2^32 starting points,
		 * from each of which jump() will generate 2^32 non-overlapping
		 * subsequences for parallel distributed computations.
		 * @return generated long jump steps
		 */
		constexpr state_type long_jump_steps_generator() noexcept override {
			return {
					0xd2a98b26625eee7b,// 1101001010101001100010110010011001100010010111101111000000000000
					0xdddf9b1090aa7ac1 // 1101110111011111100110110001000010010000101010100111100000000000
			};
		}
	};

	class xorShiftRotate128PlusEngine : public detail::engine32Plus4Base {
	public:

	};
}// namespace gal::toolbox::random

#endif//GAL_TOOLBOX_RANDOM_HPP