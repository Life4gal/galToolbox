#pragma once

#include "math_fwd.hpp"
#include "vector.hpp"
#include "matrix.hpp"
#include "math_apply.hpp"
#include <cmath>

namespace gal::test::math
{
	/**
	 * @brief converts degrees to radians and returns the result.
	 * @param degrees vector
	 * @return vector
	*/
	template <math_mathematical_vector_t T>
	constexpr T& make_radians(T& degrees)
	noexcept(noexcept(
		operator_base(
					std::declval<T&>(),
					std::declval<decltype(
						[](math_value_type<T>& d)
						{
							d *= static_cast<math_value_type<T>>(0.01745329251994329576923690768489);
						}
					)>()
					)
	))
	{
		operator_base(
					degrees,
					[](math_value_type<T>& d)
					{
						d *= static_cast<math_value_type<T>>(0.01745329251994329576923690768489);
					}
					);
		return degrees;
	}

	/**
	 * @brief converts degrees to radians and returns the result.
	 * @param degrees vector_view
	 * @return vector
	*/
	template <math_mathematical_vector_view_t T>
	constexpr T make_radians(T degrees)
	noexcept(noexcept(
		operator_base(
					std::declval<T>(),
					std::declval<decltype(
						[](math_value_type<T>& d)
						{
							d *= static_cast<math_value_type<T>>(0.01745329251994329576923690768489);
						}
					)>()
					)
	))
	{
		operator_base(
					degrees,
					[](math_value_type<T>& d)
					{
						d *= static_cast<math_value_type<T>>(0.01745329251994329576923690768489);
					}
					);
		return degrees;
	}

	/**
	 * @brief converts degrees to radians and returns the result.
	 * @param degrees vector/vector_view
	 * @return vector
	*/
	template <math_mathematical_vector_type_t T>
	constexpr auto radians(const T& degrees)
	noexcept(noexcept(
		make_radians(std::declval<std::add_lvalue_reference_t<decltype(degrees.copy())>>())
	))
	{
		auto ret = degrees.copy();
		make_radians(ret);
		return ret;
	}

	/**
	 * @brief converts radians to degrees and returns the result.
	 * @param radians vector
	 * @return vector
	*/
	template <math_mathematical_vector_t T>
	constexpr T& make_degrees(T& radians)
	noexcept(noexcept(
		operator_base(
					std::declval<T&>(),
					std::declval<decltype(
						[](math_value_type<T>& r)
						{
							r *= static_cast<math_value_type<T>>(57.295779513082320876798154814105);
						}
					)>()
					)
	))
	{
		operator_base(
					radians,
					[](math_value_type<T>& r)
					{
						r *= static_cast<math_value_type<T>>(57.295779513082320876798154814105);
					}
					);
		return radians;
	}

	/**
	 * @brief converts radians to degrees and returns the result.
	 * @param radians vector_view
	 * @return vector
	*/
	template <math_mathematical_vector_view_t T>
	constexpr T make_degrees(T radians)
	noexcept(noexcept(
		operator_base(
					std::declval<T>(),
					std::declval<decltype(
						[](math_value_type<T>& r)
						{
							r *= static_cast<math_value_type<T>>(57.295779513082320876798154814105);
						}
					)>()
					)
	))
	{
		operator_base(
					radians,
					[](math_value_type<T>& r)
					{
						r *= static_cast<math_value_type<T>>(57.295779513082320876798154814105);
					}
					);
		return radians;
	}

	/**
	 * @brief converts radians to degrees and returns the result.
	 * @param radians vector/vector_view
	 * @return vector
	*/
	template <math_mathematical_vector_type_t T>
	constexpr auto degrees(const T& radians)
	noexcept(noexcept(
		make_degrees(std::declval<std::add_lvalue_reference_t<decltype(radians.copy())>>())
	))
	{
		auto ret = radians.copy();
		make_degrees(ret);
		return ret;
	}

	template <not_math_type_t T>
	constexpr void sin(T& v)
	noexcept(noexcept(
		std::sin(std::declval<T>())
	))
	{
		v = std::sin(v);
	}

	/**
	 * @brief the standard trigonometric sine function.
	 * @note the values returned by this function will range from [-1, 1].
	 * @param angle vector
	 * @return vector
	*/
	template <math_mathematical_vector_t T>
	constexpr T& make_sin(T& angle)
	noexcept(noexcept(
		operator_base(
					std::declval<T&>(),
					sin<math_value_type<T>>
					)
	))
	{
		operator_base(
					angle,
					sin<math_value_type<T>>
					);
		return angle;
	}

