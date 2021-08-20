#pragma once

#include <type_traits>

namespace gal::test::math
{
	template <typename T>
	concept arithmetic = std::is_integral_v<T> || std::is_floating_point_v<T>;

	template <arithmetic T, std::size_t N>
	class vector;
	template<arithmetic T, std::size_t Row, std::size_t Column>
	class matrix;

	template <arithmetic T>
	using vector1 = vector<T, 1>;
	template <arithmetic T>
	using vector2 = vector<T, 2>;
	template <arithmetic T>
	using vector3 = vector<T, 3>;
	template <arithmetic T>
	using vector4 = vector<T, 4>;

	template<arithmetic T>
	using matrix2x2 = matrix<T, 2, 2>;
	template<arithmetic T>	
	using matrix2x3 = matrix<T, 2, 3>;
	template<arithmetic T>	 
	using matrix2x4 = matrix<T, 2, 4>;
	template<arithmetic T>	
	using matrix3x2 = matrix<T, 3, 2>;
	template<arithmetic T>	 
	using matrix3x3 = matrix<T, 3, 3>;
	template<arithmetic T>	  
	using matrix3x4 = matrix<T, 3, 4>;
	template<arithmetic T>	 
	using matrix4x2 = matrix<T, 4, 2>;
	template<arithmetic T>	 
	using matrix4x3 = matrix<T, 4, 3>;
	template<arithmetic T>	 
	using matrix4x4 = matrix<T, 4, 4>;
}
