#pragma once

#include "math_fwd.hpp"
#include "vector.hpp"
#include "matrix.hpp"
#include "math_apply.hpp"
#include "../utils/tuple_maker.hpp"
#include <cmath>

namespace gal::test::math
{
	using std::pow;

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
					std::declval<decltype(
						pow
					)>()
					)
	))
	{
		operator_base(
					base,
					exponent,
					pow
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
					std::declval<decltype(
						pow
					)>()
					)
	))
	{
		operator_base(
					base,
					exponent,
					pow
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
		make_pow(std::declval<std::add_lvalue_reference_t<decltype(base.copy())>>())
	))
	{
		auto ret = base.copy();
		make_pow(ret, exponent);
		return ret;
	}

	using std::exp;

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
					std::declval<decltype(
						exp
					)>()
					)
	))
	{
		operator_base(
					v,
					exp
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
					std::declval<decltype(
						exp
					)>()
					)
	))
	{
		operator_base(
					v,
					exp
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
		auto ret = v.copy;
		make_exp(ret);
		return ret;
	}

	using std::exp2;

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
					std::declval<decltype(
						exp2
					)>()
					)
	))
	{
		operator_base(
					v,
					exp2
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
					std::declval<decltype(
						exp2
					)>()
					)
	))
	{
		operator_base(
					v,
					exp2
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
		auto ret = v.copy;
		make_exp2(ret);
		return ret;
	}

	using std::log;

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
					std::declval<decltype(
						log
					)>()
					)
	))
	{
		operator_base(
					v,
					log
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
					std::declval<decltype(
						log
					)>()
					)
	))
	{
		operator_base(
					v,
					log
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
		auto ret = v.copy;
		make_log(ret);
		return ret;
	}

	using std::log2;

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
					std::declval<decltype(
						log2
					)>()
					)
	))
	{
		operator_base(
					v,
					log2
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
					std::declval<decltype(
						log2
					)>()
					)
	))
	{
		operator_base(
					v,
					log2
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
		auto ret = v.copy;
		make_log2(ret);
		return ret;
	}

	using std::sqrt;

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
					std::declval<decltype(
						sqrt
					)>()
					)
	))
	{
		operator_base(
					v,
					sqrt
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
					std::declval<decltype(
						sqrt
					)>()
					)
	))
	{
		operator_base(
					v,
					sqrt
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
		auto ret = v.copy;
		make_sqrt(ret);
		return ret;
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
		if constexpr (std::is_same_v<math_value_type<T>, float>)
		{
			constexpr static typename T::template copy_type<unsigned int> magic{
				utils::tuple_maker::duplicate<T::data_size>(0x5f375a86), std::make_index_sequence<T::data_size>{}
			};

			auto half = v.copy() * 0.5f;

			const auto* p   = reinterpret_cast<const typename T::template copy_type<unsigned int>*>(&v); // bit hack
			auto        ret = reinterpret_cast<T>(magic - (*p >> 1)); // Newton Iteration

			// ret *= 1.5f - half * ret * ret;
			// return ret;

			half *= ret;
			half *= ret;
			half *= ret;
			half *= 2;
			return half;
		}
		else
		{
			return static_cast<math_value_type<T>>(1) / sqrt(v);
		}
	}
}
