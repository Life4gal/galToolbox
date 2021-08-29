#pragma once

#include "new_vector.hpp"
#include "new_matrix.hpp"
#include "../utils/sequence_invoker.hpp"

namespace gal::test::new_math
{
	template <typename T>
	constexpr static bool is_math_type_v =
		is_vector_type_v<T> ||
		is_matrix_type_v<T>;

	template <typename T>
	struct math_value_type_helper
	{
		using value_type = T;
	};

	template <typename T>
		requires is_math_type_v<T>
	struct math_value_type_helper<T>
	{
		using value_type = typename T::value_type;
	};

	template <typename T>
	using math_value_type = typename math_value_type_helper<T>::value_type;

	template <typename T1, typename T2, bool>
	struct is_math_same_size : std::false_type {};

	template <typename T1, typename T2, bool Dummy>
		requires
		is_vector_type_v<T1> &&
		is_vector_type_v<T2> &&
		(T1::data_size == T2::data_size)
	struct is_math_same_size<T1, T2, Dummy> : std::true_type {};

	template <typename T1, typename T2, bool Dummy>
		requires
		is_matrix_type_v<T1> &&
		is_matrix_type_v<T2> &&
		(T1::column_size == T2::column_size) &&
		(T1::row_size == T2::row_size)
	struct is_math_same_size<T1, T2, Dummy> : std::true_type {};

	template <typename T1, typename T2, bool AsRow>
		requires
		is_matrix_type_v<T1> &&
		is_vector_type_v<T2> &&
		(AsRow
			? (T1::row_size == T2::data_size)
			: (T1::column_size == T2::data_size)
		)
	struct is_math_same_size<T1, T2, AsRow> : std::true_type {};

	template <typename T1, typename T2, bool AsRow>
		requires
		is_vector_type_v<T1> &&
		is_matrix_type_v<T2> &&
		(AsRow
			? (T1::data_size == T2::row_size)
			: (T1::data_size == T2::column_size)
		)
	struct is_math_same_size<T1, T2, AsRow> : std::true_type {};

	template <typename T1, typename T2, bool AsRow = true>
	constexpr static bool is_math_same_size_v = is_math_same_size<T1, T2, AsRow>::value;

	/**
	 * @brief process a vector/matrix with parameters pack
	 * @tparam RetType because there is no way to get the return value type through Pred, it needs to be specified by the user (the user should ensure that RetType is compatible with the actual return type of Pred)
	 * @tparam U vector/matrix type
	 * @tparam Pred processing function type, it is required to accept two parameters (respectively the value_type of vector/matrix and parameters pack)
	 * @tparam I1 user-defined index_sequence instead of using std::make_index_sequence 
	 * @tparam I2 user-defined index_sequence instead of using std::make_index_sequence
	 * @tparam Args parameters pack type
	 * @param scalar vector/matrix
	 * @param pred processing function
	 * @param args parameters pack
	 * @return a vector/matrix, the value_type is the type returned by pred
	*/
	template <
		typename RetType,
		typename U,
		typename Pred,
		std::size_t...I1,
		std::size_t...I2,
		typename... Args
	>
		requires
		is_math_type_v<U> &&
		(sizeof...(I1) == sizeof...(I2)) &&
		(sizeof...(I1) == U::data_size) &&
		(sizeof...(Args) == U::data_size)
	constexpr auto ternary_apply(
		const U&                  scalar,
		Pred&&                    pred,
		std::index_sequence<I1...>,
		std::index_sequence<I2...>,
		Args&&...                 args
		)
	noexcept(noexcept(
		utils::sequence_invoker::ternary_invoke<U::data_size>(
															std::declval<typename U::template copy_type<RetType>&>(),
															std::declval<const U&>(),
															std::declval<decltype(std::forward<Pred>(pred))>(),
															std::declval<std::index_sequence<I1...>>(),
															std::declval<std::index_sequence<I2...>>(),
															std::declval<decltype(std::forward<Args>(args))>()...
															)
	))
	{
		typename U::template copy_type<RetType> ret{};
		utils::sequence_invoker::ternary_invoke<U::data_size>(
															ret,
															scalar,
															std::forward<Pred>(pred),
															std::index_sequence<I1...>{},
															std::index_sequence<I2...>{},
															std::forward<Args>(args)...
															);
		return ret;
	}