	/**
	 * @brief the standard trigonometric sine function.
	 * @note the values returned by this function will range from [-1, 1].
	 * @param angle vector_view
	 * @return vector
	*/
	template <math_mathematical_vector_view_t T>
	constexpr T make_sin(T angle)
	noexcept(noexcept(
		operator_base(
					std::declval<T>(),
					sin<math_value_type<T>>
					)
	))
	{
		operator_base(
					angle,
					sin<math_value_type<T>>
					);
		return angle;
	}

	/**
	 * @brief the standard trigonometric sine function.
	 * @note the values returned by this function will range from [-1, 1].
	 * @param angle vector/vector_view
	 * @return vector
	*/
	template <math_mathematical_vector_type_t T>
	constexpr auto sin(const T& angle)
	noexcept(noexcept(
		make_sin(std::declval<std::add_lvalue_reference_t<decltype(angle.copy())>>())
	))
	{
		auto ret = angle.copy();
		make_sin(ret);
		return ret;
	}

	template <not_math_type_t T>
	constexpr void sinh(T& v)
	noexcept(noexcept(
		std::sinh(std::declval<T>())
	))
	{
		v = std::sinh(v);
	}

	/**
	 * @brief returns the hyperbolic sine function, (exp(x) - exp(-x)) / 2
	 * @param angle vector
	 * @return vector
	*/
	template <math_mathematical_vector_t T>
	constexpr T& make_sinh(T& angle)
	noexcept(noexcept(
		operator_base(
					std::declval<T&>(),
					sinh<math_value_type<T>>
					)
	))
	{
		operator_base(
					angle,
					sinh<math_value_type<T>>
					);
		return angle;
	}

	/**
	 * @brief returns the hyperbolic sine function, (exp(x) - exp(-x)) / 2
	 * @param angle vector_view
	 * @return vector
	*/
	template <math_mathematical_vector_view_t T>
	constexpr T make_sinh(T angle)
	noexcept(noexcept(
		operator_base(
					std::declval<T>(),
					sinh<math_value_type<T>>
					)
	))
	{
		operator_base(
					angle,
					sinh<math_value_type<T>>
					);
		return angle;
	}

	/**
	 * @brief returns the hyperbolic sine function, (exp(x) - exp(-x)) / 2
	 * @param angle vector/vector_view
	 * @return vector
	*/
	template <math_mathematical_vector_type_t T>
	constexpr auto sinh(const T& angle)
	noexcept(noexcept(
		make_sinh(std::declval<std::add_lvalue_reference_t<decltype(angle.copy())>>())
	))
	{
		auto ret = angle.copy();
		make_sinh(ret);
		return ret;
	}

	template <not_math_type_t T>
	constexpr void cos(T& v)
	noexcept(noexcept(
		std::cos(std::declval<T>())
	))
	{
		v = std::cos(v);
	}

	/**
	 * @brief the standard trigonometric cosine function.
	 * @note the values returned by this function will range from [-1, 1].
	 * @param angle vector
	 * @return vector
	*/
	template <math_mathematical_vector_t T>
	constexpr T& make_cos(T& angle)
	noexcept(noexcept(
		operator_base(
					std::declval<T&>(),
					cos<math_value_type<T>>
					)
	))
	{
		operator_base(
					angle,
					cos<math_value_type<T>>
					);
		return angle;
	}

	/**
	 * @brief the standard trigonometric cosine function.
	 * @note the values returned by this function will range from [-1, 1].
	 * @param angle vector_view
	 * @return vector
	*/
	template <math_mathematical_vector_view_t T>
	constexpr T make_cos(T angle)
	noexcept(noexcept(
		operator_base(
					std::declval<T>(),
					cos<math_value_type<T>>
					)
	))
	{
		operator_base(
					angle,
					cos<math_value_type<T>>
					);
		return angle;
	}

	/**
	* @brief the standard trigonometric cosine function.
	* @note the values returned by this function will range from [-1, 1].
	* @param angle vector/vector_view
	* @return vector
	*/
	template <math_mathematical_vector_type_t T>
	constexpr auto cos(const T& angle)
	noexcept(noexcept(
		make_cos(std::declval<std::add_lvalue_reference_t<decltype(angle.copy())>>())
	))
	{
		auto ret = angle.copy();
		make_cos(ret);
		return ret;
	}

