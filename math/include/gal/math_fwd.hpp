#pragma once

namespace gal::toolbox::math
{
	template <typename T, std::size_t N>
	class vector;
	template <std::size_t Stride, std::size_t Size, typename Iterator>
	class vector_view;
	template <typename T, std::size_t Row, std::size_t Column>
	class matrix;

	template <typename T>
	using vector1 = vector<T, 1>;
	template <typename T>
	using vector2 = vector<T, 2>;
	template <typename T>
	using vector3 = vector<T, 3>;
	template <typename T>
	using vector4 = vector<T, 4>;

	template <typename T>
	using matrix2x2 = matrix<T, 2, 2>;
	template <typename T>
	using matrix2x3 = matrix<T, 2, 3>;
	template <typename T>
	using matrix2x4 = matrix<T, 2, 4>;
	template <typename T>
	using matrix3x2 = matrix<T, 3, 2>;
	template <typename T>
	using matrix3x3 = matrix<T, 3, 3>;
	template <typename T>
	using matrix3x4 = matrix<T, 3, 4>;
	template <typename T>
	using matrix4x2 = matrix<T, 4, 2>;
	template <typename T>
	using matrix4x3 = matrix<T, 4, 3>;
	template <typename T>
	using matrix4x4 = matrix<T, 4, 4>;

	template <typename T>
	struct is_vector : std::false_type {};

	template <typename T, std::size_t N>
	struct is_vector<vector<T, N>> : std::true_type {};

	template <typename T>
	constexpr static bool is_vector_v = is_vector<T>::value;

	template <typename T>
	concept vector_t = is_vector_v<T>;

	template <typename T>
	concept not_vector_t = !vector_t<T>;

	template <typename T>
	struct is_vector_view : std::false_type {};

	template <std::size_t Stride, std::size_t Size, typename Iterator>
	struct is_vector_view<vector_view<Stride, Size, Iterator>> : std::true_type {};

	template <typename T>
	constexpr static bool is_vector_view_v = is_vector_view<T>::value;

	template <typename T>
	concept vector_view_t = is_vector_view_v<T>;

	template <typename T>
	concept not_vector_view_t = !vector_view_t<T>;

	template <typename T>
	constexpr static bool is_vector_type_v = is_vector_v<T> || is_vector_view_v<T>;

	template <typename T>
	concept vector_type_t = is_vector_type_v<T>;

	template <typename T>
	concept not_vector_type_t = !vector_type_t<T>;

	template <typename T>
	struct is_matrix : std::false_type {};

	template <typename T, std::size_t Row, std::size_t Column>
	struct is_matrix<matrix<T, Row, Column>> : std::true_type {};

	template <typename T>
	constexpr static bool is_matrix_v = is_matrix<T>::value;

	template <typename T>
	concept matrix_t = is_matrix_v<T>;

	template <typename T>
	concept not_matrix_t = !is_matrix_v<T>;

	template <typename T>
	constexpr static bool is_math_type_v =
			is_vector_type_v<T> ||
			is_matrix_v<T>;

	template <typename T>
	concept math_type_t = is_math_type_v<T>;

	template <typename T>
	concept not_math_type_t = !math_type_t<T>;

	template <typename T>
	concept matrix_or_vector_t = matrix_t<T> || vector_t<T>;

	template <typename T>
	struct math_value_type_helper
	{
		using type = T;
	};

	template <math_type_t T>
	struct math_value_type_helper<T>
	{
		using type = typename T::value_type;
	};

	template <typename T>
	using math_value_type = typename math_value_type_helper<T>::type;

	template <typename T1, typename T2>
	struct is_math_same_type : std::false_type {};

	template <vector_t T1, vector_t T2>
	struct is_math_same_type<T1, T2> : std::true_type {};

	template <vector_view_t T1, vector_view_t T2>
	struct is_math_same_type<T1, T2> : std::true_type {};

	template <matrix_t T1, matrix_t T2>
	struct is_math_same_type<T1, T2> : std::true_type {};

