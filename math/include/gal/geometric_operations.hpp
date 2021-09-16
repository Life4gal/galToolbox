#pragma once

#include <gal/math_fwd.hpp>
#include <gal/vector.hpp>
#include <gal/matrix.hpp>
#include <gal/math_apply.hpp>
#include <gal/basic_operations.hpp>
#include <gal/numeric_and_algorithm.hpp>
#include <gal/exponential_operations.hpp>

namespace gal::toolbox::math
{
	/**
	 * @brief returns the dot product of x and y, i.e., result = x * y.
	 * @param x vector/vector_view
	 * @param y vector/vector_view
	 * @return math_value_type<T>
	*/
	template <typename T1,
			 math_mathematical_vector_same_type_and_size_ignore_view_t<T1> T2
			 >
	constexpr math_value_type<T1> dot(const T1& x, const T2& y)
			noexcept(noexcept(
					sum(
							std::declval<const T1&>()
									)
							))
	{
		return sum(x * y);
	}

	/**
	 * @brief returns the cross product of x and y.
	 * @param x vector/vector_view
	 * @param y vector/vector_view
	 * @return vector
	*/
	template <typename T1,
			 math_mathematical_vector_same_type_and_size_ignore_view_t<T1> T2
			 >
	requires (T1::data_size == 3)
			constexpr auto cross(const T1& x, const T2& y)
					noexcept(noexcept(
							std::is_nothrow_constructible_v<
									typename T1::value_type,
									typename T1::value_type,
									typename T1::value_type,
									typename T1::value_type
									>
							))
	{
		using type = typename T1::value_type;
		return typename T1::template copy_type<>{
				static_cast<type>(x.y() * y.z() - y.y() * x.z()),
				static_cast<type>(x.z() * y.x() - y.z() * x.x()),
				static_cast<type>(x.x() * y.y() - y.x() * x.y())
		};
	}

	/**
	 * @brief returns the length of x, i.e., sqrt(x * x).
	 * @param x vector/vector_view
	 * @return math_value_type<T>
	*/
	template <math_mathematical_vector_type_t T>
	constexpr math_value_type<T> length(const T& x)
			noexcept(noexcept(
					std::sqrt(dot<T, T>(
							std::declval<const T&>(),
							std::declval<const T&>()
									))
							))
	{
		return std::sqrt(dot(x, x));
	}

	/**
	 * @brief returns the distance between p0 and p1, i.e., length(p0 - p1).
	 * @param p0 vector/vector_view
	 * @param p1 vector/vector_view
	 * @return math_value_type<T>
	*/
	template <
			typename T1,
			math_mathematical_vector_same_type_and_size_ignore_view_t<T1> T2
			>
	constexpr math_value_type<T1> distance(const T1& p0, const T2& p1)
			noexcept(noexcept(
					length<T1>(
							std::declval<std::add_lvalue_reference_t<decltype(
									std::declval<const T1&>() - std::declval<const T2&>()
											)>>()
									)
							))
	{
		return length(p1 - p0);
	}

	/**
	 * @brief normalize a vector's length to 1.
	 * @note if length(x) == 0 then result is undefined and generate an error.
	 * @param x vector
	 * @return vector self
	*/
	template <math_mathematical_vector_t T>
	constexpr T& make_normalize(T& x)
			noexcept(noexcept(
					inverse_sqrt<math_value_type<T>>(
							std::declval<
									std::add_lvalue_reference_t<decltype(dot<T, T>(
											std::declval<const T&>(),
											std::declval<const T&>()
													))>
									>()
									)
							))
	{
		auto v = dot(x, x);
		inverse_sqrt(v);
		x *= v;
		return x;
	}

	/**
	 * @brief normalize a vector's length to 1.
	 * @note if length(x) == 0 then result is undefined and generate an error.
	 * @param x vector_view
	 * @return vector_view self
	*/
	template <math_mathematical_vector_view_t T>
	constexpr T make_normalize(T x)
			noexcept(noexcept(
					inverse_sqrt<math_value_type<T>>(
							std::declval<
									std::add_lvalue_reference_t<decltype(dot<T, T>(
											std::declval<const T&>(),
											std::declval<const T&>()
													))>
									>()
									)
							))
	{
		auto v = dot(x, x);
		inverse_sqrt(v);
		x *= v;
		return x;
	}

	/**
	 * @brief returns a vector in the same direction as x but with length of 1.
	 * @note if length(x) == 0 then result is undefined and generate an error.
	 * @param x vector/vector_view
	 * @return vector
	*/
	template <math_mathematical_vector_type_t T>
	constexpr auto normalize(const T& x)
			noexcept(noexcept(
					make_normalize(std::declval<std::add_lvalue_reference_t<decltype(x.copy())>>())
							))
	{
		auto ret = x.copy();
		make_normalize(ret);
		return ret;
	}

	/**
	 * @brief for the incident vector i and surface orientation n,
	 * returns the reflection direction: result = i - 2.0 * dot(n, i) * n.
	 * @param i incident vector
	 * @param n surface orientation
	 * @return vector(reflection direction)
	*/
	template <typename T1,
			 math_mathematical_vector_same_type_and_size_ignore_view_t<T1> T2
			 >
	constexpr T1 reflect(const T1& i, const T2& n)
			noexcept(noexcept(
					std::declval<const T1&>() -
					std::declval<const T2&>() *
							dot(std::declval<const T2&>(), std::declval<const T1&>()) *
							std::declval<math_value_type<T1>>()
							))
	{
		return i - n * dot(n, i) * static_cast<math_value_type<T1>>(2);
	}

	/**
	 * @brief for the incident vector i and surface normal n,
	 * and the ratio of indices of refraction eta, return the refraction vector.
	 * @param i incident vector
	 * @param n surface normal
	 * @param eta ratio of indices of refraction
	 * @return vector(refraction vector)
	*/
	template <typename T1,
			 math_mathematical_vector_same_type_and_size_ignore_view_t<T1> T2
			 >
	constexpr T1 refract(
			const T1&                                 i,
			const T2&                                 n,
			std::type_identity_t<math_value_type<T1>> eta
			)
			noexcept(
					noexcept(
							std::declval<math_value_type<T1>>() -
							dot(std::declval<const T2&>(), std::declval<const T1&>()) *
									dot(std::declval<const T2&>(), std::declval<const T1&>())
									) &&
					noexcept(
							std::declval<math_value_type<T1>>() *
									std::declval<const T1&>() -
							(
									std::declval<math_value_type<T1>>() *
											dot(std::declval<const T2&>(), std::declval<const T1&>()) +
									std::declval<math_value_type<T1>>()
											) *
									std::declval<const T2&>()
									)
			)
	{
		using type = math_value_type<T1>;
		const auto dot_value = dot(n, i);
		const auto k         = static_cast<type>(1) - eta * eta * (static_cast<type>(1) - dot_value * dot_value);

		return
				(k >= static_cast<type>(0))
						? (eta * i - (eta * dot_value + std::sqrt(k)) * n)
						: T1{};
	}
}
