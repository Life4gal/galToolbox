#pragma once

#if !__has_include(<emmintrin.h>) ||   \
	!__has_include(<xmmintrin.h>) ||   \
	!__has_include(<pmmintrin.h>) ||   \
	!__has_include(<smmintrin.h>) ||   \
	!__has_include(<immintrin.h>)
	#define GALTOOLBOX_MATH_NOT_SUPPOTED
#endif

#ifndef GALTOOLBOX_MATH_NOT_SUPPOTED

	// SSE
	#include <emmintrin.h>
	#include <xmmintrin.h>
	// SSE3
	#include <pmmintrin.h>
	// SSE4
	#include <smmintrin.h>
	// AVX
	#include <immintrin.h>

	#include <cfloat>
	#include <cmath>
	#include <cstdint>
	#include <utils/assert.hpp>
	#include <utils/tuple_maker.hpp>
	#include <utils/concepts.hpp>

namespace gal::toolbox::math
{
	inline namespace data_types
	{
		template<typename T>
		concept basic_math_type_t = gal::toolbox::utils::any_type_of_t<T, float, std::int32_t, std::uint32_t>;

		//=======================================================
		// for operations
		//=======================================================
		using vector			  = __m128;

		static_assert(std::alignment_of_v<vector> == 16);
		constexpr static std::size_t math_type_alignment = std::alignment_of_v<vector>;

		template<typename T>
		struct alignas(math_type_alignment) generic_vector;
		using f32_vector = generic_vector<float>;
		using i32_vector = generic_vector<std::int32_t>;
		using u8_vector	 = generic_vector<std::uint8_t>;
		using u32_vector = generic_vector<std::uint32_t>;
		struct alignas(math_type_alignment) matrix;

		//=======================================================
		// for storage
		//=======================================================

	#ifdef _MSC_VER
		#pragma warning(push)
		#pragma warning(disable : 4324 4820)// C4324/4820: padding warnings
	#endif
	#ifdef __GNUC__
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wpadded"
	#endif
	#ifdef __clang__
		#pragma clang diagnostic push
		#pragma clang diagnostic ignored "-Wpadded"
	#endif

		template<typename T, std::size_t Size>
		struct generic_one_tier_container;
		using float2 = generic_one_tier_container<float, 2>;
		struct alignas(math_type_alignment) float2a;
		using float3 = generic_one_tier_container<float, 3>;
		struct alignas(math_type_alignment) float3a;
		using float4 = generic_one_tier_container<float, 4>;
		struct alignas(math_type_alignment) float4a;
		using int2 = generic_one_tier_container<std::int32_t, 2>;
		struct alignas(math_type_alignment) int2a;
		using int3 = generic_one_tier_container<std::int32_t, 3>;
		struct alignas(math_type_alignment) int3a;
		using int4 = generic_one_tier_container<std::int32_t, 4>;
		struct alignas(math_type_alignment) int4a;
		using uint2 = generic_one_tier_container<std::uint32_t, 2>;
		struct alignas(math_type_alignment) uint2a;
		using uint3 = generic_one_tier_container<std::uint32_t, 3>;
		struct alignas(math_type_alignment) uint3a;
		using uint4 = generic_one_tier_container<std::uint32_t, 4>;
		struct alignas(math_type_alignment) uint4a;

		template<typename T, std::size_t FirstTier, std::size_t SecondTier>
		struct generic_two_tier_container;
		using float3x3 = generic_two_tier_container<float, 3, 3>;
		struct alignas(math_type_alignment) float3x3a;
		using float3x4 = generic_two_tier_container<float, 3, 4>;
		struct alignas(math_type_alignment) float3x4a;
		using float4x3 = generic_two_tier_container<float, 4, 3>;
		struct alignas(math_type_alignment) float4x3a;
		using float4x4 = generic_two_tier_container<float, 4, 4>;
		struct alignas(math_type_alignment) float4x4a;

	#ifdef _MSC_VER
		#pragma warning(pop)
	#endif
	#ifdef __GNUC__
		#pragma GCC diagnostic pop
	#endif
	#ifdef __clang__
		#pragma clang diagnostic pop
	#endif

		template<typename T>
		struct is_vector : std::false_type
		{
		};
		template<>
		struct is_vector<f32_vector> : std::true_type
		{
		};
		template<>
		struct is_vector<i32_vector> : std::true_type
		{
		};
		template<>
		struct is_vector<u8_vector> : std::true_type
		{
		};
		template<>
		struct is_vector<u32_vector> : std::true_type
		{
		};
		template<typename T>
		constexpr static bool is_vector_v = is_vector<T>::value;
		template<typename T>
		concept vector_t = is_vector_v<T>;

		template<typename T>
		struct is_matrix : std::is_same<matrix, T>
		{
		};
		template<typename T>
		constexpr static bool is_matrix_v = is_matrix<T>::value;
		template<typename T>
		concept matrix_t = is_matrix_v<T>;

		template<typename T>
		constexpr static bool is_vector_or_matrix = is_vector_v<T> || is_matrix_v<T>;
		template<typename T>
		concept vector_or_matrix_t = vector_t<T> || matrix_t<T>;

		template<typename T>
		struct is_one_tier_container : std::false_type
		{
		};
		template<>
		struct is_one_tier_container<float2> : std::true_type
		{
		};
		template<>
		struct is_one_tier_container<float3> : std::true_type
		{
		};
		template<>
		struct is_one_tier_container<float4> : std::true_type
		{
		};
		template<>
		struct is_one_tier_container<int2> : std::true_type
		{
		};
		template<>
		struct is_one_tier_container<int3> : std::true_type
		{
		};
		template<>
		struct is_one_tier_container<int4> : std::true_type
		{
		};
		template<>
		struct is_one_tier_container<uint2> : std::true_type
		{
		};
		template<>
		struct is_one_tier_container<uint3> : std::true_type
		{
		};
		template<>
		struct is_one_tier_container<uint4> : std::true_type
		{
		};
		template<typename T>
		constexpr static bool is_one_tier_container_v = is_one_tier_container<T>::value;
		template<typename T>
		concept one_tier_container_t = is_one_tier_container_v<T>;

		template<typename T>
		struct is_aligned_one_tier_container : std::false_type
		{
		};
		template<>
		struct is_aligned_one_tier_container<float2a> : std::true_type
		{
		};
		template<>
		struct is_aligned_one_tier_container<float3a> : std::true_type
		{
		};
		template<>
		struct is_aligned_one_tier_container<float4a> : std::true_type
		{
		};
		template<>
		struct is_aligned_one_tier_container<int2a> : std::true_type
		{
		};
		template<>
		struct is_aligned_one_tier_container<int3a> : std::true_type
		{
		};
		template<>
		struct is_aligned_one_tier_container<int4a> : std::true_type
		{
		};
		template<>
		struct is_aligned_one_tier_container<uint2a> : std::true_type
		{
		};
		template<>
		struct is_aligned_one_tier_container<uint3a> : std::true_type
		{
		};
		template<>
		struct is_aligned_one_tier_container<uint4a> : std::true_type
		{
		};
		template<typename T>
		constexpr static bool is_aligned_one_tier_container_v = is_aligned_one_tier_container<T>::value;
		template<typename T>
		concept aligned_one_tier_container_t = is_aligned_one_tier_container_v<T>;

		template<typename T>
		constexpr static bool is_one_tier_container_ignore_aligned_v = is_one_tier_container_v<T> || is_aligned_one_tier_container_v<T>;
		template<typename T>
		concept one_tier_container_ignore_aligned_t = is_one_tier_container_ignore_aligned_v<T>;

		template<typename T>
		struct is_two_tier_container : std::false_type
		{
		};
		template<>
		struct is_two_tier_container<float3x3> : std::true_type
		{
		};
		template<>
		struct is_two_tier_container<float3x4> : std::true_type
		{
		};
		template<>
		struct is_two_tier_container<float4x3> : std::true_type
		{
		};
		template<>
		struct is_two_tier_container<float4x4> : std::true_type
		{
		};
		template<typename T>
		constexpr static bool is_two_tier_container_v = is_two_tier_container<T>::value;
		template<typename T>
		concept two_tier_container_t = is_two_tier_container_v<T>;

		template<typename T>
		struct is_aligned_two_tier_container : std::false_type
		{
		};
		template<>
		struct is_aligned_two_tier_container<float3x3a> : std::true_type
		{
		};
		template<>
		struct is_aligned_two_tier_container<float3x4a> : std::true_type
		{
		};
		template<>
		struct is_aligned_two_tier_container<float4x3a> : std::true_type
		{
		};
		template<>
		struct is_aligned_two_tier_container<float4x4a> : std::true_type
		{
		};
		template<typename T>
		constexpr static bool is_aligned_two_tier_container_v = is_aligned_two_tier_container<T>::value;
		template<typename T>
		concept aligned_two_tier_container_t = is_aligned_two_tier_container_v<T>;

		template<typename T>
		constexpr static bool is_two_tier_container_ignore_aligned_v = is_two_tier_container_v<T> || is_aligned_two_tier_container_v<T>;
		template<typename T>
		concept two_tier_container_ignore_aligned_t = is_two_tier_container_ignore_aligned_v<T>;

		template<typename T>
		constexpr static bool is_one_or_two_tier_container_v = is_one_tier_container_v<T> || is_two_tier_container_v<T>;
		template<typename T>
		constexpr static bool is_aligned_one_or_two_tier_container_v = is_aligned_one_tier_container_v<T> || is_aligned_two_tier_container_v<T>;
		template<typename T>
		constexpr static bool is_one_or_two_tier_container_ignore_aligned_v = is_one_tier_container_ignore_aligned_v<T> || is_two_tier_container_ignore_aligned_v<T>;
		template<typename T>
		concept one_or_two_tier_container_t = is_one_or_two_tier_container_v<T>;
		template<typename T>
		concept aligned_one_or_two_tier_container_t = is_aligned_one_or_two_tier_container_v<T>;
		template<typename T>
		concept one_or_two_tier_container_ignore_aligned_t = is_one_or_two_tier_container_ignore_aligned_v<T>;
	}// namespace data_types