	template <not_math_type_t T>
	constexpr void cosh(T& v)
	noexcept(noexcept(
		std::cosh(std::declval<T>())
	))
	{
		v = std::cosh(v);
	}

	/**
	 * @brief returns the hyperbolic cosine function, (exp(x) + exp(-x)) / 2
	 * @param angle vector
	 * @return vector
	*/
	template <math_mathematical_vector_t T>
	constexpr T& make_cosh(T& angle)
	noexcept(noexcept(
		operator_base(
					std::declval<T&>(),
					cosh<math_value_type<T>>
					)
	))
	{
		operator_base(
					angle,
					cosh<math_value_type<T>>
					);
		return angle;
	}

	/**
	* @brief returns the hyperbolic cosine function, (exp(x) + exp(-x)) / 2
	* @param angle vector_view
	* @return vector
	*/
	template <math_mathematical_vector_view_t T>
	constexpr T make_cosh(T angle)
	noexcept(noexcept(
		operator_base(
					std::declval<T>(),
					cosh<math_value_type<T>>
					)
	))
	{
		operator_base(
					angle,
					cosh<math_value_type<T>>
					);
		return angle;
	}

	/**
	* @brief returns the hyperbolic cosine function, (exp(x) + exp(-x)) / 2
	* @param angle vector/vector_view
	* @return vector
	*/
	template <math_mathematical_vector_type_t T>
	constexpr auto cosh(const T& angle)
	noexcept(noexcept(
		make_cosh(std::declval<std::add_lvalue_reference_t<decltype(angle.copy())>>())
	))
	{
		auto ret = angle.copy();
		make_cosh(ret);
		return ret;
	}

	template <not_math_type_t T>
	constexpr void tan(T& v)
	noexcept(noexcept(
		std::tan(std::declval<T>())
	))
	{
		v = std::tan(v);
	}

	/**
	 * @brief the standard trigonometric tangent function.
	 * @param angle vector
	 * @return vector
	*/
	template <math_mathematical_vector_t T>
	constexpr T& make_tan(T& angle)
	noexcept(noexcept(
		operator_base(
					std::declval<T&>(),
					tan<math_value_type<T>>
					)
	))
	{
		operator_base(
					angle,
					tan<math_value_type<T>>
					);
		return angle;
	}

	/**
	* @brief the standard trigonometric tangent function.
	* @param angle vector_view
	* @return vector
	*/
	template <math_mathematical_vector_view_t T>
	constexpr T make_tan(T angle)
	noexcept(noexcept(
		operator_base(
					std::declval<T>(),
					tan<math_value_type<T>>
					)
	))
	{
		operator_base(
					angle,
					tan<math_value_type<T>>
					);
		return angle;
	}

	/**
	* @brief the standard trigonometric tangent function.
	* @param angle vector/vector_view
	* @return vector
	*/
	template <math_mathematical_vector_type_t T>
	constexpr auto tan(const T& angle)
	noexcept(noexcept(
		make_tan(std::declval<std::add_lvalue_reference_t<decltype(angle.copy())>>())
	))
	{
		auto ret = angle.copy();
		make_tan(ret);
		return ret;
	}

	template <not_math_type_t T>
	constexpr void tanh(T& v)
	noexcept(noexcept(
		std::tanh(std::declval<T>())
	))
	{
		v = std::tanh(v);
	}

	/**
	 * @brief returns the hyperbolic tangent function, sinh(angle) / cosh(angle)
	 * @param angle vector
	 * @return vector
	*/
	template <math_mathematical_vector_t T>
	constexpr T& make_tanh(T& angle)
	noexcept(noexcept(
		operator_base(
					std::declval<T&>(),
					tanh<math_value_type<T>>
					)
	))
	{
		operator_base(
					angle,
					tanh<math_value_type<T>>
					);
		return angle;
	}

	/**
	* @brief returns the hyperbolic tangent function, sinh(angle) / cosh(angle)
	* @param angle vector_view
	* @return vector
	*/
	template <math_mathematical_vector_view_t T>
	constexpr T make_tanh(T angle)
	noexcept(noexcept(
		operator_base(
					std::declval<T>(),
					tanh<math_value_type<T>>
					)
	))
	{
		operator_base(
					angle,
					tanh<math_value_type<T>>
					);
		return angle;
	}