	/**
	* @brief process a vector/matrix with parameters pack
	* @tparam RetType because there is no way to get the return value type through Pred, it needs to be specified by the user (the user should ensure that RetType is compatible with the actual return type of Pred)
	* @tparam U vector/matrix type
	* @tparam Pred processing function type, it is required to accept two parameters (respectively the value_type of vector/matrix and parameters pack)
	* @tparam Args parameters pack type
	* @param scalar vector/matrix
	* @param pred processing function
	* @param args parameters pack
	* @return a vector/matrix, the value_type is the type returned by pred
	*/
	template <
		typename RetType,
		typename U,
		typename Pred,
		typename... Args
	>
		requires
		is_math_type_v<U> &&
		(sizeof...(Args) == U::data_size)
	constexpr auto ternary_apply(
		const U& scalar,
		Pred&&   pred,
		Args&&...args
		)
	noexcept(noexcept(
		ternary_apply<RetType>(
								std::declval<const U&>(),
								std::declval<decltype(std::forward<Pred>(pred))>(),
								std::declval<std::make_index_sequence<U::data_size>>(),
								std::declval<std::make_index_sequence<U::data_size>>(),
								std::declval<decltype(std::forward<Args>(args))>()...
							)
	))
	{
		return ternary_apply<RetType>(
									scalar,
									std::forward<Pred>(pred),
									std::make_index_sequence<U::data_size>{},
									std::make_index_sequence<U::data_size>{},
									std::forward<Args>(args)...
									);
	}

	/**
	 * @brief process two vectors/matrices to obtain a vector
	 * @tparam RetType because there is no way to get the return value type through Pred, it needs to be specified by the user (the user should ensure that RetType is compatible with the actual return type of Pred)
	 * @tparam U1 vector/matrices type1
	 * @tparam U2 vector/matrices type2
	 * @tparam Pred processing function type, it is required to accept two parameters (respectively the value_type of two vectors/matrices)
	 * @tparam I1 user-defined index_sequence instead of using std::make_index_sequence
	 * @tparam I2 user-defined index_sequence instead of using std::make_index_sequence
	 * @tparam I3 user-defined index_sequence instead of using std::make_index_sequence
	 * @param scalar1 vector/matrices1
	 * @param scalar2 vector/matrices2
	 * @param pred processing function
	 * @return a vector/matrices, the value_type is the type returned by pred
	*/
	template <
		typename RetType,
		typename U1,
		typename U2,
		typename Pred,
		std::size_t...I1,
		std::size_t...I2,
		std::size_t...I3>
		requires
		is_math_same_size_v<U1, U2> &&
		(sizeof...(I1) == sizeof...(I2)) &&
		(sizeof...(I2) == sizeof...(I3)) &&
		(sizeof...(I1) == U1::data_size)
	constexpr auto ternary_apply_seq(
		const U1&                 scalar1,
		const U2&                 scalar2,
		Pred&&                    pred,
		std::index_sequence<I1...>,
		std::index_sequence<I2...>,
		std::index_sequence<I3...>)
	noexcept(noexcept(
		utils::sequence_invoker::ternary_invoke_seq<U1::data_size>(
																	std::declval<typename U1::template copy_type<
																		RetType>&>(),
																	std::declval<const U1&>(),
																	std::declval<const U2&>(),
																	std::declval<decltype(std::forward<Pred>(pred))>(),
																	std::declval<std::index_sequence<I1...>>(),
																	std::declval<std::index_sequence<I2...>>(),
																	std::declval<std::index_sequence<I2...>>()
																)
	))
	{
		typename U1::template copy_type<RetType> ret{};
		utils::sequence_invoker::ternary_invoke_seq<U1::data_size>(
																	ret,
																	scalar1,
																	scalar2,
																	std::forward<Pred>(pred),
																	std::index_sequence<I1...>{},
																	std::index_sequence<I2...>{},
																	std::index_sequence<I2...>{}
																);
		return ret;
	}

