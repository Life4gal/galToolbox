#pragma once

#include <type_traits>

namespace gal::test::math
{
	template <typename T, std::size_t N>
	class vector;
	template<typename T, std::size_t Row, std::size_t Column>
	class matrix;

	template <typename T>
	using vector1 = vector<T, 1>;
	template <typename T>
	using vector2 = vector<T, 2>;
	template <typename T>
	using vector3 = vector<T, 3>;
	template <typename T>
	using vector4 = vector<T, 4>;

	template<typename T>
	using matrix2x2 = matrix<T, 2, 2>;
	template<typename T>	
	using matrix2x3 = matrix<T, 2, 3>;
	template<typename T>	 
	using matrix2x4 = matrix<T, 2, 4>;
	template<typename T>	
	using matrix3x2 = matrix<T, 3, 2>;
	template<typename T>	 
	using matrix3x3 = matrix<T, 3, 3>;
	template<typename T>	  
	using matrix3x4 = matrix<T, 3, 4>;
	template<typename T>	 
	using matrix4x2 = matrix<T, 4, 2>;
	template<typename T>	 
	using matrix4x3 = matrix<T, 4, 3>;
	template<typename T>	 
	using matrix4x4 = matrix<T, 4, 4>;

	/**
	 * @brief specialize math_trait for to_tuple support
	 * @note to_tuple will "expand" the target and get the element of the former "size" according to "size" through operator[]
	 * @tparam T specialized type
	*/
	template<typename T>
	struct math_trait : std::false_type
	{
		using value_type = T;
		constexpr static std::size_t size = 1;
	};

	/**
	 * @brief specialize math_invoker_trait for operator support
	 * @note if the operation corresponding to the target type is false, the corresponding operation is not allowed
	 * @tparam T specialized type
	*/
	template<typename T>
	struct math_invoker_trait : std::false_type {};
}