	/**
	* @brief returns the hyperbolic tangent function, sinh(angle) / cosh(angle)
	* @param angle vector/vector_view
	* @return vector
	*/
	template <math_mathematical_vector_type_t T>
	constexpr auto tanh(const T& angle)
	noexcept(noexcept(
		make_tanh(std::declval<std::add_lvalue_reference_t<decltype(angle.copy())>>())
	))
	{
		auto ret = angle.copy();
		make_tanh(ret);
		return ret;
	}

	template <not_math_type_t T>
	constexpr void asin(T& v)
	noexcept(noexcept(
		std::asin(std::declval<T>())
	))
	{
		v = std::asin(v);
	}

	/**
	 * @brief arc sine. returns an angle whose sine is x.
	 * @note the range of values returned by this function is [-PI/2, PI/2]. results are undefined if |x| > 1.
	 * @param x vector
	 * @return vector
	*/
	template <math_mathematical_vector_t T>
	constexpr T& make_asin(T& x)
	noexcept(noexcept(
		operator_base(
					std::declval<T&>(),
					asin<math_value_type<T>>
					)
	))
	{
		operator_base(
					x,
					asin<math_value_type<T>>
					);
		return x;
	}

	/**
	* @brief arc sine. returns an angle whose sine is x.
	* @note the range of values returned by this function is [-PI/2, PI/2]. results are undefined if |x| > 1.
	* @param x vector_view
	* @return vector
	*/
	template <math_mathematical_vector_view_t T>
	constexpr T make_asin(T x)
	noexcept(noexcept(
		operator_base(
					std::declval<T>(),
					asin<math_value_type<T>>
					)
	))
	{
		operator_base(
					x,
					asin<math_value_type<T>>
					);
		return x;
	}

	/**
	* @brief arc sine. returns an angle whose sine is x.
	* @note the range of values returned by this function is [-PI/2, PI/2]. results are undefined if |x| > 1.
	* @param x vector/vector_view
	* @return vector
	*/
	template <math_mathematical_vector_type_t T>
	constexpr auto asin(const T& x)
	noexcept(noexcept(
		make_asin(std::declval<std::add_lvalue_reference_t<decltype(x.copy())>>())
	))
	{
		auto ret = x.copy();
		make_asin(ret);
		return ret;
	}

	template <not_math_type_t T>
	constexpr void asinh(T& v)
	noexcept(noexcept(
		std::asinh(std::declval<T>())
	))
	{
		v = std::asinh(v);
	}

	/**
	 * @brief arc hyperbolic sine; returns the inverse of sinh.
	 * @param x vector
	 * @return vector
	*/
	template <math_mathematical_vector_t T>
	constexpr T& make_asinh(T& x)
	noexcept(noexcept(
		operator_base(
					std::declval<T&>(),
					asinh<math_value_type<T>>
					)
	))
	{
		operator_base(
					x,
					asinh<math_value_type<T>>
					);
		return x;
	}

	/**
	 * @brief arc hyperbolic sine; returns the inverse of sinh.
	 * @param x vector_view
	 * @return vector_view
	*/
	template <math_mathematical_vector_view_t T>
	constexpr T make_asinh(T x)
	noexcept(noexcept(
		operator_base(
					std::declval<T>(),
					asinh<math_value_type<T>>
					)
	))
	{
		operator_base(
					x,
					asinh<math_value_type<T>>
					);
		return x;
	}

	/**
	 * @brief arc hyperbolic sine; returns the inverse of sinh.
	 * @param x vector/vector_view
	 * @return vector
	*/
	template <math_mathematical_vector_type_t T>
	constexpr auto asinh(const T& x)
	noexcept(noexcept(
		make_asinh(std::declval<std::add_lvalue_reference_t<decltype(x.copy())>>())
	))
	{
		auto ret = x.copy();
		make_asinh(ret);
		return ret;
	}

	template <not_math_type_t T>
	constexpr void acos(T& v)
	noexcept(noexcept(
		std::acos(std::declval<T>())
	))
	{
		v = std::acos(v);
	}

	/**
	 * @brief arc cosine. returns an angle whose cosine is x.
	 * @note the range of values returned by this function is [0, PI]. results are undefined if |x| > 1.
	 * @param x vector
	 * @return vector
	*/
	template <math_mathematical_vector_t T>
	constexpr T& make_acos(T& x)
	noexcept(noexcept(
		operator_base(
					std::declval<T&>(),
					acos<math_value_type<T>>
					)
	))
	{
		operator_base(
					x,
					acos<math_value_type<T>>
					);
		return x;
	}