	/**
	 * @brief process two vectors/matrices to obtain a vector
	 * @tparam RetType because there is no way to get the return value type through Pred, it needs to be specified by the user (the user should ensure that RetType is compatible with the actual return type of Pred)
	 * @tparam U1 vector/matrices type1
	 * @tparam U2 vector/matrices type2
	 * @tparam Pred processing function type, it is required to accept two parameters (respectively the value_type of two vectors/matrices)
	 * @param scalar1 vector/matrices1
	 * @param scalar2 vector/matrices2
	 * @param pred processing function
	 * @return a vector/matrices, the value_type is the type returned by pred
	*/
	template <
		typename RetType,
		typename U1,
		typename U2,
		typename Pred
	>
		requires
		is_math_same_size_v<U1, U2>
	constexpr auto ternary_apply_seq(
		const U1& scalar1,
		const U2& scalar2,
		Pred&&    pred
		)
	noexcept(noexcept(
		ternary_apply_seq<RetType>(
									std::declval<const U1&>(),
									std::declval<const U2&>(),
									std::declval<decltype(std::forward<Pred>(pred))>(),
									std::declval<std::make_index_sequence<U1::data_size>>(),
									std::declval<std::make_index_sequence<U1::data_size>>(),
									std::declval<std::make_index_sequence<U1::data_size>>()
								)
	))
	{
		return ternary_apply_seq<RetType>(
										scalar1,
										scalar2,
										std::forward<Pred>(pred),
										std::make_index_sequence<U1::data_size>{},
										std::make_index_sequence<U1::data_size>{},
										std::make_index_sequence<U1::data_size>{}
										);
	}

	/**
	 * @brief process a vector/matrix with a duplicate arg to obtain a vector/matrix
	 * @tparam RetType because there is no way to get the return value type through Pred, it needs to be specified by the user (the user should ensure that RetType is compatible with the actual return type of Pred)
	 * @tparam U  vector/matrix type
	 * @tparam T duplicate arg type
	 * @tparam Pred processing function type, it is required to accept two parameters (respectively the value_type of vector/matrix and duplicate arg)
	 * @tparam I1 user-defined index_sequence instead of using std::make_index_sequence
	 * @tparam I2 user-defined index_sequence instead of using std::make_index_sequence
	 * @param scalar vector/matrix
	 * @param dup_arg duplicate arg
	 * @param pred processing function
	 * @return a vector/matrix, the value_type is the type returned by pred
	*/
	template <
		typename RetType,
		typename U,
		typename T,
		typename Pred,
		std::size_t...I1,
		std::size_t...I2
	>
		requires is_math_type_v<U> &&
		(sizeof...(I1) == sizeof...(I2)) &&
		(sizeof...(I1) == U::data_size)
	constexpr auto ternary_apply_dup(
		const U&                  scalar,
		T                         dup_arg,
		Pred&&                    pred,
		std::index_sequence<I1...>,
		std::index_sequence<I2...>)
	noexcept(noexcept(
		utils::sequence_invoker::ternary_invoke_dup<U::data_size>(
																std::declval<typename U::template copy_type<RetType>
																	&>(),
																std::declval<const U&>(),
																std::declval<T>(),
																std::declval<decltype(std::forward<Pred>(pred))>(),
																std::declval<std::index_sequence<I1...>>(),
																std::declval<std::index_sequence<I2...>>()
																)
	))
	{
		typename U::template copy_type<RetType> ret{};
		utils::sequence_invoker::ternary_invoke_dup<U::data_size>(
																ret,
																scalar,
																dup_arg,
																std::forward<Pred>(pred),
																std::index_sequence<I1...>{},
																std::index_sequence<I2...>{}
																);
		return ret;
	}