	inline namespace operations
	{
		[[nodiscard]] constexpr vector operator+(const vector& v) noexcept;
		[[nodiscard]] constexpr vector operator-(const vector& v) noexcept;

		constexpr vector&			   operator+=(vector& v1, const vector& v2) noexcept;
		constexpr vector&			   operator-=(vector& v1, const vector& v2) noexcept;
		constexpr vector&			   operator*=(vector& v1, const vector& v2) noexcept;
		constexpr vector&			   operator/=(vector& v1, const vector& v2) noexcept;

		constexpr vector&			   operator+=(vector& v, float scalar) noexcept;
		constexpr vector&			   operator-=(vector& v, float scalar) noexcept;
		constexpr vector&			   operator*=(vector& v, float scalar) noexcept;
		constexpr vector&			   operator/=(vector& v, float scalar) noexcept;

		[[nodiscard]] constexpr vector operator+(const vector& v1, const vector& v2) noexcept;
		[[nodiscard]] constexpr vector operator-(const vector& v1, const vector& v2) noexcept;
		[[nodiscard]] constexpr vector operator*(const vector& v1, const vector& v2) noexcept;
		[[nodiscard]] constexpr vector operator/(const vector& v1, const vector& v2) noexcept;

		[[nodiscard]] constexpr vector operator+(const vector& v, float scalar) noexcept;
		[[nodiscard]] constexpr vector operator-(const vector& v, float scalar) noexcept;
		[[nodiscard]] constexpr vector operator*(const vector& v, float scalar) noexcept;
		[[nodiscard]] constexpr vector operator/(const vector& v, float scalar) noexcept;

		[[nodiscard]] constexpr vector operator+(float scalar, const vector& v) noexcept;
		[[nodiscard]] constexpr vector operator-(float scalar, const vector& v) noexcept;
		[[nodiscard]] constexpr vector operator*(float scalar, const vector& v) noexcept;
		[[nodiscard]] constexpr vector operator/(float scalar, const vector& v) noexcept;

		[[nodiscard]] constexpr matrix operator+(const matrix& m) noexcept;
		[[nodiscard]] constexpr matrix operator-(const matrix& m) noexcept;

		constexpr matrix&			   operator+=(matrix& m1, const matrix& m2) noexcept;
		constexpr matrix&			   operator-=(matrix& m1, const matrix& m2) noexcept;
		constexpr matrix&			   operator*=(matrix& m1, const matrix& m2) noexcept;
		constexpr matrix&			   operator/=(matrix& m1, const matrix& m2) noexcept;

		constexpr matrix&			   operator+=(matrix& m, float scalar) noexcept;
		constexpr matrix&			   operator-=(matrix& m, float scalar) noexcept;
		constexpr matrix&			   operator*=(matrix& m, float scalar) noexcept;
		constexpr matrix&			   operator/=(matrix& m, float scalar) noexcept;

		[[nodiscard]] constexpr matrix operator+(const matrix& m1, const matrix& m2) noexcept;
		[[nodiscard]] constexpr matrix operator-(const matrix& m1, const matrix& m2) noexcept;
		[[nodiscard]] constexpr matrix operator*(const matrix& m1, const matrix& m2) noexcept;
		[[nodiscard]] constexpr matrix operator/(const matrix& m1, const matrix& m2) noexcept;

		[[nodiscard]] constexpr matrix operator+(const matrix& m, float scalar) noexcept;
		[[nodiscard]] constexpr matrix operator-(const matrix& m, float scalar) noexcept;
		[[nodiscard]] constexpr matrix operator*(const matrix& m, float scalar) noexcept;
		[[nodiscard]] constexpr matrix operator/(const matrix& m, float scalar) noexcept;

		[[nodiscard]] constexpr matrix operator+(float scalar, const matrix& m) noexcept;
		[[nodiscard]] constexpr matrix operator-(float scalar, const matrix& m) noexcept;
		[[nodiscard]] constexpr matrix operator*(float scalar, const matrix& m) noexcept;
		[[nodiscard]] constexpr matrix operator/(float scalar, const matrix& m) noexcept;
	}// namespace operations

