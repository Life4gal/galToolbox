#pragma once

#include "math_fwd.hpp"
#include "vector.hpp"
#include "matrix.hpp"
#include "math_apply.hpp"
#include "../utils/tuple_maker.hpp"
// break abi to save cmath
#include <cmath>

namespace gal::test::math
{
	template <not_math_type_t T1, not_math_type_t T2>
	constexpr void pow(T1& base, T2 exponent)
	noexcept(noexcept(
		std::pow(std::declval<T1>(), std::declval<T2>())
	))
	{
		base = std::pow(base, exponent);
	}

	/**
	 * @brief returns 'base' raised to the power 'exponent'.
	 * @param base vector
	 * @param exponent vector/vector_view
	 * @return vector
	*/
	template <math_mathematical_vector_t T1, math_vector_same_size_t<T1> T2>
	constexpr T1& make_pow(T1& base, const T2& exponent)
	noexcept(noexcept(
		operator_base(
					std::declval<T1&>(),
					std::declval<const T2&>(),
					pow<math_value_type<T1>, math_value_type<T2>>
					)
	))
	{
		operator_base(
					base,
					exponent,
					pow<math_value_type<T1>, math_value_type<T2>>
					);
		return base;
	}

	/**
	* @brief returns 'base' raised to the power 'exponent'.
	* @param base vector_view
	* @param exponent vector/vector_view
	* @return vector
	*/
	template <math_mathematical_vector_view_t T1, math_vector_same_size_t<T1> T2>
	constexpr T1 make_pow(T1 base, const T2& exponent)
	noexcept(noexcept(
		operator_base(
					std::declval<T1&>(),
					std::declval<const T2&>(),
					pow<math_value_type<T1>, math_value_type<T2>>
					)
	))
	{
		operator_base(
					base,
					exponent,
					pow<math_value_type<T1>, math_value_type<T2>>
					);
		return base;
	}

	/**
	* @brief returns 'base' raised to the power 'exponent'.
	* @param base vector/vector_view
	* @param exponent vector/vector_view
	* @return vector
	*/
	template <math_mathematical_vector_type_t T1, math_vector_same_size_t<T1> T2>
	constexpr auto pow(const T1& base, const T2& exponent)
	noexcept(noexcept(
		make_pow(
				std::declval<std::add_lvalue_reference_t<decltype(base.copy())>>(),
				std::declval<const T2&>()
				)
	))
	{
		auto ret = base.copy();
		make_pow(ret, exponent);
		return ret;
	}

	template <not_math_type_t T>
	constexpr void exp(T& v)
	noexcept(noexcept(
		std::exp(std::declval<T>())
	))
	{
		v = std::exp(v);
	}

	/**
	* @brief return the natural exponentiation of x, i.e., e^x.
	* @param v exp function is defined for input values of v defined in the range (inf-, inf+) in the limit of the type qualifier.
	* @return vector
	*/
	template <math_mathematical_vector_t T>
	constexpr T& make_exp(T& v)
	noexcept(noexcept(
		operator_base(
					std::declval<T&>(),
					exp<math_value_type<T>>
					)
	))
	{
		operator_base(
					v,
					exp<math_value_type<T>>
					);
		return v;
	}

	/**
	* @brief return the natural exponentiation of x, i.e., e^x.
	* @param v exp function is defined for input values of v defined in the range (inf-, inf+) in the limit of the type qualifier.
	* @return vector
	*/
	template <math_mathematical_vector_view_t T>
	constexpr T make_exp(T v)
	noexcept(noexcept(
		operator_base(
					std::declval<T>(),
					exp<math_value_type<T>>
					)
	))
	{
		operator_base(
					v,
					exp<math_value_type<T>>
					);
		return v;
	}

	/**
	* @brief return the natural exponentiation of x, i.e., e^x.
	* @param v exp function is defined for input values of v defined in the range (inf-, inf+) in the limit of the type qualifier.
	* @return vector
	*/
	template <math_mathematical_vector_type_t T>
	constexpr auto exp(const T& v)
	noexcept(noexcept(
		make_exp(std::declval<std::add_lvalue_reference_t<decltype(v.copy())>>())
	))
	{
		auto ret = v.copy();
		make_exp(ret);
		return ret;
	}

	template <not_math_type_t T>
	constexpr void exp2(T& v)
	noexcept(noexcept(
		std::exp2(std::declval<T>())
	))
	{
		v = std::exp2(v);
	}

	/**
	* @brief return 2 raised to the v power.
	* @param v exp2 function is defined for input values of v defined in the range (inf-, inf+) in the limit of the type qualifier.
	* @return vector
	*/
	template <math_mathematical_vector_t T>
	constexpr T& make_exp2(T& v)
	noexcept(noexcept(
		operator_base(
					std::declval<T&>(),
					exp2<math_value_type<T>>
					)
	))
	{
		operator_base(
					v,
					exp2<math_value_type<T>>
					);
		return v;
	}

