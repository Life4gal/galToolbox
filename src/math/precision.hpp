#pragma once

namespace gal::test
{
	enum class precision
	{
		high,
		medium,
		low,

		default_precision = high
	};

	template<typename T>
	concept arithmetic = std::is_integral_v<T> || std::is_floating_point_v<T>;
	
	using vector_size_type = std::size_t;
	using matrix_size_type = std::size_t;
	template<vector_size_type Size, arithmetic T, precision = precision::default_precision>
	struct vector;
	template<matrix_size_type Column, matrix_size_type Row, arithmetic T, precision = precision::default_precision>
	struct matrix;
	template<arithmetic T, precision = precision::default_precision>
	struct quaternion;

	template<arithmetic T, precision P = precision::default_precision>
	using vector1 = vector<1, T, P>;
	template<arithmetic T, precision P = precision::default_precision>
	using vector2 = vector<2, T, P>;
	template<arithmetic T, precision P = precision::default_precision>
	using vector3 = vector<3, T, P>;
	template<arithmetic T, precision P = precision::default_precision>
	using vector4 = vector<4, T, P>;

	template<arithmetic T, precision P = precision::default_precision>
	using matrix2x2 = matrix<2, 2, T, P>;
	template<arithmetic T, precision P = precision::default_precision>
	using matrix2x3 = matrix<2, 3, T, P>;
	template<arithmetic T, precision P = precision::default_precision>
	using matrix2x4 = matrix<2, 4, T, P>;
	template<arithmetic T, precision P = precision::default_precision>
	using matrix3x2 = matrix<3, 2, T, P>;
	template<arithmetic T, precision P = precision::default_precision>
	using matrix3x3 = matrix<3, 3, T, P>;
	template<arithmetic T, precision P = precision::default_precision>
	using matrix3x4 = matrix<3, 4, T, P>;
	template<arithmetic T, precision P = precision::default_precision>
	using matrix4x2 = matrix<4, 2, T, P>;
	template<arithmetic T, precision P = precision::default_precision>
	using matrix4x3 = matrix<4, 3, T, P>;
	template<arithmetic T, precision P = precision::default_precision>
	using matrix4x4 = matrix<4, 4, T, P>;
}