	/**
	* @brief arc cosine. returns an angle whose cosine is x.
	* @note the range of values returned by this function is [0, PI]. results are undefined if |x| > 1.
	* @param x vector_view
	* @return vector
	*/
	template <math_mathematical_vector_view_t T>
	constexpr T make_acos(T x)
	noexcept(noexcept(
		operator_base(
					std::declval<T>(),
					acos<math_value_type<T>>
					)
	))
	{
		operator_base(
					x,
					acos
					);
		return x;
	}

	/**
	* @brief arc cosine. returns an angle whose cosine is x.
	* @note the range of values returned by this function is [0, PI]. results are undefined if |x| > 1.
	* @param x vector/vector_view
	* @return vector
	*/
	template <math_mathematical_vector_type_t T>
	constexpr auto acos(const T& x)
	noexcept(noexcept(
		make_acos(std::declval<std::add_lvalue_reference_t<decltype(x.copy())>>())
	))
	{
		auto ret = x.copy();
		make_acos(ret);
		return ret;
	}

	template <not_math_type_t T>
	constexpr void acosh(T& v)
	noexcept(noexcept(
		std::acosh(std::declval<T>())
	))
	{
		v = std::acosh(v);
	}

	/**
	 * @brief arc hyperbolic cosine; returns the non-negative inverse of cosh.
	 * @note results are undefined if x < 1.
	 * @param x vector
	 * @return vector
	*/
	template <math_mathematical_vector_t T>
	constexpr T& make_acosh(T& x)
	noexcept(noexcept(
		operator_base(
					std::declval<T&>(),
					acosh<math_value_type<T>>
					)
	))
	{
		operator_base(
					x,
					acosh<math_value_type<T>>
					);
		return x;
	}

	/**
	 * @brief arc hyperbolic cosine; returns the non-negative inverse of cosh.
	 * @note results are undefined if x < 1.
	 * @param x vector_view
	 * @return vector_view
	*/
	template <math_mathematical_vector_view_t T>
	constexpr T make_acosh(T x)
	noexcept(noexcept(
		operator_base(
					std::declval<T>(),
					acosh<math_value_type<T>>
					)
	))
	{
		operator_base(
					x,
					acosh<math_value_type<T>>
					);
		return x;
	}

	/**
	 * @brief arc hyperbolic cosine; returns the non-negative inverse of cosh.
	 * @note results are undefined if x < 1.
	 * @param x vector/vector_view
	 * @return vector
	*/
	template <math_mathematical_vector_type_t T>
	constexpr auto acosh(const T& x)
	noexcept(noexcept(
		make_acosh(std::declval<std::add_lvalue_reference_t<decltype(x.copy())>>())
	))
	{
		auto ret = x.copy();
		make_acosh(ret);
		return ret;
	}

	template <not_math_type_t T>
	constexpr void atan(T& v)
	noexcept(noexcept(
		std::atan(std::declval<T>())
	))
	{
		v = std::atan(v);
	}

	/**
	 * @brief arc tangent. returns an angle whose tangent is y_over_x.
	 * @note the range of values returned by this function is [-PI/2, PI/2].
	 * @param y_over_x vector
	 * @return vector
	*/
	template <math_mathematical_vector_t T>
	constexpr T& make_atan(T& y_over_x)
	noexcept(noexcept(
		operator_base(
					std::declval<T&>(),
					atan<math_value_type<T>>
					)
	))
	{
		operator_base(
					y_over_x,
					atan<math_value_type<T>>
					);
		return y_over_x;
	}

	/**
	* @brief arc tangent. returns an angle whose tangent is y_over_x.
	* @note the range of values returned by this function is [-PI/2, PI/2].
	* @param y_over_x vector_view
	* @return vector
	*/
	template <math_mathematical_vector_view_t T>
	constexpr T make_atan(T y_over_x)
	noexcept(noexcept(
		operator_base(
					std::declval<T>(),
					atan<math_value_type<T>>
					)
	))
	{
		operator_base(
					y_over_x,
					atan<math_value_type<T>>
					);
		return y_over_x;
	}

	/**
	* @brief arc tangent. returns an angle whose tangent is y_over_x.
	* @note the range of values returned by this function is [-PI/2, PI/2].
	* @param y_over_x vector/vector_view
	* @return vector
	*/
	template <math_mathematical_vector_type_t T>
	constexpr auto atan(const T& y_over_x)
	noexcept(noexcept(
		make_atan(std::declval<std::add_lvalue_reference_t<decltype(y_over_x.copy())>>())
	))
	{
		auto ret = y_over_x.copy();
		make_atan(ret);
		return ret;
	}