	/**
	 * @brief process a vector/matrix with a duplicate arg to obtain a vector/matrix
	 * @tparam RetType because there is no way to get the return value type through Pred, it needs to be specified by the user (the user should ensure that RetType is compatible with the actual return type of Pred)
	 * @tparam U  vector/matrix type
	 * @tparam T duplicate arg type
	 * @tparam Pred processing function type, it is required to accept two parameters (respectively the value_type of vector/matrix and duplicate arg)
	 * @param scalar vector/matrix
	 * @param dup_arg duplicate arg
	 * @param pred processing function
	 * @return a vector/matrix, the value_type is the type returned by pred
	*/
	template <
		typename RetType,
		typename U,
		typename T,
		typename Pred
	>
		requires is_math_type_v<U>
	constexpr auto ternary_apply_dup(
		const U& scalar,
		T        dup_arg,
		Pred&&   pred
		)
	noexcept(noexcept(
		ternary_apply_dup<RetType>(
									std::declval<const U&>(),
									std::declval<T>(),
									std::declval<decltype(std::forward<Pred>(pred))>(),
									std::declval<std::make_index_sequence<U::data_size>>(),
									std::declval<std::make_index_sequence<U::data_size>>()
								)
	))
	{
		return ternary_apply_dup<RetType>(
										scalar,
										dup_arg,
										std::forward<Pred>(pred),
										std::make_index_sequence<U::data_size>{},
										std::make_index_sequence<U::data_size>{}
										);
	}

	/**
	 * @brief process a vector/matrix with parameters pack
	 * @tparam HasRet does the function need to return a value
	 * @tparam All the superposition method of the return value (bool), is it All(&&) or Any(||)
	 * @tparam U vector/matrix type
	 * @tparam Pred processing function type, it is required to accept two parameters (respectively the value_type of vector/matrix and parameters pack)
	 * @tparam I user-defined index_sequence instead of using std::make_index_sequence
	 * @tparam Args parameters pack type
	 * @param scalar vector/matrix
	 * @param pred processing function
	 * @param args parameters pack
	 * @return bool or void
	*/
	template <
		bool HasRet = false,
		bool All = true,
		typename U,
		typename Pred,
		std::size_t...I,
		typename ...Args
	>
		requires
		is_math_type_v<U> &&
		(sizeof...(I) == U::data_size) &&
		(sizeof...(Args) == U::data_size)
	// ReSharper disable once CppNotAllPathsReturnValue (actually all paths have return value)
	constexpr std::conditional_t<HasRet, bool, void> binary_apply(
		U&                       scalar,
		Pred&&                   pred,
		std::index_sequence<I...>,
		Args&&...                args
		)
	noexcept(
		noexcept(
			utils::sequence_invoker::binary_invoke<U::data_size, HasRet, All>(
																			std::declval<U&>(),
																			std::declval<decltype(std::forward<
																				Pred>(pred))>(),
																			std::declval<std::index_sequence<I...>>(),
																			std::declval<decltype(std::forward<
																				Args>(args))>()...
																			)
		)
	)
	{
		return utils::sequence_invoker::binary_invoke<U::data_size, HasRet, All>(
																				scalar,
																				std::forward<Pred>(pred),
																				std::index_sequence<I...>{},
																				std::forward<Args>(args)...
																				);
	}

	/**
	 * @brief process a vector/matrix with parameters pack
	 * @tparam HasRet does the function need to return a value
	 * @tparam All the superposition method of the return value (bool), is it All(&&) or Any(||)
	 * @tparam U vector/matrix type 
	 * @tparam Pred processing function type, it is required to accept two parameters (respectively the value_type of vector/matrix and parameters pack)
	 * @tparam Args parameters pack type
	 * @param scalar vector/matrix
	 * @param pred processing function
	 * @param args parameters pack
	 * @return bool or void
	*/
	template <
		bool HasRet = false,
		bool All = true,
		typename U,
		typename Pred,
		typename ...Args
	>
		requires
		is_math_type_v<U> &&
		(sizeof...(Args) == U::data_size)
	constexpr std::conditional_t<HasRet, bool, void> binary_apply(
		U&       scalar,
		Pred&&   pred,
		Args&&...args
		)
	noexcept(
		noexcept(
			binary_apply<HasRet, All>(
									std::declval<U&>(),
									std::declval<decltype(std::forward<Pred>(pred))>(),
									std::declval<std::make_index_sequence<U::data_size>>(),
									std::declval<decltype(std::forward<Args>(args))>()...
									)
		)
	)
	{
		return binary_apply<HasRet, All>(
										scalar,
										std::forward<Pred>(pred),
										std::make_index_sequence<U::data_size>{},
										std::forward<Args>(args)...
										);
	}

