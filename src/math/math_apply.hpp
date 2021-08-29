#pragma once

#include "new_vector.hpp"
#include "new_matrix.hpp"
#include "../utils/sequence_invoker.hpp"

namespace gal::test::new_math
{
	template <typename T>
	constexpr static bool is_math_type_v =
		is_vector_type_v<T> ||
		is_matrix_v<T>;

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
		is_matrix_v<T1> &&
		is_matrix_v<T2> &&
		(T1::column_size == T2::column_size) &&
		(T1::row_size == T2::row_size)
	struct is_math_same_size<T1, T2, Dummy> : std::true_type {};

	template <typename T1, typename T2, bool AsRow>
		requires
		is_matrix_v<T1> &&
		is_vector_type_v<T2> &&
		(AsRow
			? (T1::row_size == T2::data_size)
			: (T1::column_size == T2::data_size)
		)
	struct is_math_same_size<T1, T2, AsRow> : std::true_type {};

	template <typename T1, typename T2, bool AsRow>
		requires
		is_vector_type_v<T1> &&
		is_matrix_v<T2> &&
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

	// matrix + matrix / vector + vector
	template <bool HasRet = false, bool All = true, typename T1, typename T2, typename Pred>
		requires
		(
			(is_matrix_v<T1> && is_matrix_v<T2>) ||
			(is_vector_v<T1> && is_vector_type_v<T2>)
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

	// vector_view + vector/vector_view
	template <bool HasRet = false, bool All = true, typename T1, typename T2, typename Pred>
		requires
		(is_vector_view_v<T1> && is_vector_type_v<T2>) &&
		is_math_same_size_v<T1, T2>
	constexpr auto operator_base(T1 t1, const T2& t2, Pred&& pred)
	{
		return binary_apply_seq<HasRet, All>(
											t1,
											t2,
											std::forward<Pred>(pred)
											);
	}

	// one matrix/vector, one arg
	template <bool HasRet = false, bool All = true, typename T1, typename T2, typename Pred>
		requires
		(is_vector_v<T1> || is_matrix_v<T1>) &&
		(!is_math_type_v<T2>)
	constexpr auto operator_base(T1& t1, T2 t2, Pred&& pred)
	{
		return binary_apply_dup<HasRet, All>(
											t1,
											t2,
											std::forward<Pred>(pred)
											);
	}

	// one vector_view, one arg
	template <bool HasRet = false, bool All = true, typename T1, typename T2, typename Pred>
		requires
		is_vector_view_v<T1> &&
		(!is_math_type_v<T2>)
	constexpr auto operator_base(T1 t1, T2 t2, Pred&& pred)
	{
		return binary_apply_dup<HasRet, All>(
											t1,
											t2,
											std::forward<Pred>(pred)
											);
	}

	// one matrix/vector, one parameters pack
	template <bool HasRet = false, bool All = true, typename T1, typename Pred, typename ...Args>
		requires
		(is_matrix_v<T1> || is_vector_v<T1>) &&
		(sizeof...(Args) == T1::data_size)
	constexpr auto operator_base(T1& t1, Pred&& pred, Args&&...args)
	{
		return binary_apply<HasRet, All>(
										t1,
										std::forward<Pred>(pred),
										std::forward<Args>(args)...
										);
	}

	// one vector_view, one parameters pack
	template <bool HasRet = false, bool All = true, typename T1, typename Pred, typename ...Args>
		requires
		is_vector_view_v<T1> &&
		(sizeof...(Args) == T1::data_size)
	constexpr auto operator_base(T1 t1, Pred&& pred, Args&&...args)
	{
		return binary_apply<HasRet, All>(
										t1,
										std::forward<Pred>(pred),
										std::forward<Args>(args)...
										);
	}
}