	inline namespace utils
	{
		[[nodiscard]] constexpr float  degrees2radians(float degrees) noexcept;

		[[nodiscard]] constexpr float  radians2degrees(float radians) noexcept;

		[[nodiscard]] constexpr vector vector_i2f(const vector& v, std::uint32_t div_exponent) noexcept;
		[[nodiscard]] constexpr vector vector_f2i(const vector& v, std::uint32_t mul_exponent) noexcept;
		[[nodiscard]] constexpr vector vector_ui2f(const vector& v, std::uint32_t div_exponent) noexcept;
		[[nodiscard]] constexpr vector vector_f2ui(const vector& v, std::uint32_t mul_exponent) noexcept;

		/****************************************************************************
	     *
	     * Load operations
	     *
	     ****************************************************************************/

		/**
		 * @brief Read `Size` data from a pointer of type float/std::int32_t/std::uint32_t to initialize the vector.
		 * It must be guaranteed that 1 <= Size <= 4, when the type is std::int32_t and the data is not greater than 0 The behavior is undefined,
		 * and it is only guaranteed that the obtained vector is of the correct type when read with the given type.
		 * @tparam Size data size
		 * @tparam T data type
		 * @param source source pointer
		 * @return vector
		 */
		template<std::size_t Size, basic_math_type_t T>
		requires(Size >= 1 && Size <= 4) constexpr vector vector_load(const T* source) noexcept;

		/**
		 * @brief Read `Size` data from a one_tier_container of value_type float/std::int32_t/std::uint32_t to initialize the vector.
		 * It must be guaranteed that 2 <= Size <= 4, when the type is std::int32_t and the data is not greater than 0 The behavior is undefined,
		 * and it is only guaranteed that the obtained vector is of the correct type when read with the float.
		 * @note it is only guaranteed that the obtained vector is of the correct type when read with the float.
		 * @tparam T one_tier_container_ignore_aligned type
		 * @param source one_tier_container source
		 * @return vector
		 */
		template<one_tier_container_ignore_aligned_t T>
		requires (T::size >= 2 && T::size <= 4) constexpr vector vector_load(const T& source) noexcept;

		/**
		 * @brief Read `Size` data from a two_tier_container of value_type float to initialize the matrix.
		 * It must be guaranteed that 3 <= FirstTier <= 4 and 3 <= SecondTier,
		 * and it is only guaranteed that the obtained vector is of the correct type when read with the float.
		 * @note need to be transposed before use if source type is float3x4
		 * @tparam T two_tier_container_ignore_aligned type
		 * @param source two_tier_container source
		 * @return matrix
		 */
		template<two_tier_container_ignore_aligned_t T>
		requires (T::first_size == 3 || T::first_size == 4) && (T::second_size == 3 || T::second_size == 4) constexpr matrix matrix_load(const T& source) noexcept;

		/****************************************************************************
	     *
	     * Store operations
	     *
	     ****************************************************************************/

		template<std::size_t Size, basic_math_type_t T>
		requires(Size >= 1 && Size <= 4) constexpr void vector_store(T* dest, const vector& source) noexcept;

		template<one_tier_container_ignore_aligned_t T>
		requires (T::size >= 2 && T::size <= 4) constexpr void vector_store(T& dest, const vector& source) noexcept;

		template<two_tier_container_ignore_aligned_t T>
		requires (T::first_size == 3 || T::first_size == 4) && (T::second_size == 3 || T::secondt_size == 4) constexpr void matrix_store(T& dest, const matrix& source) noexcept;

		/****************************************************************************
	     *
	     * General vector operations
	     *
	     ****************************************************************************/

		/**
		 * @brief Return a vector with all elements equaling zero
		 * @return vector
		 */
		constexpr vector vector_zero() noexcept;

		/**
		 * @brief Initialize a vector with four basic_math_type_t values
		 * @tparam T parameters type
		 * @return vector
		 */
		template<basic_math_type_t T>
		constexpr vector vector_set(T x, std::type_identity_t<T> y, std::type_identity_t<T> z, std::type_identity_t<T> w) noexcept;

		/**
		 * @brief Initialize a vector with a replicated basic_math_type_t value
		 * @tparam T parameter type
		 * @return vector
		 */
		template<basic_math_type_t T>
		constexpr vector vector_replicate(T value) noexcept;

		/**
		 * @brief Initialize a vector with a replicated basic_math_type_t value passed by pointer
		 * @tparam T parameter type
		 * @return vector
		 */
		template<basic_math_type_t T>
		constexpr vector vector_replicate(const T* value) noexcept;

		/**
		 * @brief Initialize a vector with all bits set (true mask)
		 * @return vector
		 */
		constexpr vector vector_true() noexcept;

		/**
		 * @brief Initialize a vector with all bits clear (false mask)
		 * @return vector
		 */
		constexpr vector vector_false() noexcept;

		/**
		 * @brief Replicate the x/y/z/w(depends on Index) component of the vector
		 * @tparam Index x/y/z/w -> 0/1/2/3
		 * @tparam ExpectType active union member type
		 * @return vector
		 */
		template<std::size_t Index, basic_math_type_t ActiveType = float>
		requires(Index >= 0 && Index <= 3) constexpr vector vector_splat(const vector& v) noexcept;

		/**
		 * @brief Return a vector of 1.0f,1.0f,1.0f,1.0f
		 * @return vector
		 */
		constexpr vector vector_splat_one() noexcept;

		/**
		 * @brief Return a vector of INF,INF,INF,INF
		 * @return vector
		 */
		constexpr vector vector_splat_infinity() noexcept;

		/**
		 * @brief Return a vector of Q_NAN,Q_NAN,Q_NAN,Q_NAN
		 * @return vector
		 */
		constexpr vector vector_splat_qnan() noexcept;

		/**
		 * @brief Return a vector of 1.192092896e-7f,1.192092896e-7f,1.192092896e-7f,1.192092896e-7f
		 * @return vector
		 */
		constexpr vector vector_splat_epsilon() noexcept;

		/**
		 * @brief Return a vector of -0.0f (0x80000000),-0.0f,-0.0f,-0.0f
		 * @return vector
		 */
		constexpr vector vector_splat_sign_mask() noexcept;

		/**
		 * @brief Return a floating point value via an index.
		 * @note active union member of vector must be m128_f32
		 * @tparam Index x/y/z/w -> 0/1/2/3
		 * @tparam ActiveType active union member type
		 * @return ActiveType value
		 */
		template<std::size_t Index, basic_math_type_t ActiveType = float>
		requires(Index >= 0 && Index <= 3) constexpr ActiveType vector_get(const vector& v) noexcept;
	}// namespace utils