	/**
	 * @brief process two vectors/matrices
	 * @tparam HasRet does the function need to return a value
	 * @tparam All the superposition method of the return value (bool), is it All(&&) or Any(||)
	 * @tparam U1 vector/matrix type1
	 * @tparam U2 vector/matrix type2
	 * @tparam Pred processing function type, it is required to accept two parameters (respectively the value_type of two vectors/matrices)
	 * @tparam I1 user-defined index_sequence instead of using std::make_index_sequence
	 * @tparam I2 user-defined index_sequence instead of using std::make_index_sequence
	 * if a return value is required, the function must return bool (or it can be converted to bool)
	 * the superposition method depends on the template parameter All
	 * @param scalar1 vector/matrix1
	 * @param scalar2 vector/matrix2
	 * @param pred processing function
	 * @return bool or void
	*/
	template <
		bool HasRet = false,
		bool All = true,
		typename U1,
		typename U2,
		typename Pred,
		std::size_t...I1,
		std::size_t...I2
	>
		requires is_math_same_size_v<U1, U2> &&
		(sizeof...(I1) == sizeof...(I2)) &&
		(sizeof...(I1) == U1::data_size)
	// ReSharper disable once CppNotAllPathsReturnValue (actually all paths have return value)
	constexpr std::conditional_t<HasRet, bool, void> binary_apply_seq(
		U1&                       scalar1,
		const U2&                 scalar2,
		Pred&&                    pred,
		std::index_sequence<I1...>,
		std::index_sequence<I2...>)
	noexcept(
		noexcept(
			utils::sequence_invoker::binary_invoke_seq<U1::data_size, HasRet, All>(
																					std::declval<U1&>(),
																					std::declval<const U2&>(),
																					std::declval<decltype(std::forward<
																						Pred>(pred))>(),
																					std::declval<std::index_sequence<I1
																						...>>(),
																					std::declval<std::index_sequence<I2
																						...>>()
																				)
		)
	)
	{
		return utils::sequence_invoker::binary_invoke_seq<U1::data_size, HasRet, All>(
																					scalar1,
																					scalar2,
																					std::forward<Pred>(pred),
																					std::index_sequence<I1...>{},
																					std::index_sequence<I2...>{}
																					);
	}

	/**
	 * @brief process two vectors/matrices
	 * @tparam HasRet does the function need to return a value
	 * @tparam All the superposition method of the return value (bool), is it All(&&) or Any(||)
	 * @tparam U1 vector/matrix type1
	 * @tparam U2 vector/matrix type2
	 * @tparam Pred processing function type, it is required to accept two parameters (respectively the value_type of two vectors/matrices)
	 * if a return value is required, the function must return bool (or it can be converted to bool)
	 * the superposition method depends on the template parameter All
	 * @param scalar1 vector/matrix1
	 * @param scalar2 vector/matrix2
	 * @param pred processing function
	 * @return bool or void
	*/
	template <
		bool HasRet = false,
		bool All = true,
		typename U1,
		typename U2,
		typename Pred
	>
		requires is_math_same_size_v<U1, U2>
	// ReSharper disable once CppNotAllPathsReturnValue (actually all paths have return value)
	constexpr std::conditional_t<HasRet, bool, void> binary_apply_seq(
		U1&       scalar1,
		const U2& scalar2,
		Pred&&    pred
		)
	noexcept(
		noexcept(
			binary_apply_seq<HasRet, All>(
										std::declval<U1&>(),
										std::declval<const U2&>(),
										std::declval<decltype(std::forward<Pred>(pred))>(),
										std::declval<std::make_index_sequence<U1::data_size>>(),
										std::declval<std::make_index_sequence<U1::data_size>>()
										)
		)
	)
	{
		return binary_apply_seq<HasRet, All>(
											scalar1,
											scalar2,
											std::forward<Pred>(pred),
											std::make_index_sequence<U1::data_size>{},
											std::make_index_sequence<U1::data_size>{}
											);
	}

