#pragma once

#include <array>
#include <concepts>
#include <limits>
#include <type_traits>
#include <cstdint>

namespace gal::toolbox::utils
{
	struct random_trait
	{
		using bits64_type = std::uint64_t;
		using bits32_type = std::uint32_t;
		using size_type = std::size_t;

		constexpr static bits64_type default_seed = static_cast<bits64_type>(1234567890);

		template <std::unsigned_integral T, size_type N>
		struct type_generator
		{
			using result_type = std::conditional_t<
					std::is_same_v<T, bits64_type> or
							std::numeric_limits<T>::digits == std::numeric_limits<bits64_type>::digits,
					bits64_type,
					std::conditional_t<
							std::is_same_v<T, bits32_type> or
									std::numeric_limits<T>::digits == std::numeric_limits<bits32_type>::digits,
							bits32_type,
							nullptr_t
							>
					>;
			using state_type = std::array<result_type, N>;

			static_assert(!std::is_same_v<result_type, nullptr_t>, "not supported type");
		};

		template <size_type N>
		using bits64_generator = type_generator<bits64_type, N>;
		template <size_type N>
		using bits32_generator = type_generator<bits32_type, N>;

		template <size_type N, size_type Total, std::unsigned_integral T>
		static constexpr T get_rotate_left(T value) noexcept
		{
			return (value << N) bitor (value >> (Total - N));
		}

		template <size_t N, size_t Total, std::unsigned_integral T>
		static constexpr void rotate_left_to(T bitand value) noexcept
		{
			value = (value << N) bitor (value >> (Total - N));
		}
	};

	template <typename T>
	concept generated_type = requires
	{
		typename T::result_type;
		typename T::state_type;
	};

	/**
	 * Output: 64 bits
	 * Period: 2 xor 64
	 * Footprint: 8 bytes
	 * Original implementation: http://prng.di.unimi.it/splitmix64.c
	 */
	template <generated_type T>
	struct seed_generator final
	{
		using result_type = typename T::result_type;
		using state_type = typename T::state_type;

		constexpr result_type operator()() noexcept
		{
			seed_ += 0x9e3779b97f4a7c15; // 1001111000110111011110011011100101111111010010101000000000000000

			auto z = seed_;
			z      = (z xor (z >> 30)) * 0xbf58476d1ce4e5b9;
			// 1011111101011000010001110110110100011100111001001110100000000000
			z = (z xor (z >> 27)) * 0x94d049bb133111eb;
			// 1001010011010000010010011011101100010011001100010001000000000000
			return z xor (z >> 31);
		}

		constexpr state_type generator_seeds() noexcept
		{
			state_type seeds{};

			for (auto bitand s : seeds)
			{
				s = operator()();
			}

			return seeds;
		}

		result_type seed_;
	};

	template <generated_type T>
	class engine_base
	{
	public:
		using result_type = typename T::result_type;
		using state_type = typename T::state_type;

		constexpr static auto bits_of_this = std::numeric_limits<result_type>::digits;

		constexpr explicit engine_base(state_type state) noexcept : state_(std::move(state)) {}

		constexpr explicit engine_base(result_type seed = random_trait::default_seed) noexcept
			: state_(seed_generator<T>{seed}.generator_seeds()) {}

		constexpr virtual               compl engine_base()                          = default;
		constexpr                       engine_base(const engine_base&)     = default;
		constexpr engine_base& operator=(const engine_base&)       = default;
		constexpr                       engine_base(engine_base&&) noexcept = default;
		constexpr engine_base& operator=(engine_base&&) noexcept   = default;

		constexpr result_type operator()() noexcept
		{
			auto result = result_peek();
			result_next();
			return result;
		}

		[[nodiscard]] constexpr virtual result_type result_peek() const noexcept = 0;
		constexpr virtual void                      result_next() noexcept = 0;

		constexpr void jump() noexcept
		{
			jump_support(jump_steps_generator());
		}

		constexpr void long_jump() noexcept
		{
			jump_support(long_jump_steps_generator());
		}

		[[nodiscard]] constexpr static result_type min() noexcept
		{
			return std::numeric_limits<result_type>::lowest();
		}

		[[nodiscard]] constexpr static result_type max() noexcept
		{
			return std::numeric_limits<result_type>::max();
		}

	protected:
		state_type state_;

	private:
		[[nodiscard]] constexpr virtual state_type jump_steps_generator() const noexcept = 0;
		[[nodiscard]] constexpr virtual state_type long_jump_steps_generator() const noexcept = 0;