	inline namespace data_types
	{
		template<typename T>
		struct alignas(math_type_alignment) generic_vector
		{
			// the size of the required type is a factor of math_type_alignment
			static_assert(math_type_alignment % sizeof(T) == 0);
			static_assert(sizeof(T) <= math_type_alignment);

			using value_type				= T;
			using size_type					= std::size_t;

			constexpr static size_type size = math_type_alignment / sizeof(value_type);

			union
			{
				value_type v1[size];
				vector	   v2;
			} data;

			constexpr explicit operator const value_type*() const noexcept
			{
				return data.v1;
			}

			[[nodiscard]] constexpr explicit operator vector() const noexcept
					requires basic_math_type_t<value_type>
			{
				// Cannot access an inactive union member in constant expression
				// copy one by hand
				if (std::is_constant_evaluated())
				{
					vector		v{};
					value_type* p;
					if constexpr (std::is_same_v<float, value_type>)
					{
						p = v.m128_f32;
					}
					else if constexpr (std::is_same_v<std::int32_t, value_type>)
					{
						p = v.m128_i32;
					}
					else
					{
						p = v.m128_u32;
					}

					p[0] = data.v1[0];
					p[1] = data.v1[1];
					p[2] = data.v1[2];
					p[3] = data.v1[3];
					return v;
					//					return std::bit_cast<vector>(data.v1);
				}

				return data.v2;
			}

			[[nodiscard]] inline explicit operator __m128i() const noexcept
			{
				return _mm_castps_si128(this->operator vector());
			}

			[[nodiscard]] inline explicit operator __m128d() const noexcept
			{
				return _mm_castps_pd(this->operator vector());
			}
		};

		static_assert(f32_vector::size == 4);
		static_assert(i32_vector::size == 4);
		static_assert(u8_vector::size == 16);
		static_assert(u32_vector::size == 4);

		struct alignas(math_type_alignment) matrix
		{
			using value_type					   = float;
			using size_type						   = std::size_t;
			using vector_type					   = vector;

			constexpr static size_type extent_size = 4;
			constexpr static size_type size		   = extent_size * 4;

			using reference						   = vector_type&;

			vector_type						  data[4];

			[[nodiscard]] constexpr reference operator[](size_type index) noexcept
			{
				gal::toolbox::utils::gal_assert(index < extent_size, "index out of bound");

				return data[index];
			}

			[[nodiscard]] constexpr vector_type operator[](size_type index) const noexcept
			{
				gal::toolbox::utils::gal_assert(index < extent_size, "index out of bound");

				return data[index];
			}
		};

		template<typename T, std::size_t Size>
		struct generic_one_tier_container
		{
			using value_type					  = T;
			using size_type						  = std::size_t;

			constexpr static size_type size		  = Size;

			// HARD CODE, DO NOT CHANGE IT!
			constexpr static size_type index_of_x = 0;
			constexpr static size_type index_of_y = 1;
			constexpr static size_type index_of_z = 2;
			constexpr static size_type index_of_w = 3;

			using reference						  = value_type&;

			value_type						  data[size];

			[[nodiscard]] constexpr reference operator[](size_type index) noexcept
			{
				gal::toolbox::utils::gal_assert(index < size, "index out of bound");

				return data[index];
			}

			[[nodiscard]] constexpr value_type operator[](size_type index) const noexcept
			{
				gal::toolbox::utils::gal_assert(index < size, "index out of bound");

				return data[index];
			}

			[[nodiscard]] constexpr reference x() noexcept
					requires(size >= index_of_x + 1)
			{
				return data[index_of_x];
			}

			[[nodiscard]] constexpr value_type x() const noexcept
					requires(size >= index_of_x + 1)
			{
				return data[index_of_x];
			}

			[[nodiscard]] constexpr reference r() noexcept
					requires(size >= index_of_x + 1)
			{
				return data[index_of_x];
			}

			[[nodiscard]] constexpr value_type r() const noexcept
					requires(size >= index_of_x + 1)
			{
				return data[index_of_x];
			}

			[[nodiscard]] constexpr reference s() noexcept
					requires(size >= index_of_x + 1)
			{
				return data[index_of_x];
			}

			[[nodiscard]] constexpr value_type s() const noexcept
					requires(size >= index_of_x + 1)
			{
				return data[index_of_x];
			}

			[[nodiscard]] constexpr reference y() noexcept
					requires(size >= index_of_y + 1)
			{
				return data[index_of_y];
			}

			[[nodiscard]] constexpr value_type y() const noexcept
					requires(size >= index_of_y + 1)
			{
				return data[index_of_y];
			}

			[[nodiscard]] constexpr reference g() noexcept
					requires(size >= index_of_y + 1)
			{
				return data[index_of_y];
			}

			[[nodiscard]] constexpr value_type g() const noexcept
					requires(size >= index_of_y + 1)
			{
				return data[index_of_y];
			}

			[[nodiscard]] constexpr reference t() noexcept
					requires(size >= index_of_y + 1)
			{
				return data[index_of_y];
			}

			[[nodiscard]] constexpr value_type t() const noexcept
					requires(size >= index_of_y + 1)
			{
				return data[index_of_y];
			}

			[[nodiscard]] constexpr reference z() noexcept
					requires(size >= index_of_z + 1)
			{
				return data[index_of_z];
			}

			[[nodiscard]] constexpr value_type z() const noexcept
					requires(size >= index_of_z + 1)
			{
				return data[index_of_z];
			}

			[[nodiscard]] constexpr reference b() noexcept
					requires(size >= index_of_z + 1)
			{
				return data[index_of_z];
			}

			[[nodiscard]] constexpr value_type b() const noexcept
					requires(size >= index_of_z + 1)
			{
				return data[index_of_z];
			}

			[[nodiscard]] constexpr reference p() noexcept
					requires(size >= index_of_z + 1)
			{
				return data[index_of_z];
			}

			[[nodiscard]] constexpr value_type p() const noexcept
					requires(size >= index_of_z + 1)
			{
				return data[index_of_z];
			}

			[[nodiscard]] constexpr reference w() noexcept
					requires(size >= index_of_w + 1)
			{
				return data[index_of_w];
			}

			[[nodiscard]] constexpr value_type w() const noexcept
					requires(size >= index_of_w + 1)
			{
				return data[index_of_w];
			}

			[[nodiscard]] constexpr reference a() noexcept
					requires(size >= index_of_w + 1)
			{
				return data[index_of_w];
			}

			[[nodiscard]] constexpr value_type a() const noexcept
					requires(size >= index_of_w + 1)
			{
				return data[index_of_w];
			}

			[[nodiscard]] constexpr reference q() noexcept
					requires(size >= index_of_w + 1)
			{
				return data[index_of_w];
			}

			[[nodiscard]] constexpr value_type q() const noexcept
					requires(size >= index_of_w + 1)
			{
				return data[index_of_w];
			}
		};

	#ifdef _MSC_VER
		#pragma warning(push)
		#pragma warning(disable : 4324 4820)// C4324/4820: padding warnings
	#endif
	#ifdef __GNUC__
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wpadded"
	#endif
	#ifdef __clang__
		#pragma clang diagnostic push
		#pragma clang diagnostic ignored "-Wpadded"
	#endif

		struct alignas(math_type_alignment) float2a : public float2
		{
		};
		struct alignas(math_type_alignment) float3a : public float3
		{
		};
		struct alignas(math_type_alignment) float4a : public float4
		{
		};

		struct alignas(math_type_alignment) int2a : public int2
		{
		};
		struct alignas(math_type_alignment) int3a : public int3
		{
		};
		struct alignas(math_type_alignment) int4a : public int4
		{
		};

		struct alignas(math_type_alignment) uint2a : public uint2
		{
		};
		struct alignas(math_type_alignment) uint3a : public uint3
		{
		};
		struct alignas(math_type_alignment) uint4a : public uint4
		{
		};

	#ifdef _MSC_VER
		#pragma warning(pop)
	#endif
	#ifdef __GNUC__
		#pragma GCC diagnostic pop
	#endif
	#ifdef __clang__
		#pragma clang diagnostic pop
	#endif

		/**
		 * @note:
		 *
		 * |||||||| -> SecondTier == 8
		 * |
		 * |
		 * |
		 * ^ FirstTier == 4
		 * FirstTier == how many rows
		 * SecondTier == how many columns
		 */
		template<typename T, std::size_t FirstTier, std::size_t SecondTier>
		struct generic_two_tier_container
		{
			using value_type					   = T;
			using size_type						   = std::size_t;

			constexpr static size_type first_size  = FirstTier;
			constexpr static size_type second_size = SecondTier;
			constexpr static size_type size		   = first_size * second_size;

			using reference						   = value_type&;

			value_type			data[first_size][second_size];

			constexpr reference operator()(size_type row, size_type column) noexcept
			{
				gal::toolbox::utils::gal_assert(row < first_size, "row out of bound");
				gal::toolbox::utils::gal_assert(column < second_size, "row out of bound");

				return data[row][column];
			}

			constexpr value_type operator()(size_type row, size_type column) const noexcept
			{
				gal::toolbox::utils::gal_assert(row < first_size, "row out of bound");
				gal::toolbox::utils::gal_assert(column < second_size, "row out of bound");

				return data[row][column];
			}
		};

	#ifdef _MSC_VER
		#pragma warning(push)
		#pragma warning(disable : 4324 4820)// C4324/4820: padding warnings
	#endif
	#ifdef __GNUC__
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wpadded"
	#endif
	#ifdef __clang__
		#pragma clang diagnostic push
		#pragma clang diagnostic ignored "-Wpadded"
	#endif

		struct alignas(math_type_alignment) float3x3a : public float3x3
		{
		};
		struct alignas(math_type_alignment) float3x4a : public float3x4
		{
		};
		struct alignas(math_type_alignment) float4x3a : public float4x3
		{
		};
		struct alignas(math_type_alignment) float4x4a : public float4x4
		{
		};

	#ifdef _MSC_VER
		#pragma warning(pop)
	#endif
	#ifdef __GNUC__
		#pragma GCC diagnostic pop
	#endif
	#ifdef __clang__
		#pragma clang diagnostic pop
	#endif

		template<typename Target, typename Tuple, typename Target::size_type... I>
		requires(vector_or_matrix_t<Target> || one_or_two_tier_container_ignore_aligned_t<Target>) && (sizeof...(I) <= Target::size) constexpr Target make(Tuple&& tuple, std::index_sequence<I...>) noexcept
		{
			return {static_cast<typename Target::value_type>(std::get<I>(std::forward<Tuple>(tuple)))...};
		}

		template<typename Target, typename... Args>
		requires(vector_or_matrix_t<Target> || one_or_two_tier_container_ignore_aligned_t<Target>) && (sizeof...(Args) <= Target::size) constexpr Target make(Args... args) noexcept
		{
			return make<Target>(std::tuple_cat(gal::toolbox::utils::tuple_maker::to_tuple(args)...), std::make_index_sequence<std::min(Target::size, std::tuple_size_v<decltype(std::tuple_cat(gal::toolbox::utils::tuple_maker::to_tuple(args)...))>)>{});
		}
	}// namespace data_types

