export module gal.toolbox.math.basic_type:basic_type_alias;

import :vector;
import :matrix;

namespace gal::toolbox::math
{
	export
	{
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
	}
}