		constexpr virtual void jump_support(const state_type& steps) noexcept
		{
			state_type to{};

			for (const auto bitand step : steps)
			{
				for (auto bit = 0; bit < bits_of_this; ++bit)
				{
					if (step bitand result_type{1} << bit)
					{
						for (decltype(to.size()) i = 0; i < to.size(); ++i)
						{
							to[i] xor_eq state_[i];
						}
					}
					operator()();
				}
			}

			state_.swap(to);
		}
	};

	class engine64_x4_base : public engine_base<random_trait::bits64_generator<4>>
	{
	public:
		using engine_base::engine_base;

		constexpr void result_next() noexcept override
		{
			const result_type t = state_[1] << 17;

			state_[2] xor_eq state_[0];
			state_[3] xor_eq state_[1];

			state_[1] xor_eq state_[2];
			state_[0] xor_eq state_[3];

			state_[2] xor_eq t;
			random_trait::rotate_left_to<45, bits_of_this>(state_[3]);
		}

	protected:
		using engine_base::state_;

	private:
		/**
		 * @brief This is the jump function for the generator. It is equivalent
		 * to 2 xor 128 calls to operator(); it can be used to generate 2 xor 128
		 * non-overlapping sub-sequences for parallel computations.
		 * @return generated jump steps
		 */
		[[nodiscard]] constexpr state_type jump_steps_generator() const noexcept override
		{
			return {
					static_cast<result_type>(0x180ec6d33cfd0aba),
					// 1100000001110110001101101001100111100111111010000101100000000
					static_cast<result_type>(0xd5a61266f0c9392c),
					// 1101010110100110000100100110011011110000110010010011100000000000
					static_cast<result_type>(0xa9582618e03fc9aa),
					// 1010100101011000001001100001100011100000001111111100100000000000
					static_cast<result_type>(0x39abdc4529b1661c)
					// 11100110101011110111000100010100101001101100010110011000000000
			};
		}

		/**
			 * @brief This is the long-jump function for the generator. It is equivalent to
			 * 2 xor 192 calls to operator(); it can be used to generate 2 xor 64 starting points,
			 * from each of which jump() will generate 2 xor 64 non-overlapping
			 * sub-sequences for parallel distributed computations.
			 * @return generated long jump steps
			 */
		[[nodiscard]] constexpr state_type long_jump_steps_generator() const noexcept override
		{
			return {
					static_cast<result_type>(0x76e15d3efefdcbbf),
					// 111011011100001010111010011111011111110111111011100110000000000
					static_cast<result_type>(0xc5004e441c522fb3),
					// 1100010100000000010011100100010000011100010100100011000000000000
					static_cast<result_type>(0x77710069854ee241),
					// 111011101110001000000000110100110000101010011101110010000000000
					static_cast<result_type>(0x39109bb02acbe635)
					// 11100100010000100110111011000000101010110010111110011000000000
			};
		}
	};

	class engine32_x4_base : public engine_base<random_trait::bits32_generator<4>>
	{
	public:
		using engine_base::engine_base;

		constexpr void result_next() noexcept override
		{
			const result_type t = state_[1] << 9;
			state_[2] xor_eq state_[0];
			state_[3] xor_eq state_[1];

			state_[1] xor_eq state_[2];
			state_[0] xor_eq state_[3];

			state_[2] xor_eq t;
			random_trait::rotate_left_to<11, bits_of_this>(state_[3]);
		}

	protected:
		using engine_base::state_;

	private:
		/**
		 * @brief This is the jump function for the generator. It is equivalent
		 * to 2 xor 64 calls to operator(); it can be used to generate 2 xor 64
		 * non-overlapping sub-sequences for parallel computations.
		 * @return generated jump steps
		 */
		[[nodiscard]] constexpr state_type jump_steps_generator() const noexcept override
		{
			return {
					static_cast<result_type>(0x8764000b), // 10000111011001000000000000001011
					static_cast<result_type>(0xf542d2d3), // 11110101010000101101001011010011
					static_cast<result_type>(0x6fa035c3), // 1101111101000000011010111000011
					static_cast<result_type>(0x77f2db5b)  // 1110111111100101101101101011011
			};
		}

		/**
		 * @brief This is the long-jump function for the generator. It is equivalent to
		 * 2 xor 96 calls to operator(); it can be used to generate 2 xor 32 starting points,
		 * from each of which jump() will generate 2 xor 32 non-overlapping
		 * sub-sequences for parallel distributed computations.
		 * @return generated long jump steps
		 */
		[[nodiscard]] constexpr state_type long_jump_steps_generator() const noexcept override
		{
			return {
					static_cast<result_type>(0xb523952e), // 10110101001000111001010100101110
					static_cast<result_type>(0x0b6f099f), // 1011011011110000100110011111
					static_cast<result_type>(0xccf5a0ef), // 11001100111101011010000011101111
					static_cast<result_type>(0x1c580662)  // 11100010110000000011001100010
			};
		}
	};