	/**
	* @brief return 2 raised to the v power.
	* @param v exp2 function is defined for input values of v defined in the range (inf-, inf+) in the limit of the type qualifier.
	* @return vector
	*/
	template <math_mathematical_vector_view_t T>
	constexpr T make_exp2(T v)
	noexcept(noexcept(
		operator_base(
					std::declval<T>(),
					exp2<math_value_type<T>>
					)
	))
	{
		operator_base(
					v,
					exp2<math_value_type<T>>
					);
		return v;
	}

	/**
	* @brief return 2 raised to the v power.
	* @param v exp2 function is defined for input values of v defined in the range (inf-, inf+) in the limit of the type qualifier.
	* @return vector
	*/
	template <math_mathematical_vector_type_t T>
	constexpr auto exp2(const T& v)
	noexcept(noexcept(
		make_exp2(std::declval<std::add_lvalue_reference_t<decltype(v.copy())>>())
	))
	{
		auto ret = v.copy();
		make_exp2(ret);
		return ret;
	}

	template <not_math_type_t T>
	constexpr void log(T& v)
	noexcept(noexcept(
		std::log(std::declval<T>())
	))
	{
		v = std::log(v);
	}

	/**
	* @brief return the natural logarithm of v, i.e.,
	* returns the value y which satisfies the equation x = e^y.
	* results are undefined if v <= 0.
	* @param v log function is defined for input values of v defined in the range (0, inf+) in the limit of the type qualifier.
	* @return vector
	*/
	template <math_mathematical_vector_t T>
	constexpr T& make_log(T& v)
	noexcept(noexcept(
		operator_base(
					std::declval<T&>(),
					log<math_value_type<T>>
					)
	))
	{
		operator_base(
					v,
					log<math_value_type<T>>
					);
		return v;
	}

	/**
	* @brief return the natural logarithm of v, i.e.,
	* returns the value y which satisfies the equation x = e^y.
	* results are undefined if v <= 0.
	* @param v log function is defined for input values of v defined in the range (0, inf+) in the limit of the type qualifier.
	* @return vector
	*/
	template <math_mathematical_vector_view_t T>
	constexpr T make_log(T v)
	noexcept(noexcept(
		operator_base(
					std::declval<T>(),
					log<math_value_type<T>>
					)
	))
	{
		operator_base(
					v,
					log<math_value_type<T>>
					);
		return v;
	}

	/**
	* @brief return the natural logarithm of v, i.e.,
	* returns the value y which satisfies the equation x = e^y.
	* results are undefined if v <= 0.
	* @param v log function is defined for input values of v defined in the range (0, inf+) in the limit of the type qualifier.
	* @return vector
	*/
	template <math_mathematical_vector_type_t T>
	constexpr auto log(const T& v)
	noexcept(noexcept(
		make_log(std::declval<std::add_lvalue_reference_t<decltype(v.copy())>>())
	))
	{
		auto ret = v.copy();
		make_log(ret);
		return ret;
	}

	template <not_math_type_t T>
	constexpr void log2(T& v)
	noexcept(noexcept(
		std::log2(std::declval<T>())
	))
	{
		v = std::log2(v);
	}

	/**
	* @brief return the the base 2 log of x, i.e., returns the value y,
	* which satisfies the equation x = 2 ^ y.
	* @param v log2 function is defined for input values of v defined in the range (0, inf+) in the limit of the type qualifier.
	* @return vector
	*/
	template <math_mathematical_vector_t T>
	constexpr T& make_log2(T& v)
	noexcept(noexcept(
		operator_base(
					std::declval<T&>(),
					log2<math_value_type<T>>
					)
	))
	{
		operator_base(
					v,
					log2<math_value_type<T>>
					);
		return v;
	}

	/**
	* @brief return the natural logarithm of v, i.e.,
	* returns the value y which satisfies the equation x = e^y.
	* results are undefined if v <= 0.
	* @param v log2 function is defined for input values of v defined in the range (0, inf+) in the limit of the type qualifier.
	* @return vector
	*/
	template <math_mathematical_vector_view_t T>
	constexpr T make_log2(T v)
	noexcept(noexcept(
		operator_base(
					std::declval<T>(),
					log2<math_value_type<T>>
					)
	))
	{
		operator_base(
					v,
					log2<math_value_type<T>>
					);
		return v;
	}

