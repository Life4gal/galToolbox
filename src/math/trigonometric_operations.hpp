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

	using std::sin;

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
					std::declval<decltype(
						sin
					)>()
					)
	))
	{
		operator_base(
					angle,
					sin
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
					std::declval<decltype(
						sin
					)>()
					)
	))
	{
		operator_base(
					angle,
					sin
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
		make_sin(std::declval<std::add_lvalue_reference_t<decltype(degrees.copy())>>())
	))
	{
		auto ret = angle.copy;
		make_sin(ret);
		return ret;
	}

	using std::sinh;

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
					std::declval<decltype(
						sinh
					)>()
					)
	))
	{
		operator_base(
					angle,
					sinh
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
					std::declval<decltype(
						sinh
					)>()
					)
	))
	{
		operator_base(
					angle,
					sinh
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
		make_sinh(std::declval<std::add_lvalue_reference_t<decltype(degrees.copy())>>())
	))
	{
		auto ret = angle.copy;
		make_sinh(ret);
		return ret;
	}

	using std::cos;

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
					std::declval<decltype(
						cos
					)>()
					)
	))
	{
		operator_base(
					angle,
					cos
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
					std::declval<decltype(
						cos
					)>()
					)
	))
	{
		operator_base(
					angle,
					cos
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
		make_cos(std::declval<std::add_lvalue_reference_t<decltype(degrees.copy())>>())
	))
	{
		auto ret = angle.copy;
		make_cos(ret);
		return ret;
	}

	using std::cosh;

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
					std::declval<decltype(
						cosh
					)>()
					)
	))
	{
		operator_base(
					angle,
					cosh
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
					std::declval<decltype(
						cosh
					)>()
					)
	))
	{
		operator_base(
					angle,
					cosh
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
		make_cosh(std::declval<std::add_lvalue_reference_t<decltype(degrees.copy())>>())
	))
	{
		auto ret = angle.copy;
		make_cosh(ret);
		return ret;
	}

	using std::tan;

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
					std::declval<decltype(
						tan
					)>()
					)
	))
	{
		operator_base(
					angle,
					tan
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
					std::declval<decltype(
						tan
					)>()
					)
	))
	{
		operator_base(
					angle,
					tan
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
		make_tan(std::declval<std::add_lvalue_reference_t<decltype(degrees.copy())>>())
	))
	{
		auto ret = angle.copy;
		make_tan(ret);
		return ret;
	}

	using std::tanh;

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
					std::declval<decltype(
						tanh
					)>()
					)
	))
	{
		operator_base(
					angle,
					tanh
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
					std::declval<decltype(
						tanh
					)>()
					)
	))
	{
		operator_base(
					angle,
					tanh
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
		make_tanh(std::declval<std::add_lvalue_reference_t<decltype(degrees.copy())>>())
	))
	{
		auto ret = angle.copy;
		make_tanh(ret);
		return ret;
	}

	using std::asin;

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
					std::declval<decltype(
						asin
					)>()
					)
	))
	{
		operator_base(
					x,
					asin
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
					std::declval<decltype(
						asin
					)>()
					)
	))
	{
		operator_base(
					x,
					asin
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
		make_asin(std::declval<std::add_lvalue_reference_t<decltype(degrees.copy())>>())
	))
	{
		auto ret = x.copy;
		make_asin(ret);
		return ret;
	}

	using std::asinh;

	template <math_mathematical_vector_t T>
	constexpr T& make_asinh(T& t)
	noexcept(noexcept(
		operator_base(
					std::declval<T&>(),
					std::declval<decltype(
						asinh
					)>()
					)
	))
	{
		operator_base(
					t,
					asinh
					);
		return t;
	}

	template <math_mathematical_vector_view_t T>
	constexpr T make_asinh(T t)
	noexcept(noexcept(
		operator_base(
					std::declval<T>(),
					std::declval<decltype(
						asinh
					)>()
					)
	))
	{
		operator_base(
					t,
					asinh
					);
		return t;
	}

	template <math_mathematical_vector_type_t T>
	constexpr auto asinh(const T& t)
	noexcept(noexcept(
		make_asinh(std::declval<std::add_lvalue_reference_t<decltype(degrees.copy())>>())
	))
	{
		auto ret = t.copy;
		make_asinh(ret);
		return ret;
	}

	using std::acos;

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
					std::declval<decltype(
						acos
					)>()
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
	* @param x vector_view
	* @return vector
	*/
	template <math_mathematical_vector_view_t T>
	constexpr T make_acos(T x)
	noexcept(noexcept(
		operator_base(
					std::declval<T>(),
					std::declval<decltype(
						acos
					)>()
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
		make_acos(std::declval<std::add_lvalue_reference_t<decltype(degrees.copy())>>())
	))
	{
		auto ret = x.copy;
		make_acos(ret);
		return ret;
	}

	using std::acosh;

	template <math_mathematical_vector_t T>
	constexpr T& make_acosh(T& t)
	noexcept(noexcept(
		operator_base(
					std::declval<T&>(),
					std::declval<decltype(
						acosh
					)>()
					)
	))
	{
		operator_base(
					t,
					acosh
					);
		return t;
	}

	template <math_mathematical_vector_view_t T>
	constexpr T make_acosh(T t)
	noexcept(noexcept(
		operator_base(
					std::declval<T>(),
					std::declval<decltype(
						acosh
					)>()
					)
	))
	{
		operator_base(
					t,
					acosh
					);
		return t;
	}

	template <math_mathematical_vector_type_t T>
	constexpr auto acosh(const T& t)
	noexcept(noexcept(
		make_acosh(std::declval<std::add_lvalue_reference_t<decltype(degrees.copy())>>())
	))
	{
		auto ret = t.copy;
		make_acosh(ret);
		return ret;
	}

	using std::atan;

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
					std::declval<decltype(
						atan
					)>()
					)
	))
	{
		operator_base(
					y_over_x,
					atan
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
					std::declval<decltype(
						atan
					)>()
					)
	))
	{
		operator_base(
					y_over_x,
					atan
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
		make_atan(std::declval<std::add_lvalue_reference_t<decltype(degrees.copy())>>())
	))
	{
		auto ret = y_over_x.copy;
		make_atan(ret);
		return ret;
	}

	using std::atanh;

	template <math_mathematical_vector_t T>
	constexpr T& make_atanh(T& t)
	noexcept(noexcept(
		operator_base(
					std::declval<T&>(),
					std::declval<decltype(
						atanh
					)>()
					)
	))
	{
		operator_base(
					t,
					atanh
					);
		return t;
	}

	template <math_mathematical_vector_view_t T>
	constexpr T make_atanh(T t)
	noexcept(noexcept(
		operator_base(
					std::declval<T>(),
					std::declval<decltype(
						atanh
					)>()
					)
	))
	{
		operator_base(
					t,
					atanh
					);
		return t;
	}

	template <math_mathematical_vector_type_t T>
	constexpr auto atanh(const T& t)
	noexcept(noexcept(
		make_atanh(std::declval<std::add_lvalue_reference_t<decltype(degrees.copy())>>())
	))
	{
		auto ret = t.copy;
		make_atanh(ret);
		return ret;
	}

	using std::atan2;

	/**
	 * @brief rrc tangent. returns an angle whose tangent is y/x
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
					std::declval<decltype(
						atan2
					)>()
					)
	))
	{
		operator_base(
					y,
					x,
					atan2
					);
		return y;
	}

	/**
	* @brief rrc tangent. returns an angle whose tangent is y/x
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
					std::declval<decltype(
						atan2
					)>()
					)
	))
	{
		operator_base(
					y,
					x,
					atan2
					);
		return y;
	}

	/**
	* @brief rrc tangent. returns an angle whose tangent is y/x
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
					std::declval<std::add_lvalue_reference_t<decltype(degrees.copy())>>(),
					std::declval<const X&>()
				)
	))
	{
		auto ret = y.copy;
		make_atan2(ret, x);
		return ret;
	}
}