	template <typename T1, typename T2>
	constexpr static bool is_math_same_type_v = is_math_same_type<T1, T2>::value;

	template <typename T1, typename T2>
	concept math_same_type_t = is_math_same_type_v<T1, T2>;

	template <bool, typename T1, typename T2>
	struct is_math_same_type_ignore_view : is_math_same_type<T1, T2> {};

	template <vector_type_t T1, vector_type_t T2>
	struct is_math_same_type_ignore_view<true, T1, T2> : std::true_type {};

	template <typename T1, typename T2>
	constexpr static bool is_math_same_type_ignore_view_v = is_math_same_type_ignore_view<true, T1, T2>::value;

	template <typename T1, typename T2>
	concept math_same_type_ignore_view_t = is_math_same_type_ignore_view_v<T1, T2>;

	template <typename T1, typename T2, bool>
	struct is_math_same_size : std::false_type {};

	template <vector_type_t T1, vector_type_t T2>
	requires (T1::data_size == T2::data_size)
			struct is_math_same_size<T1, T2, true> : std::true_type {};

	template <matrix_t T1, matrix_t T2>
	requires
			(T1::column_size == T2::column_size) &&
			(T1::row_size == T2::row_size)
					struct is_math_same_size<T1, T2, true> : std::true_type {};

	template <matrix_t T1, vector_type_t T2, bool AsRow>
	requires
			(AsRow
					 ? (T1::row_size == T2::data_size)
					 : (T1::column_size == T2::data_size)
					 )
					struct is_math_same_size<T1, T2, AsRow> : std::true_type {};

	template <vector_type_t T1, matrix_t T2, bool AsRow>
	requires
			(AsRow
					 ? (T1::data_size == T2::row_size)
					 : (T1::data_size == T2::column_size)
					 )
					struct is_math_same_size<T1, T2, AsRow> : std::true_type {};

	template <typename T1, typename T2, bool AsRow = true>
	constexpr static bool is_math_same_size_v = is_math_same_size<T1, T2, AsRow>::value;

	template <typename T1, typename T2, bool AsRow = true>
	concept math_same_size_t = is_math_same_size_v<T1, T2, AsRow>;

	template <typename T1, typename T2>
	concept math_same_type_and_size_t = math_same_type_t<T1, T2> && math_same_size_t<T1, T2>;

	template <typename T1, typename T2>
	concept math_same_type_and_size_ignore_view_t = math_same_type_ignore_view_t<T1, T2> && math_same_size_t<T1, T2>;

	template <typename T1, typename T2>
	concept math_vector_same_size_t = vector_type_t<T1> && vector_type_t<T2> && math_same_size_t<T1, T2>;

	template <typename T1, typename T2>
	concept math_matrix_same_size_t = matrix_t<T1> && matrix_t<T2> && math_same_size_t<T1, T2>;

	//====================================================

	template <typename T>
	concept math_mathematical_vector_type_size_t = (1 <= T::data_size) && (T::data_size <= 4);

	template <typename T>
	concept math_mathematical_value_type_t = std::is_floating_point_v<math_value_type<T>>;

	template <typename T>
	concept math_mathematical_vector_t =
			vector_t<T> &&
			math_mathematical_vector_type_size_t<T> &&
			math_mathematical_value_type_t<T>;

	template <typename T>
	concept math_mathematical_vector_view_t =
			vector_view_t<T> &&
			math_mathematical_vector_type_size_t<T> &&
			math_mathematical_value_type_t<T>;

	template <typename T>
	concept math_mathematical_vector_type_t =
			vector_type_t<T> &&
			math_mathematical_vector_type_size_t<T> &&
			math_mathematical_value_type_t<T>;

	template <typename T1, typename T2>
	concept math_mathematical_vector_same_type_and_size_t =
			math_mathematical_vector_t<T1> &&
			math_same_type_and_size_t<T1, T2>;

	template <typename T1, typename T2>
	concept math_mathematical_vector_same_type_and_size_ignore_view_t =
			math_mathematical_vector_type_t<T1> &&
			math_same_type_and_size_ignore_view_t<T1, T2>;
}