	/**
	 * @brief process a vector/matrix with a duplicate arg
	 * @tparam HasRet does the function need to return a value
	 * @tparam All the superposition method of the return value (bool), is it All(&&) or Any(||)
	 * @tparam U  vector/matrix type
	 * @tparam T duplicate arg type
	 * @tparam Pred processing function type, it is required to accept two parameters (respectively the value_type of vector/matrix and duplicate arg)
	 * @tparam I user-defined index_sequence instead of using std::make_index_sequence
	 * @param scalar vector/matrix
	 * @param dup_arg duplicate arg
	 * @param pred processing function
	 * @return bool or void
	*/
	template <
		bool HasRet = false,
		bool All = true,
		typename U,
		typename T,
		typename Pred,
		std::size_t...I
	>
		requires is_math_type_v<U> &&
		(sizeof...(I) == U::data_size)
	constexpr std::conditional_t<HasRet, bool, void> binary_apply_dup(
		U&                       scalar,
		T                        dup_arg,
		Pred&&                   pred,
		std::index_sequence<I...>)
	noexcept(
		noexcept(
			utils::sequence_invoker::binary_invoke_dup<U::data_size, HasRet, All>(
																				std::declval<U&>(),
																				std::declval<T>(),
																				std::declval<decltype(std::forward<
																					Pred>(pred))>(),
																				std::declval<std::index_sequence<I
																					...>>()
																				)
		)
	)
	{
		return utils::sequence_invoker::binary_invoke_dup<U::data_size, HasRet, All>(
																					scalar,
																					dup_arg,
																					std::forward<Pred>(pred),
																					std::index_sequence<I...>{}
																					);
	}

	/**
	 * @brief process a vector/matrix with a duplicate arg
	 * @tparam HasRet does the function need to return a value
	 * @tparam All the superposition method of the return value (bool), is it All(&&) or Any(||)
	 * @tparam U  vector/matrix type
	 * @tparam T duplicate arg type
	 * @tparam Pred processing function type, it is required to accept two parameters (respectively the value_type of vector/matrix and duplicate arg)
	 * @param scalar vector/matrix
	 * @param dup_arg duplicate arg
	 * @param pred processing function
	 * @return bool or void
	*/
	template <
		bool HasRet = false,
		bool All = true,
		typename U,
		typename T,
		typename Pred
	>
		requires is_math_type_v<U>
	constexpr std::conditional_t<HasRet, bool, void> binary_apply_dup(
		U&     scalar,
		T      dup_arg,
		Pred&& pred
		)
	noexcept(
		noexcept(
			binary_apply_dup<HasRet, All>(
										std::declval<U&>(),
										std::declval<T>(),
										std::declval<decltype(std::forward<Pred>(pred))>(),
										std::declval<std::make_index_sequence<U::data_size>>()
										)
		)
	)
	{
		return binary_apply_dup<HasRet, All>(
											scalar,
											dup_arg,
											std::forward<Pred>(pred),
											std::make_index_sequence<U::data_size>{}
											);
	}

	/**
	 * @brief process a vector/matrix
	 * @tparam HasRet does the function need to return a value
	 * @tparam All the superposition method of the return value (bool), is it All(&&) or Any(||)
	 * @tparam U  vector/matrix type
	 * @tparam Pred processing function type, it is required to accept one parameter (respectively the value_type of vector/matrix)
	 * @tparam I user-defined index_sequence instead of using std::make_index_sequence
	 * @param scalar vector/matrix
	 * @param pred processing function
	 * @return bool or void
	*/
	template <
		bool HasRet = false,
		bool All = true,
		typename U,
		typename Pred,
		std::size_t...I
	>
		requires is_math_type_v<U> &&
		(sizeof...(I) == U::data_size)
	constexpr std::conditional_t<HasRet, bool, void> unary_apply(
		U&                       scalar,
		Pred&&                   pred,
		std::index_sequence<I...>)
	noexcept(
		noexcept(
			utils::sequence_invoker::unary_invoke<U::data_size>(
																std::declval<U&>(),
																std::declval<decltype(std::forward<Pred>(pred))>(),
																std::declval<std::index_sequence<I...>>()
																)
		)
	)
	{
		return utils::sequence_invoker::unary_invoke<U::data_size, HasRet, All>(
																				scalar,
																				std::forward<Pred>(pred),
																				std::index_sequence<I...>{}
																				);
	}