	inline namespace constants
	{
		// pi / 4
		constexpr static float		   quarter_pi	   = 0.785'398'163f;
		// pi / 2
		constexpr static float		   half_pi		   = 1.570'796'326f;
		// pi
		constexpr static float		   pi			   = 3.141'592'653f;
		// 2 * pi
		constexpr static float		   two_pi		   = 6.283'185'307f;

		// 1 / pi
		constexpr static float		   one_over_pi	   = 0.318'309'886f;
		// 1 / (2 * pi)
		constexpr static float		   one_over_two_pi = 0.159'154'943f;

		constexpr static std::uint32_t select_0		   = 0x00000000;
		constexpr static std::uint32_t select_1		   = 0xFFFFFFFF;

		constexpr static f32_vector	   g_vector_sin_coefficients0{-0.16666667f, +0.0083333310f, -0.00019840874f, +2.7525562e-06f};
		constexpr static f32_vector	   g_vector_sin_coefficients1{-2.3889859e-08f, -0.16665852f /*Est1*/, +0.0083139502f /*Est2*/, -0.00018524670f /*Est3*/};
		constexpr static f32_vector	   g_vector_cos_coefficients0{-0.5f, +0.041666638f, -0.0013888378f, +2.4760495e-05f};
		constexpr static f32_vector	   g_vector_cos_coefficients1{-2.6051615e-07f, -0.49992746f /*Est1*/, +0.041493919f /*Est2*/, -0.0012712436f /*Est3*/};
		constexpr static f32_vector	   g_vector_tan_coefficients0{1.0f, 0.333333333f, 0.133333333f, 5.396825397e-2f};
		constexpr static f32_vector	   g_vector_tan_coefficients1{2.186948854e-2f, 8.863235530e-3f, 3.592128167e-3f, 1.455834485e-3f};
		constexpr static f32_vector	   g_vector_tan_coefficients2{5.900274264e-4f, 2.391290764e-4f, 9.691537707e-5f, 3.927832950e-5f};
		constexpr static f32_vector	   g_vector_arc_coefficients0{+1.5707963050f, -0.2145988016f, +0.0889789874f, -0.0501743046f};
		constexpr static f32_vector	   g_vector_arc_coefficients1{+0.0308918810f, -0.0170881256f, +0.0066700901f, -0.0012624911f};
		constexpr static f32_vector	   g_vector_atan_coefficients0{-0.3333314528f, +0.1999355085f, -0.1420889944f, +0.1065626393f};
		constexpr static f32_vector	   g_vector_atan_coefficients1{-0.0752896400f, +0.0429096138f, -0.0161657367f, +0.0028662257f};
		constexpr static f32_vector	   g_vector_atan_est_coefficients0{+0.999866f, +0.999866f, +0.999866f, +0.999866f};
		constexpr static f32_vector	   g_vector_atan_est_coefficients1{-0.3302995f, +0.180141f, -0.085133f, +0.0208351f};
		constexpr static f32_vector	   g_vector_tan_est_coefficients{2.484f, -1.954923183e-1f, 2.467401101f, pi};
		constexpr static f32_vector	   g_vector_arc_est_coefficients{+1.5707288f, -0.2121144f, +0.0742610f, -0.0187293f};
		constexpr static f32_vector	   g_vector_identity_r0{1.0f, 0.0f, 0.0f, 0.0f};
		constexpr static f32_vector	   g_vector_identity_r1{0.0f, 1.0f, 0.0f, 0.0f};
		constexpr static f32_vector	   g_vector_identity_r2{0.0f, 0.0f, 1.0f, 0.0f};
		constexpr static f32_vector	   g_vector_identity_r3{0.0f, 0.0f, 0.0f, 1.0f};
		constexpr static f32_vector	   g_vector_neg_identity_r0{-1.0f, 0.0f, 0.0f, 0.0f};
		constexpr static f32_vector	   g_vector_neg_identity_r1{0.0f, -1.0f, 0.0f, 0.0f};
		constexpr static f32_vector	   g_vector_neg_identity_r2{0.0f, 0.0f, -1.0f, 0.0f};
		constexpr static f32_vector	   g_vector_neg_identity_r3{0.0f, 0.0f, 0.0f, -1.0f};
		constexpr static u32_vector	   g_vector_neg_zero{0x80000000, 0x80000000, 0x80000000, 0x80000000};
		constexpr static u32_vector	   g_vector_neg_zero_xyz{0x80000000, 0x80000000, 0x80000000, 0x00000000};
		constexpr static u32_vector	   g_vector_mask{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
		constexpr static u32_vector	   g_vector_mask_xyz{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000};
		constexpr static u32_vector	   g_vector_mask_xy{0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000};
		constexpr static u32_vector	   g_vector_mask_x{0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000};
		constexpr static u32_vector	   g_vector_mask_y{0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000};
		constexpr static u32_vector	   g_vector_mask_z{0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000000};
		constexpr static u32_vector	   g_vector_mask_w{0x00000000, 0x00000000, 0x00000000, 0xFFFFFFFF};
		constexpr static f32_vector	   g_vector_zero{0.0f, 0.0f, 0.0f, 0.0f};
		constexpr static f32_vector	   g_vector_one{1.0f, 1.0f, 1.0f, 1.0f};
		constexpr static f32_vector	   g_vector_one_xyz{1.0f, 1.0f, 1.0f, 0.0f};
		constexpr static f32_vector	   g_vector_neg_one{-1.0f, -1.0f, -1.0f, -1.0f};
		constexpr static f32_vector	   g_vector_half_one{0.5f, 0.5f, 0.5f, 0.5f};
		constexpr static f32_vector	   g_vector_neg_half_one{-0.5f, -0.5f, -0.5f, -0.5f};
		constexpr static f32_vector	   g_vector_quarter_pi{quarter_pi, quarter_pi, quarter_pi, quarter_pi};
		constexpr static f32_vector	   g_vector_half_pi{half_pi, half_pi, half_pi, half_pi};
		constexpr static f32_vector	   g_vector_pi{pi, pi, pi, pi};
		constexpr static f32_vector	   g_vector_two_pi{two_pi, two_pi, two_pi, two_pi};
		constexpr static f32_vector	   g_vector_neg_quarter_pi{-quarter_pi, -quarter_pi, -quarter_pi, -quarter_pi};
		constexpr static f32_vector	   g_vector_neg_half_pi{-half_pi, -half_pi, -half_pi, -half_pi};
		constexpr static f32_vector	   g_vector_neg_pi{-pi, -pi, -pi, -pi};
		constexpr static f32_vector	   g_vector_neg_two_pi{-two_pi, -two_pi, -two_pi, -two_pi};
		constexpr static f32_vector	   g_vector_reciprocal_pi{one_over_pi, one_over_pi, one_over_pi, one_over_pi};
		constexpr static f32_vector	   g_vector_reciprocal_two_pi{one_over_two_pi, one_over_two_pi, one_over_two_pi, one_over_two_pi};
		constexpr static f32_vector	   g_vector_neg_reciprocal_pi{-one_over_pi, -one_over_pi, -one_over_pi, -one_over_pi};
		constexpr static f32_vector	   g_vector_neg_reciprocal_two_pi{-one_over_two_pi, -one_over_two_pi, -one_over_two_pi, -one_over_two_pi};
		constexpr static f32_vector	   g_vector_epsilon{1.192092896e-7f, 1.192092896e-7f, 1.192092896e-7f, 1.192092896e-7f};
		constexpr static i32_vector	   g_vector_half_one_minus_epsilon{0x3EFFFFFD, 0x3EFFFFFD, 0x3EFFFFFD, 0x3EFFFFFD};
		constexpr static i32_vector	   g_vector_infinity{0x7F800000, 0x7F800000, 0x7F800000, 0x7F800000};
		constexpr static i32_vector	   g_vector_nan{0x7FC00000, 0x7FC00000, 0x7FC00000, 0x7FC00000};
		constexpr static i32_vector	   g_vector_nan_test{0x007FFFFF, 0x007FFFFF, 0x007FFFFF, 0x007FFFFF};
		constexpr static i32_vector	   g_vector_abs_mask{0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF};
		constexpr static i32_vector	   g_vector_float_min{0x00800000, 0x00800000, 0x00800000, 0x00800000};
		constexpr static i32_vector	   g_vector_float_max{0x7F7FFFFF, 0x7F7FFFFF, 0x7F7FFFFF, 0x7F7FFFFF};
		constexpr static u32_vector	   g_vector_neg_one_mask{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
		constexpr static u32_vector	   g_vector_mask_a8r8g8b8{0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000};
		constexpr static u32_vector	   g_vector_flip_a8r8g8b8{0x00000000, 0x00000000, 0x00000000, 0x80000000};
		constexpr static f32_vector	   g_vector_fix_a8r8g8b8{0.0f, 0.0f, 0.0f, float(0x80000000U)};
		constexpr static f32_vector	   g_vector_normalize_a8r8g8b8{1.0f / (255.0f * float(0x10000)), 1.0f / (255.0f * float(0x100)), 1.0f / 255.0f, 1.0f / (255.0f * float(0x1000000))};
		constexpr static u32_vector	   g_vector_mask_a2b10g10r10{0x000003FF, 0x000FFC00, 0x3FF00000, 0xC0000000};
		constexpr static u32_vector	   g_vector_flip_a2b10g10r10{0x00000200, 0x00080000, 0x20000000, 0x80000000};
		constexpr static f32_vector	   g_vector_fix_a2b10g10r10{-512.0f, -512.0f * float(0x400), -512.0f * float(0x100000), float(0x80000000U)};
		constexpr static f32_vector	   g_vector_normalize_a2b10g10r10{1.0f / 511.0f, 1.0f / (511.0f * float(0x400)), 1.0f / (511.0f * float(0x100000)), 1.0f / (3.0f * float(0x40000000))};
		constexpr static u32_vector	   g_vector_mask_x16y16{0x0000FFFF, 0xFFFF0000, 0x00000000, 0x00000000};
		constexpr static i32_vector	   g_vector_flip_x16y16{0x00008000, 0x00000000, 0x00000000, 0x00000000};
		constexpr static f32_vector	   g_vector_fix_x16y16{-32768.0f, 0.0f, 0.0f, 0.0f};
		constexpr static f32_vector	   g_vector_normalize_x16y16{1.0f / 32767.0f, 1.0f / (32767.0f * 65536.0f), 0.0f, 0.0f};
		constexpr static u32_vector	   g_vector_mask_x16y16z16w16{0x0000FFFF, 0x0000FFFF, 0xFFFF0000, 0xFFFF0000};
		constexpr static i32_vector	   g_vector_flip_x16y16z16w16{0x00008000, 0x00008000, 0x00000000, 0x00000000};
		constexpr static f32_vector	   g_vector_fix_x16y16z16w16{-32768.0f, -32768.0f, 0.0f, 0.0f};
		constexpr static f32_vector	   g_vector_normalize_x16y16z16w16{1.0f / 32767.0f, 1.0f / 32767.0f, 1.0f / (32767.0f * 65536.0f), 1.0f / (32767.0f * 65536.0f)};
		constexpr static f32_vector	   g_vector_no_fraction{8388608.0f, 8388608.0f, 8388608.0f, 8388608.0f};
		constexpr static i32_vector	   g_vector_mask_byte{0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF};
		constexpr static f32_vector	   g_vector_neg_x{-1.0f, 1.0f, 1.0f, 1.0f};
		constexpr static f32_vector	   g_vector_neg_y{1.0f, -1.0f, 1.0f, 1.0f};
		constexpr static f32_vector	   g_vector_neg_z{1.0f, 1.0f, -1.0f, 1.0f};
		constexpr static f32_vector	   g_vector_neg_w{1.0f, 1.0f, 1.0f, -1.0f};
		constexpr static u32_vector	   g_vector_select_0101{select_0, select_1, select_0, select_1};
		constexpr static u32_vector	   g_vector_select_1010{select_1, select_0, select_1, select_0};
		constexpr static u32_vector	   g_vector_select_1000{select_1, select_0, select_0, select_0};
		constexpr static u32_vector	   g_vector_select_1100{select_1, select_1, select_0, select_0};
		constexpr static u32_vector	   g_vector_select_1110{select_1, select_1, select_1, select_0};
		constexpr static u32_vector	   g_vector_select_1011{select_1, select_0, select_1, select_1};
		constexpr static f32_vector	   g_vector_fixup_y16{1.0f, 1.0f / 65536.0f, 0.0f, 0.0f};
		constexpr static f32_vector	   g_vector_fixup_y16w16{1.0f, 1.0f, 1.0f / 65536.0f, 1.0f / 65536.0f};
		constexpr static u32_vector	   g_vector_flip_y{0, 0x80000000, 0, 0};
		constexpr static u32_vector	   g_vector_flip_z{0, 0, 0x80000000, 0};
		constexpr static u32_vector	   g_vector_flip_w{0, 0, 0, 0x80000000};
		constexpr static u32_vector	   g_vector_flip_yz{0, 0x80000000, 0x80000000, 0};
		constexpr static u32_vector	   g_vector_flip_zw{0, 0, 0x80000000, 0x80000000};
		constexpr static u32_vector	   g_vector_flip_yw{0, 0x80000000, 0, 0x80000000};
		constexpr static i32_vector	   g_vector_mask_dec4{0x3FF, 0x3FF << 10, 0x3FF << 20, static_cast<int>(0xC0000000)};
		constexpr static i32_vector	   g_vector_xor_dec4{0x200, 0x200 << 10, 0x200 << 20, 0};
		constexpr static f32_vector	   g_vector_add_u_dec4{0, 0, 0, 32768.0f * 65536.0f};
		constexpr static f32_vector	   g_vector_add_dec4{-512.0f, -512.0f * 1024.0f, -512.0f * 1024.0f * 1024.0f, 0};
		constexpr static f32_vector	   g_vector_mul_dec4{1.0f, 1.0f / 1024.0f, 1.0f / (1024.0f * 1024.0f), 1.0f / (1024.0f * 1024.0f * 1024.0f)};
		constexpr static u32_vector	   g_vector_mask_byte4{0xFF, 0xFF00, 0xFF0000, 0xFF000000};
		constexpr static i32_vector	   g_vector_xor_byte4{0x80, 0x8000, 0x800000, 0x00000000};
		constexpr static f32_vector	   g_vector_add_byte4{-128.0f, -128.0f * 256.0f, -128.0f * 65536.0f, 0};
		constexpr static f32_vector	   g_vector_fix_unsigned{32768.0f * 65536.0f, 32768.0f * 65536.0f, 32768.0f * 65536.0f, 32768.0f * 65536.0f};
		constexpr static f32_vector	   g_vector_max_int{65536.0f * 32768.0f - 128.0f, 65536.0f * 32768.0f - 128.0f, 65536.0f * 32768.0f - 128.0f, 65536.0f * 32768.0f - 128.0f};
		constexpr static f32_vector	   g_vector_max_uint{65536.0f * 65536.0f - 256.0f, 65536.0f * 65536.0f - 256.0f, 65536.0f * 65536.0f - 256.0f, 65536.0f * 65536.0f - 256.0f};
		constexpr static f32_vector	   g_vector_unsigned_fix{32768.0f * 65536.0f, 32768.0f * 65536.0f, 32768.0f * 65536.0f, 32768.0f * 65536.0f};


	}// namespace constants
}// namespace gal::toolbox::math

	#include <math/details/math_utils.inl>
	#include <math/details/math_vector.hpp>

#endif