	/**
	* @brief return the natural logarithm of v, i.e.,
	* returns the value y which satisfies the equation x = e^y.
	* results are undefined if v <= 0.
	* @param v log2 function is defined for input values of v defined in the range (0, inf+) in the limit of the type qualifier.
	* @return vector
	*/
	template <math_mathematical_vector_type_t T>
	constexpr auto log2(const T& v)
	noexcept(noexcept(
		make_log2(std::declval<std::add_lvalue_reference_t<decltype(v.copy())>>())
	))
	{
		auto ret = v.copy();
		make_log2(ret);
		return ret;
	}

	template <not_math_type_t T>
	constexpr void sqrt(T& v)
	noexcept(noexcept(
		std::sqrt(std::declval<T>())
	))
	{
		v = std::sqrt(v);
	}

	/**
	* @brief return the positive square root of v.
	* @param v sqrt function is defined for input values of v defined in the range [0, inf+) in the limit of the type qualifier.
	* @return vector
	*/
	template <math_mathematical_vector_t T>
	constexpr T& make_sqrt(T& v)
	noexcept(noexcept(
		operator_base(
					std::declval<T&>(),
					sqrt<math_value_type<T>>
					)
	))
	{
		operator_base(
					v,
					sqrt<math_value_type<T>>
					);
		return v;
	}

	/**
	* @brief return the positive square root of v.
	* @param v sqrt function is defined for input values of v defined in the range [0, inf+) in the limit of the type qualifier.
	* @return vector
	*/
	template <math_mathematical_vector_view_t T>
	constexpr T make_sqrt(T v)
	noexcept(noexcept(
		operator_base(
					std::declval<T>(),
					sqrt<math_value_type<T>>
					)
	))
	{
		operator_base(
					v,
					sqrt<math_value_type<T>>
					);
		return v;
	}

	/**
	* @brief return the positive square root of v.
	* @param v sqrt function is defined for input values of v defined in the range [0, inf+) in the limit of the type qualifier.
	* @return vector
	*/
	template <math_mathematical_vector_type_t T>
	constexpr auto sqrt(const T& v)
	noexcept(noexcept(
		make_sqrt(std::declval<std::add_lvalue_reference_t<decltype(v.copy())>>())
	))
	{
		auto ret = v.copy();
		make_sqrt(ret);
		return ret;
	}

	template <not_math_type_t T>
	constexpr void inverse_sqrt(float& v) noexcept
	{
		if constexpr (std::is_same_v<T, float>)
		{
			const auto magic = 0x5f375a86;

			const float half = v * 0.5f;
			// bit hack

			auto hack =
				*reinterpret_cast<unsigned int*>(&v); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast, clang-diagnostic-undefined-reinterpret-cast)
			hack = magic - (hack >> 1);
			v =
				*reinterpret_cast<float*>(&hack); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast, clang-diagnostic-undefined-reinterpret-cast)
			// Newton Iteration
			v *= 1.5f - half * v * v;
		}
		else
		{
			v = static_cast<T>(1) / std::sqrt(v);
		}
	}

	/**
	* @brief return the reciprocal of the positive square root of v.
	* @param v inverse sqrt function is defined for input values of v defined in the range [0, inf+) in the limit of the type qualifier.
	* @return vector
	*/
	template <math_mathematical_vector_t T>
	constexpr T& make_inverse_sqrt(T& v)
	noexcept(noexcept(
		operator_base(
					std::declval<T&>(),
					inverse_sqrt<math_value_type<T>>
					)
	))
	{
		operator_base(
					v,
					inverse_sqrt<math_value_type<T>>
					);
		return v;
	}

	/**
	* @brief return the reciprocal of the positive square root of v.
	* @param v inverse sqrt function is defined for input values of v defined in the range [0, inf+) in the limit of the type qualifier.
	* @return vector
	*/
	template <math_mathematical_vector_view_t T>
	constexpr T make_inverse_sqrt(T v)
	noexcept(noexcept(
		operator_base(
					std::declval<T>(),
					inverse_sqrt<math_value_type<T>>
					)
	))
	{
		operator_base(
					v,
					inverse_sqrt<math_value_type<T>>
					);
		return v;
	}

	/**
	* @brief return the reciprocal of the positive square root of v.
	* @note the reason why make_inverse_sqrt does not exist is that it will not bring benefits (creating additional vector is unavoidable)
	* @param v inverse sqrt function is defined for input values of v defined in the range [0, inf+) in the limit of the type qualifier.
	* @return vector
	*/
	template <math_mathematical_vector_type_t T>
	constexpr auto inverse_sqrt(const T& v)
	noexcept(noexcept(
		std::declval<math_value_type<T>>() / std::declval<const T&>()
	))
	{
		auto ret = v.copy();
		operator_base(
					ret,
					inverse_sqrt<math_value_type<T>>
					);
		return ret;
	}
}