	template <not_math_type_t T>
	constexpr void atanh(T& v)
	noexcept(noexcept(
		std::atanh(std::declval<T>())
	))
	{
		v = std::atanh(v);
	}

	/**
	 * @brief arc hyperbolic tangent; returns the inverse of tanh.
	 * @note results are undefined if abs(x) >= 1.
	 * @param x vector
	 * @return vector
	*/
	template <math_mathematical_vector_t T>
	constexpr T& make_atanh(T& x)
	noexcept(noexcept(
		operator_base(
					std::declval<T&>(),
					atanh<math_value_type<T>>
					)
	))
	{
		operator_base(
					x,
					atanh<math_value_type<T>>
					);
		return x;
	}

	/**
	 * @brief arc hyperbolic tangent; returns the inverse of tanh.
	 * @note results are undefined if abs(x) >= 1.
	 * @param x vector_view
	 * @return vector_view
	*/
	template <math_mathematical_vector_view_t T>
	constexpr T make_atanh(T x)
	noexcept(noexcept(
		operator_base(
					std::declval<T>(),
					atanh<math_value_type<T>>
					)
	))
	{
		operator_base(
					x,
					atanh<math_value_type<T>>
					);
		return x;
	}

	/**
	 * @brief arc hyperbolic tangent; returns the inverse of tanh.
	 * @note results are undefined if abs(x) >= 1.
	 * @param x vector/vector_view
	 * @return vector
	*/
	template <math_mathematical_vector_type_t T>
	constexpr auto atanh(const T& x)
	noexcept(noexcept(
		make_atanh(std::declval<std::add_lvalue_reference_t<decltype(x.copy())>>())
	))
	{
		auto ret = x.copy();
		make_atanh(ret);
		return ret;
	}

	template <not_math_type_t T1, not_math_type_t T2>
	constexpr void atan2(T1& y, T2 x)
	noexcept(noexcept(
		std::atan2(std::declval<T1>(), std::declval<T2>())
	))
	{
		y = std::atan2(y, x);
	}

	/**
	 * @brief arc tangent. returns an angle whose tangent is y/x
	 * @note the signs of x and y are used to determine what quadrant the angle is in. the range of values returned
	 * by this function is [-PI, PI]. results are undefined if x and y are both 0.
	 * @param y vector
	 * @param x vector/vector_view
	 * @return vector
	*/
	template <math_mathematical_vector_t Y, math_vector_same_size_t<Y> X>
	constexpr Y& make_atan2(Y& y, const X& x)
	noexcept(noexcept(
		operator_base(
					std::declval<Y&>(),
					std::declval<const X&>(),
					atan2<math_value_type<Y>, math_value_type<X>>
					)
	))
	{
		operator_base(
					y,
					x,
					atan2<math_value_type<Y>, math_value_type<X>>
					);
		return y;
	}

	/**
	* @brief arc tangent. returns an angle whose tangent is y/x
	* @note the signs of x and y are used to determine what quadrant the angle is in. the range of values returned
	* by this function is [-PI, PI]. results are undefined if x and y are both 0.
	* @param y vector_view
	* @param x vector/vector_view
	* @return vector
	*/
	template <math_mathematical_vector_view_t Y, math_vector_same_size_t<Y> X>
	constexpr Y make_atan2(Y y, const X& x)
	noexcept(noexcept(
		operator_base(
					std::declval<Y>(),
					std::declval<const X&>(),
					atan2<math_value_type<Y>, math_value_type<X>>
					)
	))
	{
		operator_base(
					y,
					x,
					atan2<math_value_type<Y>, math_value_type<X>>
					);
		return y;
	}

	/**
	* @brief arc tangent. returns an angle whose tangent is y/x
	* @note the signs of x and y are used to determine what quadrant the angle is in. the range of values returned
	* by this function is [-PI, PI]. results are undefined if x and y are both 0.
	* @param y vector/vector_view
	* @param x vector/vector_view
	* @return vector
	*/
	template <math_mathematical_vector_type_t Y, math_vector_same_size_t<Y> X>
	constexpr auto atan2(const Y& y, const X& x)
	noexcept(noexcept(
		make_atan2(
					std::declval<std::add_lvalue_reference_t<decltype(y.copy())>>(),
					std::declval<const X&>()
				)
	))
	{
		auto ret = y.copy();
		make_atan2(ret, x);
		return ret;
	}
}