	/**
	 * @brief process a vector/matrix
	 * @tparam HasRet does the function need to return a value
	 * @tparam All the superposition method of the return value (bool), is it All(&&) or Any(||)
	 * @tparam U  vector/matrix type
	 * @tparam Pred processing function type, it is required to accept one parameter (respectively the value_type of vector/matrix)
	 * @param scalar vector/matrix
	 * @param pred processing function
	 * @return bool or void
	*/
	template <
		bool HasRet = false,
		bool All = true,
		typename U,
		typename Pred
	>
		requires is_math_type_v<U>
	constexpr std::conditional_t<HasRet, bool, void> unary_apply(
		U&     scalar,
		Pred&& pred
		)
	noexcept(
		noexcept(
			unary_apply<HasRet, All>(
									std::declval<U&>(),
									std::declval<decltype(std::forward<Pred>(pred))>(),
									std::declval<std::make_index_sequence<U::data_size>>()
									)
		)
	)
	{
		return unary_apply<HasRet, All>(
										scalar,
										std::forward<Pred>(pred),
										std::make_index_sequence<U::data_size>{}
										);
	}

	/**
	 * @brief specialize math_invoker for the actual implementer of the operation
	 * @note if the operation corresponding to the target type is not implemented, the corresponding operation is not allowed
	 * @tparam T specialized type
	*/
	template <typename T>
	struct math_invoker : std::false_type {};

	/**
	 * @brief give some basic operations for the arithmetic type
	 * @tparam T arithmetic
	*/
	template <typename T>
		requires std::is_arithmetic_v<T>
	struct math_invoker<T> : std::true_type
	{
		using value_type = T;
		static_assert(std::is_arithmetic_v<value_type>);
	private:
		template <bool Left, typename U>
		constexpr static void shift_impl(value_type& value, U scalar) noexcept
		{
			if constexpr (std::is_floating_point_v<value_type>)
			{
				bool s_lt_0 = scalar < 0;
				if (s_lt_0)
				{
					scalar = -scalar;
				}

				if constexpr (
					constexpr auto shift = [](value_type& v, const bool left, auto s) constexpr noexcept -> void
					{
						if (
							constexpr auto pow = [](auto base, auto p) constexpr noexcept -> auto
							{
								auto ret = base;
								while (--p)
								{
									ret *= base;
								}
								return ret;
							};
							left)
						{
							v *= pow(2,
									static_cast<std::conditional_t<
										(sizeof(T) > sizeof(std::conditional_t<
											std::is_signed_v<T>, std::int32_t, std::uint32_t>)),
										std::conditional_t<std::is_signed_v<T>, std::int64_t, std::uint64_t>,
										std::conditional_t<std::is_signed_v<T>, std::int32_t, std::uint32_t>>>(
										s));
						}
						else
						{
							v /= pow(2,
									static_cast<std::conditional_t<
										(sizeof(T) > sizeof(std::conditional_t<
											std::is_signed_v<T>, std::int32_t, std::uint32_t>)),
										std::conditional_t<std::is_signed_v<T>, std::int64_t, std::uint64_t>,
										std::conditional_t<std::is_signed_v<T>, std::int32_t, std::uint32_t>>>(
										s));
						}
					}; Left)
				{
					shift(value, !s_lt_0, scalar);
				}
				else
				{
					shift(value, s_lt_0, scalar);
				}
			}
			else
			{
				if constexpr (Left)
				{
					value <<= static_cast<value_type>(scalar);
				}
				else
				{
					value >>= static_cast<value_type>(scalar);
				}
			}
		}

	public:
		// unchecked
		template <typename U>
			requires std::is_convertible_v<U, value_type>
		constexpr static void operator_add(
			value_type& value,
			U           scalar
			) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			value += static_cast<value_type>(scalar);
		}