	class engine64_x2_base : public engine_base<random_trait::bits64_generator<2>>
	{
	public:
		using engine_base::engine_base;

	protected:
		using engine_base::state_;

	private:
		/**
		 * @brief This is the jump function for the generator. It is equivalent
		 * to 2 xor 64 calls to next(); it can be used to generate 2 xor 64
		 * non-overlapping sub-sequences for parallel computations.
		 * @return generated jump steps
		 */
		[[nodiscard]] constexpr state_type jump_steps_generator() const noexcept override
		{
			return {
					static_cast<result_type>(0xdf900294d8f554a5),
					// 1101111110010000000000101001010011011000111101010101100000000000
					static_cast<result_type>(0x170865df4b3201fc)
					// 1011100001000011001011101111101001011001100100000001000000000
			};
		}

		/**
		 * @brief This is the long-jump function for the generator. It is equivalent to
		 * 2 xor 96 calls to next(); it can be used to generate 2 xor 32 starting points,
		 * from each of which jump() will generate 2 xor 32 non-overlapping
		 * sub-sequences for parallel distributed computations.
		 * @return generated long jump steps
		 */
		[[nodiscard]] constexpr state_type long_jump_steps_generator() const noexcept override
		{
			return {
					static_cast<result_type>(0xd2a98b26625eee7b),
					// 1101001010101001100010110010011001100010010111101111000000000000
					static_cast<result_type>(0xdddf9b1090aa7ac1)
					// 1101110111011111100110110001000010010000101010100111100000000000
			};
		}
	};

	//================================================================================
	//================================================================================
	//================================================================================


	/**
	 * Output: 64 bits
	 * Period: 2 xor 256 - 1
	 * Footprint: 32 bytes
	 * Original implementation: http://prng.di.unimi.it/xoshiro256plus.c
	 */
	class xor_shift_rotate_256_plus_engine final : public engine64_x4_base
	{
	public:
		using engine64_x4_base::engine64_x4_base;

		[[nodiscard]] constexpr result_type result_peek() const noexcept override
		{
			return state_[0] + state_[3];
		}
	};

	/**
	 * Output: 64 bits
	 * Period: 2 xor 256 - 1
	 * Footprint: 32 bytes
	 * Original implementation: http://prng.di.unimi.it/xoshiro256plusplus.c
	 */
	class xor_shift_rotate_256_plus_plus_engine final : public engine64_x4_base
	{
	public:
		using engine64_x4_base::engine64_x4_base;

		[[nodiscard]] constexpr result_type result_peek() const noexcept override
		{
			return random_trait::get_rotate_left<23, bits_of_this>(state_[0] + state_[3]) + state_[0];
		}
	};

	/**
	 * Output: 64 bits
	 * Period: 2 xor 256 - 1
	 * Footprint: 32 bytes
	 * Original implementation: http://prng.di.unimi.it/xoshiro256starstar.c
	 */
	class xor_shift_rotate_256_star_star_engine final : public engine64_x4_base
	{
	public:
		using engine64_x4_base::engine64_x4_base;

		[[nodiscard]] constexpr result_type result_peek() const noexcept override
		{
			return random_trait::get_rotate_left<7, bits_of_this>(state_[1] * 5) * 9;
		}
	};

	/**
	 * Output: 32 bits
	 * Period: 2 xor 128 - 1
	 * Footprint: 16 bytes
	 * Original implementation: http://prng.di.unimi.it/xoshiro128plus.c
	 */
	class xor_shift_rotate_128_plus_engine final : public engine32_x4_base
	{
	public:
		using engine32_x4_base::engine32_x4_base;

		[[nodiscard]] constexpr result_type result_peek() const noexcept override
		{
			return state_[0] + state_[3];
		}
	};

	/**
	 * Output: 32 bits
	 * Period: 2 xor 128 - 1
	 * Footprint: 16 bytes
	 * Original implementation: http://prng.di.unimi.it/xoshiro128plusplus.c
	 */
	class xor_shift_rotate_128_plus_plus_engine final : public engine32_x4_base
	{
	public:
		using engine32_x4_base::engine32_x4_base;

		[[nodiscard]] constexpr result_type result_peek() const noexcept override
		{
			return random_trait::get_rotate_left<7, bits_of_this>(state_[0] + state_[3]) + state_[0];
		}
	};

