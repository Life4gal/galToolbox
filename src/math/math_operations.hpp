#pragma once

#include "new_vector.hpp"
#include "new_matrix.hpp"
#include "math_apply.hpp"

namespace gal::test::new_math
{
	// two matrices/vectors
	template <bool HasRet = false, bool All = true, typename T1, typename T2, typename Pred>
		requires
		(
			(is_matrix_type_v<T1> && is_matrix_type_v<T2>) ||
			(is_vector_type_v<T1> && is_vector_type_v<T2>)
		)
		&&
		is_math_same_size_v<T1, T2>
	constexpr auto operator_base(T1& t1, const T2& t2, Pred&& pred)
	{
		return binary_apply_seq<HasRet, All>(
											t1,
											t2,
											std::forward<Pred>(pred)
											);
	}

	// one matrix, one vector
	template <bool AsRow, bool HasRet = false, bool All = true, typename T1, typename T2, typename Pred>
		requires
		is_matrix_type_v<T1> &&
		is_vector_type_v<T2> &&
		is_math_same_size_v<T1, T2, AsRow>
	constexpr auto operator_base(T1& t1, const T2& t2, Pred&& pred)
	{
		return [&]<std::size_t...I>(std::index_sequence<I...>)
		{
			[](std::convertible_to<bool> auto ...x)
			{
				if constexpr (All)
				{
					return (x && ...);
				}
				else
				{
					return (x || ...);
				}
			}(operator_base<HasRet, All>(
										[&t1]() constexpr
										{
											if constexpr (AsRow)
											{
												return t1.get_row_view(I);
											}
											else
											{
												return t1.get_column_view(I);
											}
										},
										t2,
										std::forward<Pred>(pred)
										)...
			);
		}(std::make_index_sequence<AsRow ? T1::column_size : T1::row_size>{});
		// if constexpr (AsRow)
		// {
		// 	return [&]<std::size_t...I>(std::index_sequence<I...>)
		// 	{
		// 		[](std::convertible_to<bool> auto ...x)
		// 		{
		// 			if constexpr (All)
		// 			{
		// 				return (x && ...);
		// 			}
		// 			else
		// 			{
		// 				return (x || ...);
		// 			}
		// 		}(operator_base<HasRet, All>(t1.get_row_view(I), t2, std::forward<Pred>(pred))...);
		// 	}(std::make_index_sequence<T1::column_size>{});
		// }
		// else
		// {
		// 	return [&] <std::size_t...I>(std::index_sequence<I...>)
		// 	{
		// 		[](std::convertible_to<bool> auto ...x)
		// 		{
		// 			if constexpr (All)
		// 			{
		// 				return (x && ...);
		// 			}
		// 			else
		// 			{
		// 				return (x || ...);
		// 			}
		// 		}(operator_base<HasRet, All>(t1.get_column_view(I), t2, std::forward<Pred>(pred))...);
		// 	}(std::make_index_sequence<T1::row_size>{});
		// }
	}

	// one matrix/vector, one arg
	template <bool HasRet, bool All = true, typename T1, typename T2, typename Pred>
		requires
		is_math_type_v<T1>
	constexpr auto operator_base(T1& t1, T2 t2, Pred&& pred)
	{
		return binary_apply_dup<HasRet, All>(
											t1,
											t2,
											std::forward<Pred>(pred)
											);
	}

	// one matrix/vector, one parameters pack
	template <bool HasRet, bool All = true, typename T1, typename Pred, typename ...Args>
		requires
		is_math_type_v<T1> &&
		(sizeof...(Args) == T1::data_size)
	constexpr auto operator_base(T1& t1, Pred&& pred, Args&&...args)
	{
		return binary_apply<HasRet, All>(
										t1,
										std::forward<Pred>(pred),
										std::forward<Args>(args)...
										);
	}

	template<typename T1, typename T2>
	requires is_math_type_v<T1>
	constexpr T1& operator+=(T1& t1, const T2& t2)
	{
		operator_base(t1, t2, math_invoker<math_value_type<T1>>::template operator_add<math_value_type<T2>>);
		return t1;
	}
}