		// unchecked
		template <typename U>
			requires std::is_convertible_v<U, value_type>
		constexpr static void operator_subtract(
			value_type& value,
			U           scalar
			) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			value -= static_cast<value_type>(scalar);
		}

		// unchecked
		template <typename U>
			requires std::is_convertible_v<U, value_type>
		constexpr static void operator_multiply(
			value_type& value,
			U           scalar
			) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			value *= static_cast<value_type>(scalar);
		}

		// unchecked
		template <typename U>
			requires std::is_convertible_v<U, value_type>
		constexpr static void operator_divide(
			value_type& value,
			U           scalar
			) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			value /= static_cast<value_type>(scalar);
		}

		// unchecked
		template <typename U>
			requires std::is_convertible_v<U, value_type>
		constexpr static void operator_model(
			value_type& value,
			U           scalar
			) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			if constexpr (std::is_floating_point_v<value_type>)
			{
				if (scalar < 0)
				{
					scalar = -scalar;
				}

				if (value < 0)
				{
					auto n = static_cast<std::conditional_t<
						(sizeof(value_type) > sizeof(std::int32_t)), std::int64_t, std::int32_t>>(-value / static_cast<
						value_type>(scalar));
					value += static_cast<value_type>(n) * static_cast<decltype(n)>(scalar);
				}
				else
				{
					auto n = static_cast<std::conditional_t<
						(sizeof(value_type) > sizeof(std::uint32_t)), std::uint64_t, std::uint32_t>>(value / static_cast
						<value_type>(scalar));
					value -= static_cast<value_type>(n) * static_cast<decltype(n)>(scalar);
				}
			}
			else
			{
				value %= static_cast<value_type>(scalar);
			}
		}

		// unchecked
		template <typename U>
			requires std::is_convertible_v<U, value_type>
		constexpr static void operator_and(
			value_type& value,
			U           scalar
			) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			value &= static_cast<value_type>(scalar);
		}

		// unchecked
		template <typename U>
			requires std::is_convertible_v<U, value_type>
		constexpr static void operator_or(
			value_type& value,
			U           scalar
			) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			value |= static_cast<value_type>(scalar);
		}

		// unchecked
		template <typename U>
			requires std::is_convertible_v<U, value_type>
		constexpr static void operator_xor(
			value_type& value,
			U           scalar
			) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			value ^= static_cast<value_type>(scalar);
		}

		// unchecked
		template <typename U>
			requires std::is_convertible_v<U, value_type>
		constexpr static void operator_left_shift(
			value_type& value,
			U           scalar
			) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			math_invoker::template shift_impl<true>(value, scalar);
		}

		// unchecked
		template <typename U>
			requires std::is_convertible_v<U, value_type>
		constexpr static void operator_right_shift(
			value_type& value,
			U           scalar
			) noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			math_invoker::template shift_impl<false>(value, scalar);
		}

		// unchecked
		constexpr static void operator_unary_minus(value_type& value)
		noexcept
		{
			value = -value;
		}

		// unchecked
		constexpr static void operator_unary_tilde(value_type& value)
		noexcept
		{
			value = ~value;
		}

		// unchecked
		template <typename U>
			requires std::is_convertible_v<U, value_type>
		constexpr static bool operator_equal_to(value_type v1, U v2)
		noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			return v1 == v2;
		}

		// unchecked
		template <typename U>
			requires (std::is_convertible_v<U, value_type> && std::is_floating_point_v<value_type>)
		constexpr static bool operator_equal_to(value_type v1, U v2, value_type epsilon = 0.000001)
		noexcept(std::is_nothrow_convertible_v<U, value_type>)
		{
			return static_cast<double>(v1) - static_cast<double>(v2) <= epsilon &&
				static_cast<double>(v1) - static_cast<double>(v2) >= -epsilon;
		}

		// unchecked
		template <typename U1, typename U2>
			requires std::is_same_v<value_type, bool>
		constexpr static bool operator_less_than(U1 scalar1, U2 scalar2) noexcept
		{
			return scalar1 < scalar2;
		}

		// unchecked
		template <typename U1, typename U2>
			requires std::is_same_v<value_type, bool>
		constexpr static bool operator_less_equal_than(U1 scalar1, U2 scalar2) noexcept
		{
			return scalar1 < scalar2 || scalar1 == scalar2;
		}

		// unchecked
		template <typename U1, typename U2>
			requires std::is_same_v<value_type, bool>
		constexpr static bool operator_greater_than(U1 scalar1, U2 scalar2) noexcept
		{
			return scalar1 > scalar2;
		}

		// unchecked
		template <typename U1, typename U2>
			requires std::is_same_v<value_type, bool>
		constexpr static bool operator_greater_equal_than(U1 scalar1, U2 scalar2) noexcept
		{
			return scalar1 > scalar2 || scalar1 == scalar2;
		}
	};
}