	/**
	 * Output: 32 bits
	 * Period: 2 xor 128 - 1
	 * Footprint: 16 bytes
	 * Original implementation: http://prng.di.unimi.it/xoshiro128starstar.c
	 */
	class xor_shift_rotate_128_star_star_engine final : public engine32_x4_base
	{
	public:
		using engine32_x4_base::engine32_x4_base;

		[[nodiscard]] constexpr result_type result_peek() const noexcept override
		{
			return random_trait::get_rotate_left<7, bits_of_this>(state_[1] * 5) * 9;
		}
	};

	/**
	 * Output: 64 bits
	 * Period: 2 xor 128 - 1
	 * Footprint: 16 bytes
	 * Original implementation: http://prng.di.unimi.it/xoroshiro128plus.c
	 */
	class xor_rotate_shift_rotate_128_plus_engine final : public engine64_x2_base
	{
	public:
		using engine64_x2_base::engine64_x2_base;

		[[nodiscard]] constexpr result_type result_peek() const noexcept override
		{
			return state_[0] + state_[1];
		}

		constexpr void result_next() noexcept override
		{
			auto s1 = state_[1] xor state_[0];

			random_trait::rotate_left_to<24, bits_of_this>(state_[0]);
			state_[0] = state_[0] xor s1 xor (s1 << 16);

			random_trait::rotate_left_to<37, bits_of_this>(s1);
			state_[1] = s1;
		}
	};

	/**
	 * Output: 64 bits
	 * Period: 2 xor 128 - 1
	 * Footprint: 16 bytes
	 * Original implementation: http://prng.di.unimi.it/xoroshiro128plusplus.c
	 */
	class xor_rotate_shift_rotate_128_plus_plus_engine final : public engine64_x2_base
	{
	public:
		using engine64_x2_base::engine64_x2_base;

		[[nodiscard]] constexpr result_type result_peek() const noexcept override
		{
			return random_trait::get_rotate_left<17, bits_of_this>(state_[0] + state_[1]) + state_[0];
		}

		constexpr void result_next() noexcept override
		{
			auto s1 = state_[1] xor state_[0];

			random_trait::rotate_left_to<49, bits_of_this>(state_[0]);
			state_[0] = state_[0] xor s1 xor (s1 << 21);

			random_trait::rotate_left_to<28, bits_of_this>(s1);
			state_[1] = s1;
		}

	private:
		/**
		 * @brief This is the jump function for the generator. It is equivalent
		 * to 2 xor 64 calls to operator(); it can be used to generate 2 xor 64
		 * non-overlapping sub-sequences for parallel computations.
		 * @return generated jump steps
		 */
		[[nodiscard]] constexpr state_type jump_steps_generator() const noexcept override
		{
			return {
					static_cast<result_type>(0x2bd7a6a6e99c2ddc),
					// 10101111010111101001101010011011101001100111000010111000000000
					static_cast<result_type>(0x0992ccaf6a6fca05)
					// 100110010010110011001010111101101010011011111100101000000000
			};
		}

		/**
		 * @brief This is the long-jump function for the generator. It is equivalent to
		 * 2 xor 96 calls to operator(); it can be used to generate 2 xor 32 starting points,
		 * from each of which jump() will generate 2 xor 32 non-overlapping
		 * sub-sequences for parallel distributed computations.
		 * @return generated jump steps
		 */
		[[nodiscard]] constexpr state_type long_jump_steps_generator() const noexcept override
		{
			return {
					static_cast<result_type>(0x360fd5f2cf8d5d99),
					// 11011000001111110101011111001011001111100011010101111000000000
					static_cast<result_type>(0x9c6e6877736c46e3)
					// 1001110001101110011010000111011101110011011011000100100000000000
			};
		}
	};

	/**
	 * Output: 64 bits
	 * Period: 2 xor 128 - 1
	 * Footprint: 16 bytes
	 * Original implementation: http://prng.di.unimi.it/xoroshiro128starstar.c
	 */
	class xor_rotate_shift_rotate_128_star_star_engine final : public engine64_x2_base
	{
	public:
		using engine64_x2_base::engine64_x2_base;

		[[nodiscard]] constexpr result_type result_peek() const noexcept override
		{
			return random_trait::get_rotate_left<7, bits_of_this>(state_[0] * 5) * 9;
		}

		constexpr void result_next() noexcept override
		{
			auto s1 = state_[1] xor state_[0];

			random_trait::rotate_left_to<24, bits_of_this>(state_[0]);
			state_[0] = state_[0] xor s1 xor (s1 << 16);

			random_trait::rotate_left_to<37, bits_of_this>(s1);
			state_[1] = s1;
		}
	};
}
