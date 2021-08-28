#pragma once

#include <type_traits>

namespace gal::test::utils
{
	/**
	 * @brief a simple auxiliary class for handling some continuous (accessible through operator[]) containers
	 * @note all invokes provide two invocation methods, one is to specify index_sequence by user (requires the same length as N), and the other is to use std::make_index_sequence<N>
	*/
	struct sequence_invoker
	{
	private:
		template <typename Sequence1, typename Sequence2, typename Func, std::size_t...I1, std::size_t...I2, typename
				...Args>
		constexpr static void ternary_invoke_impl(
			Sequence1&                sequence1,
			const Sequence2&          sequence2,
			Func&&                    func,
			std::index_sequence<I1...>,
			std::index_sequence<I2...>,
			Args&&...                 args
			)
		noexcept((
			noexcept(
				func(
					std::declval<decltype(sequence2[I2])>(),
					std::declval<decltype(std::forward<Args>(args))>())
			) && ...
		))
		{
			((sequence1[I1] = func(sequence2[I2], args)), ...);
		}

		template <typename Sequence1, typename Sequence2, typename Sequence3, typename Func, std::size_t...I1,
				std::size_t...I2, std::size_t...I3>
		constexpr static void ternary_invoke_seq_impl(
			Sequence1&                sequence1,
			const Sequence2&          sequence2,
			const Sequence3&          sequence3,
			Func&&                    func,
			std::index_sequence<I1...>,
			std::index_sequence<I2...>,
			std::index_sequence<I3...>)
		noexcept((
			noexcept(
				func(
					std::declval<decltype(sequence2[I2])>(),
					std::declval<decltype(sequence3[I3])>())
			) && ...
		))
		{
			((sequence1[I1] = func(sequence2[I2], sequence3[I3])), ...);
		}

		template <typename Sequence1, typename Sequence2, typename T, typename Func, std::size_t...I1, std::size_t...I2>
		constexpr static void ternary_invoke_dup_impl(
			Sequence1&                sequence1,
			const Sequence2&          sequence2,
			T                         arg,
			Func&&                    func,
			std::index_sequence<I1...>,
			std::index_sequence<I2...>)
		noexcept((
			noexcept(
				func(
					std::declval<decltype(sequence2[I2])>(),
					std::declval<T>()
					)
			) && ...
		))
		{
			((sequence1[I1] = func(sequence2[I2], arg)), ...);
		}

		template <typename Sequence, typename Func, std::size_t...I, typename... Args>
		constexpr static void binary_invoke_impl(
			Sequence&                sequence,
			Func&&                   func,
			std::index_sequence<I...>,
			Args&&...                args
			)
		noexcept((
			noexcept(
				func(
					std::declval<decltype(sequence[I])>(),
					std::declval<decltype(std::forward<Args>(args))>()
					)
			) && ...
		))
		{
			(func(sequence[I], args), ...);
		}

		template <typename Sequence, typename Func, std::size_t...I, typename... Args>
		constexpr static bool binary_invoke_all_impl(
			Sequence&                sequence,
			Func&&                   func,
			std::index_sequence<I...>,
			Args&&...                args
			)
		noexcept((
			noexcept(
				func(
					std::declval<decltype(sequence[I])>(),
					std::declval<decltype(std::forward<Args>(args))>()
					)
			) && ...
		))
		{
			return [](std::convertible_to<bool> auto ...x)
			{
				return (x && ...);
			}(func(sequence[I], args)...);
		}

		template <typename Sequence, typename Func, std::size_t...I, typename... Args>
		constexpr static bool binary_invoke_any_impl(
			Sequence&                sequence,
			Func&&                   func,
			std::index_sequence<I...>,
			Args&&...                args
			)
		noexcept((
			noexcept(
				func(
					std::declval<decltype(sequence[I])>(),
					std::declval<decltype(std::forward<Args>(args))>()
					)
			) && ...
		))
		{
			return [](std::convertible_to<bool> auto ...x)
			{
				return (x || ...);
			}(func(sequence[I], args)...);
		}

		template <typename Sequence1, typename Sequence2, typename Func, std::size_t...I1, std::size_t...I2>
		constexpr static void binary_invoke_seq_impl(
			Sequence1&                sequence1,
			const Sequence2&          sequence2,
			Func&&                    func,
			std::index_sequence<I1...>,
			std::index_sequence<I2...>)
		noexcept((
			noexcept(
				func(
					std::declval<decltype(sequence1[I1])>(),
					std::declval<decltype(sequence2[I2])>()
					)
			) && ...
		))
		{
			(func(sequence1[I1], sequence2[I2]), ...);
		}

		template <typename Sequence1, typename Sequence2, typename Func, std::size_t...I1, std::size_t...I2>
		constexpr static bool binary_invoke_seq_all_impl(
			Sequence1&                sequence1,
			const Sequence2&          sequence2,
			Func&&                    func,
			std::index_sequence<I1...>,
			std::index_sequence<I2...>)
		noexcept((
			noexcept(
				func(
					std::declval<decltype(sequence1[I1])>(),
					std::declval<decltype(sequence2[I2])>()
					)
			) && ...
		))
		{
			return [](std::convertible_to<bool> auto ...x)
			{
				return (x && ...);
			}(func(sequence1[I1], sequence2[I2])...);
		}

		template <typename Sequence1, typename Sequence2, typename Func, std::size_t...I1, std::size_t...I2>
		constexpr static bool binary_invoke_seq_any_impl(
			Sequence1&                sequence1,
			const Sequence2&          sequence2,
			Func&&                    func,
			std::index_sequence<I1...>,
			std::index_sequence<I2...>)
		noexcept((
			noexcept(
				func(
					std::declval<decltype(sequence1[I1])>(),
					std::declval<decltype(sequence2[I2])>()
					)
			) && ...
		))
		{
			return [](std::convertible_to<bool> auto ...x)
			{
				return (x || ...);
			}(func(sequence1[I1], sequence2[I2])...);
		}

		template <typename Sequence, typename T, typename Func, std::size_t...I>
		constexpr static void binary_invoke_dup_impl(
			Sequence&                sequence,
			T                        arg,
			Func&&                   func,
			std::index_sequence<I...>)
		noexcept((
			noexcept(
				func(
					std::declval<decltype(sequence[I])>(),
					std::declval<T>()
					)
			) && ...
		))
		{
			(func(sequence[I], arg), ...);
		}

		template <typename Sequence, typename T, typename Func, std::size_t...I>
		constexpr static bool binary_invoke_dup_all_impl(
			Sequence&                sequence,
			T                        arg,
			Func&&                   func,
			std::index_sequence<I...>)
		noexcept((
			noexcept(
				func(
					std::declval<decltype(sequence[I])>(),
					std::declval<T>()
					)
			) && ...
		))
		{
			return [](std::convertible_to<bool> auto ...x)
			{
				return (x && ...);
			}(func(sequence[I], arg)...);
		}

		template <typename Sequence, typename T, typename Func, std::size_t...I>
		constexpr static bool binary_invoke_dup_any_impl(
			Sequence&                sequence,
			T                        arg,
			Func&&                   func,
			std::index_sequence<I...>)
		noexcept((
			noexcept(
				func(
					std::declval<decltype(sequence[I])>(),
					std::declval<T>()
					)
			) && ...
		))
		{
			return [](std::convertible_to<bool> auto ...x)
			{
				return (x || ...);
			}(func(sequence[I], arg)...);
		}

		template <typename Sequence, typename Func, std::size_t...I>
		constexpr static void unary_invoke_impl(
			Sequence&                sequence,
			Func&&                   func,
			std::index_sequence<I...>)
		noexcept((
			noexcept(
				func(std::declval<decltype(sequence[I])>())
			) && ...
		))
		{
			(func(sequence[I]), ...);
		}

		template <typename Sequence, typename Func, std::size_t...I>
		constexpr static bool unary_invoke_all_impl(
			Sequence&                sequence,
			Func&&                   func,
			std::index_sequence<I...>)
		noexcept((
			noexcept(
				func(std::declval<decltype(sequence[I])>())
			) && ...
		))
		{
			return [](std::convertible_to<bool> auto ...x)
			{
				return (x && ...);
			}(func(sequence[I])...);
		}

		template <typename Sequence, typename Func, std::size_t...I>
		constexpr static bool unary_invoke_any_impl(
			Sequence&                sequence,
			Func&&                   func,
			std::index_sequence<I...>)
		noexcept((
			noexcept(
				func(std::declval<decltype(sequence[I])>())
			) && ...
		))
		{
			return [](std::convertible_to<bool> auto ...x)
			{
				return (x || ...);
			}(func(sequence[I])...);
		}

	public:
		template <
			std::size_t N,
			typename Sequence1,
			typename Sequence2,
			typename Func,
			std::size_t...I1,
			std::size_t...I2,
			typename... Args
		>
			requires (sizeof...(I1) == N) && (sizeof...(I2) == N) && (sizeof...(Args) == N)
		constexpr static void ternary_invoke(
			Sequence1&                sequence1,
			const Sequence2&          sequence2,
			Func&&                    func,
			std::index_sequence<I1...>,
			std::index_sequence<I2...>,
			Args&&...                 args
			)
		noexcept(
			noexcept(
				sequence_invoker::ternary_invoke_impl(
													std::declval<decltype(sequence1)>(),
													std::declval<decltype(sequence2)>(),
													std::declval<decltype(func)>(),
													std::declval<std::index_sequence<I1...>>(),
													std::declval<std::index_sequence<I2...>>(),
													std::declval<decltype(std::forward<Args>(args))>()...
													)
			)
		)
		{
			sequence_invoker::ternary_invoke_impl(
												sequence1,
												sequence2,
												std::forward<Func>(func),
												std::index_sequence<I1...>{},
												std::index_sequence<I2...>{},
												std::forward<Args>(args)...
												);
		}

		template <
			std::size_t N,
			typename Sequence1,
			typename Sequence2,
			typename Func,
			typename... Args
		>
			requires (sizeof...(Args) == N)
		constexpr static void ternary_invoke(
			Sequence1&       sequence1,
			const Sequence2& sequence2,
			Func&&           func,
			Args&&...        args
			)
		noexcept(
			noexcept(
				sequence_invoker::ternary_invoke<N>(
													std::declval<decltype(sequence1)>(),
													std::declval<decltype(sequence2)>(),
													std::declval<decltype(func)>(),
													std::declval<std::make_index_sequence<N>>(),
													std::declval<std::make_index_sequence<N>>(),
													std::declval<decltype(std::forward<Args>(args))>()...
													)
			)
		)
		{
			sequence_invoker::ternary_invoke<N>(
												sequence1,
												sequence2,
												std::forward<Func>(func),
												std::make_index_sequence<N>{},
												std::make_index_sequence<N>{},
												std::forward<Args>(args)...
												);
		}

		template <
			std::size_t N,
			typename Sequence1,
			typename Sequence2,
			typename Sequence3,
			typename Func,
			std::size_t...I1,
			std::size_t...I2,
			std::size_t...I3
		>
			requires (sizeof...(I1) == N) && (sizeof...(I2) == N) && (sizeof...(I3) == N)
		constexpr static void ternary_invoke_seq(
			Sequence1&                sequence1,
			const Sequence2&          sequence2,
			const Sequence3&          sequence3,
			Func&&                    func,
			std::index_sequence<I1...>,
			std::index_sequence<I2...>,
			std::index_sequence<I3...>)
		noexcept(
			noexcept(
				sequence_invoker::ternary_invoke_seq_impl(
														std::declval<decltype(sequence1)>(),
														std::declval<decltype(sequence2)>(),
														std::declval<decltype(sequence3)>(),
														std::declval<decltype(func)>(),
														std::declval<std::index_sequence<I1...>>(),
														std::declval<std::index_sequence<I2...>>(),
														std::declval<std::index_sequence<I3...>>()
														)
			)
		)
		{
			sequence_invoker::ternary_invoke_seq_impl(
													sequence1,
													sequence2,
													sequence3,
													std::forward<Func>(func),
													std::index_sequence<I1...>{},
													std::index_sequence<I2...>{},
													std::index_sequence<I3...>{}
													);
		}

		template <
			std::size_t N,
			typename Sequence1,
			typename Sequence2,
			typename Sequence3,
			typename Func
		>
		constexpr static void ternary_invoke_seq(
			Sequence1&       sequence1,
			const Sequence2& sequence2,
			const Sequence3& sequence3,
			Func&&           func
			)
		noexcept(
			noexcept(
				sequence_invoker::ternary_invoke_seq<N>(
														std::declval<decltype(sequence1)>(),
														std::declval<decltype(sequence2)>(),
														std::declval<decltype(sequence3)>(),
														std::declval<decltype(func)>(),
														std::declval<std::make_index_sequence<N>>(),
														std::declval<std::make_index_sequence<N>>(),
														std::declval<std::make_index_sequence<N>>()
														)
			)
		)
		{
			sequence_invoker::ternary_invoke_seq<N>(
													sequence1,
													sequence2,
													sequence3,
													std::forward<Func>(func),
													std::make_index_sequence<N>{},
													std::make_index_sequence<N>{},
													std::make_index_sequence<N>{}
													);
		}

		template <
			std::size_t N,
			typename Sequence1,
			typename Sequence2,
			typename T,
			typename Func,
			std::size_t...I1,
			std::size_t...I2
		>
			requires (sizeof...(I1) == N) && (sizeof...(I2) == N)
		constexpr static void ternary_invoke_dup(
			Sequence1&                sequence1,
			const Sequence2&          sequence2,
			T                         dup_arg,
			Func&&                    func,
			std::index_sequence<I1...>,
			std::index_sequence<I2...>)
		noexcept(
			noexcept(
				sequence_invoker::ternary_invoke_dup_impl(
														std::declval<decltype(sequence1)>(),
														std::declval<decltype(sequence2)>(),
														std::declval<T>(),
														std::declval<decltype(func)>(),
														std::declval<std::index_sequence<I1...>>(),
														std::declval<std::index_sequence<I2...>>()
														)
			)
		)
		{
			sequence_invoker::ternary_invoke_dup_impl(
													sequence1,
													sequence2,
													dup_arg,
													std::forward<Func>(func),
													std::index_sequence<I1...>{},
													std::index_sequence<I2...>{}
													);
		}

		template <
			std::size_t N,
			typename Sequence1,
			typename Sequence2,
			typename T,
			typename Func
		>
		constexpr static void ternary_invoke_dup(
			Sequence1&       sequence1,
			const Sequence2& sequence2,
			T                dup_arg,
			Func&&           func
			)
		noexcept(
			noexcept(
				sequence_invoker::ternary_invoke_dup<N>(
														std::declval<decltype(sequence1)>(),
														std::declval<decltype(sequence2)>(),
														std::declval<T>(),
														std::declval<decltype(func)>(),
														std::declval<std::make_index_sequence<N>>(),
														std::declval<std::make_index_sequence<N>>()
														)
			)
		)
		{
			sequence_invoker::ternary_invoke_dup<N>(
													sequence1,
													sequence2,
													dup_arg,
													std::forward<Func>(func),
													std::make_index_sequence<N>{},
													std::make_index_sequence<N>{}
													);
		}

		template <
			std::size_t N,
			typename Sequence,
			typename Func,
			std::size_t ...I,
			typename... Args
		>
			requires (sizeof...(I) == N) && (sizeof...(Args) == N)
		constexpr static void binary_invoke(
			Sequence&                sequence,
			Func&&                   func,
			std::index_sequence<I...>,
			Args&&...                args
			)
		noexcept(
			noexcept(
				sequence_invoker::binary_invoke_impl(
													std::declval<decltype(sequence)>(),
													std::declval<decltype(func)>(),
													std::declval<std::index_sequence<I...>>(),
													std::declval<decltype(std::forward<Args>(args))>()...
													)
			)
		)
		{
			sequence_invoker::binary_invoke_impl(
												sequence,
												std::forward<Func>(func),
												std::index_sequence<I...>{},
												std::forward<Args>(args)...
												);
		}

		template <
			std::size_t N,
			typename Sequence,
			typename Func,
			typename... Args
		>
			requires (sizeof...(Args) == N)
		constexpr static void binary_invoke(
			Sequence& sequence,
			Func&&    func,
			Args&&... args
			)
		noexcept(
			noexcept(
				sequence_invoker::binary_invoke<N>(
													std::declval<decltype(sequence)>(),
													std::declval<decltype(func)>(),
													std::declval<std::make_index_sequence<N>>(),
													std::declval<decltype(std::forward<Args>(args))>()...
												)
			)
		)
		{
			sequence_invoker::binary_invoke<N>(
												sequence,
												std::forward<Func>(func),
												std::make_index_sequence<N>{},
												std::forward<Args>(args)...
											);
		}

		template <
			std::size_t N,
			typename Sequence,
			typename Func,
			std::size_t...I,
			typename... Args
		>
			requires (sizeof...(I) == N) && (sizeof...(Args) == N)
		constexpr static bool binary_invoke_all(
			Sequence&                sequence,
			Func&&                   func,
			std::index_sequence<I...>,
			Args&&...                args
			)
		noexcept(
			noexcept(
				sequence_invoker::binary_invoke_all_impl(
														std::declval<decltype(sequence)>(),
														std::declval<decltype(func)>(),
														std::declval<std::index_sequence<I...>>(),
														std::declval<decltype(std::forward<Args>(args))>()...
														)
			)
		)
		{
			return sequence_invoker::binary_invoke_all_impl(
															sequence,
															std::forward<Func>(func),
															std::index_sequence<I...>{},
															std::forward<Args>(args)...
															);
		}

		template <
			std::size_t N,
			typename Sequence,
			typename Func,
			typename... Args
		>
			requires (sizeof...(Args) == N)
		constexpr static bool binary_invoke_all(
			Sequence& sequence,
			Func&&    func,
			Args&&... args
			)
		noexcept(
			noexcept(
				sequence_invoker::binary_invoke_all<N>(
														std::declval<decltype(sequence)>(),
														std::declval<decltype(func)>(),
														std::declval<std::make_index_sequence<N>>(),
														std::declval<decltype(std::forward<Args>(args))>()...
													)
			)
		)
		{
			return sequence_invoker::binary_invoke_all<N>(
														sequence,
														std::forward<Func>(func),
														std::make_index_sequence<N>{},
														std::forward<Args>(args)...
														);
		}

		template <
			std::size_t N,
			typename Sequence,
			typename Func,
			std::size_t...I,
			typename... Args
		>
			requires (sizeof...(I) == N) && (sizeof...(Args) == N)
		constexpr static bool binary_invoke_any(
			Sequence&                sequence,
			Func&&                   func,
			std::index_sequence<I...>,
			Args&&...                args
			)
		noexcept(
			noexcept(
				sequence_invoker::binary_invoke_any_impl(
														std::declval<decltype(sequence)>(),
														std::declval<decltype(func)>(),
														std::declval<std::index_sequence<I...>>(),
														std::declval<decltype(std::forward<Args>(args))>()...
														)
			)
		)
		{
			return sequence_invoker::binary_invoke_any_impl(
															sequence,
															std::forward<Func>(func),
															std::index_sequence<I...>{},
															std::forward<Args>(args)...
															);
		}

		template <
			std::size_t N,
			typename Sequence,
			typename Func,
			typename... Args
		>
			requires (sizeof...(Args) == N)
		constexpr static bool binary_invoke_any(
			Sequence& sequence,
			Func&&    func,
			Args&&... args
			)
		noexcept(
			noexcept(
				sequence_invoker::binary_invoke_any<N>(
														std::declval<decltype(sequence)>(),
														std::declval<decltype(func)>(),
														std::declval<std::make_index_sequence<N>>(),
														std::declval<decltype(std::forward<Args>(args))>()...
													)
			)
		)
		{
			return sequence_invoker::binary_invoke_any<N>(
														sequence,
														std::forward<Func>(func),
														std::make_index_sequence<N>{},
														std::forward<Args>(args)...
														);
		}

		template <
			std::size_t N,
			typename Sequence1,
			typename Sequence2,
			typename Func,
			std::size_t...I1,
			std::size_t...I2
		>
			requires (sizeof...(I1) == N) && (sizeof...(I2) == N)
		constexpr static void binary_invoke_seq(
			Sequence1&                sequence1,
			const Sequence2&          sequence2,
			Func&&                    func,
			std::index_sequence<I1...>,
			std::index_sequence<I2...>)
		noexcept(
			noexcept(
				sequence_invoker::binary_invoke_seq_impl(
														std::declval<decltype(sequence1)>(),
														std::declval<decltype(sequence2)>(),
														std::declval<decltype(func)>(),
														std::declval<std::index_sequence<I1...>>(),
														std::declval<std::index_sequence<I2...>>()
														)
			)
		)
		{
			sequence_invoker::binary_invoke_seq_impl(
													sequence1,
													sequence2,
													std::forward<Func>(func),
													std::index_sequence<I1...>{},
													std::index_sequence<I1...>{}
													);
		}

		template <
			std::size_t N,
			typename Sequence1,
			typename Sequence2,
			typename Func
		>
		constexpr static void binary_invoke_seq(
			Sequence1&       sequence1,
			const Sequence2& sequence2,
			Func&&           func
			)
		noexcept(
			noexcept(
				sequence_invoker::binary_invoke_seq<N>(
														std::declval<decltype(sequence1)>(),
														std::declval<decltype(sequence2)>(),
														std::declval<decltype(func)>(),
														std::declval<std::make_index_sequence<N>>(),
														std::declval<std::make_index_sequence<N>>()
													)
			)
		)
		{
			sequence_invoker::binary_invoke_seq<N>(
													sequence1,
													sequence2,
													std::forward<Func>(func),
													std::make_index_sequence<N>{},
													std::make_index_sequence<N>{}
												);
		}

		template <
			std::size_t N,
			typename Sequence1,
			typename Sequence2,
			typename Func,
			std::size_t...I1,
			std::size_t...I2
		>
			requires (sizeof...(I1) == N) && (sizeof...(I2) == N)
		constexpr static bool binary_invoke_seq_all(
			Sequence1&                sequence1,
			const Sequence2&          sequence2,
			Func&&                    func,
			std::index_sequence<I1...>,
			std::index_sequence<I2...>)
		noexcept(
			noexcept(
				sequence_invoker::binary_invoke_seq_all_impl(
															std::declval<decltype(sequence1)>(),
															std::declval<decltype(sequence2)>(),
															std::declval<decltype(func)>(),
															std::declval<std::index_sequence<I1...>>(),
															std::declval<std::index_sequence<I2...>>()
															)
			)
		)
		{
			return sequence_invoker::binary_invoke_seq_all_impl(
																sequence1,
																sequence2,
																std::forward<Func>(func),
																std::index_sequence<I1...>{},
																std::index_sequence<I2...>{}
																);
		}

		template <
			std::size_t N,
			typename Sequence1,
			typename Sequence2,
			typename Func
		>
		constexpr static bool binary_invoke_seq_all(
			Sequence1&       sequence1,
			const Sequence2& sequence2,
			Func&&           func
			)
		noexcept(
			noexcept(
				sequence_invoker::binary_invoke_seq_all<N>(
															std::declval<decltype(sequence1)>(),
															std::declval<decltype(sequence2)>(),
															std::declval<decltype(func)>(),
															std::declval<std::make_index_sequence<N>>(),
															std::declval<std::make_index_sequence<N>>()
														)
			)
		)
		{
			return sequence_invoker::binary_invoke_seq_all<N>(
															sequence1,
															sequence2,
															std::forward<Func>(func),
															std::make_index_sequence<N>{},
															std::make_index_sequence<N>{}
															);
		}

		template <
			std::size_t N,
			typename Sequence1,
			typename Sequence2,
			typename Func,
			std::size_t...I1,
			std::size_t...I2
		>
			requires (sizeof...(I1) == N) && (sizeof...(I2) == N)
		constexpr static bool binary_invoke_seq_any(
			Sequence1&                sequence1,
			const Sequence2&          sequence2,
			Func&&                    func,
			std::index_sequence<I1...>,
			std::index_sequence<I2...>)
		noexcept(
			noexcept(
				sequence_invoker::binary_invoke_seq_any_impl(
															std::declval<decltype(sequence1)>(),
															std::declval<decltype(sequence2)>(),
															std::declval<decltype(func)>(),
															std::declval<std::index_sequence<I1...>>(),
															std::declval<std::index_sequence<I2...>>()
															)
			)
		)
		{
			return sequence_invoker::binary_invoke_seq_any_impl(
																sequence1,
																sequence2,
																std::forward<Func>(func),
																std::index_sequence<I1...>{},
																std::index_sequence<I2...>{}
																);
		}

		template <
			std::size_t N,
			typename Sequence1,
			typename Sequence2,
			typename Func
		>
		constexpr static bool binary_invoke_seq_any(
			Sequence1&       sequence1,
			const Sequence2& sequence2,
			Func&&           func
			)
		noexcept(
			noexcept(
				sequence_invoker::binary_invoke_seq_any<N>(
															std::declval<decltype(sequence1)>(),
															std::declval<decltype(sequence2)>(),
															std::declval<decltype(func)>(),
															std::declval<std::make_index_sequence<N>>(),
															std::declval<std::make_index_sequence<N>>()
														)
			)
		)
		{
			return sequence_invoker::binary_invoke_seq_any<N>(
															sequence1,
															sequence2,
															std::forward<Func>(func),
															std::make_index_sequence<N>{},
															std::make_index_sequence<N>{}
															);
		}

		template <
			std::size_t N,
			typename Sequence,
			typename Func,
			typename T,
			std::size_t...I
		>
			requires (sizeof...(I) == N)
		constexpr static void binary_invoke_dup(
			Sequence&                sequence,
			T                        dup_arg,
			Func&&                   func,
			std::index_sequence<I...>)
		noexcept(
			noexcept(
				sequence_invoker::binary_invoke_dup_impl(
														std::declval<decltype(sequence)>(),
														std::declval<T>(),
														std::declval<decltype(func)>(),
														std::declval<std::index_sequence<I...>>()
														)
			)
		)
		{
			sequence_invoker::binary_invoke_dup_impl(
													sequence,
													dup_arg,
													std::forward<Func>(func),
													std::index_sequence<I...>{}
													);
		}

		template <
			std::size_t N,
			typename Sequence,
			typename Func,
			typename T
		>
		constexpr static void binary_invoke_dup(
			Sequence& sequence,
			T         dup_arg,
			Func&&    func
			)
		noexcept(
			noexcept(
				sequence_invoker::binary_invoke_dup<N>(
														std::declval<decltype(sequence)>(),
														std::declval<T>(),
														std::declval<decltype(func)>(),
														std::declval<std::make_index_sequence<N>>()
													)
			)
		)
		{
			sequence_invoker::binary_invoke_dup<N>(
													sequence,
													dup_arg,
													std::forward<Func>(func),
													std::make_index_sequence<N>{}
												);
		}

		template <
			std::size_t N,
			typename Sequence,
			typename Func,
			typename T,
			std::size_t...I
		>
			requires (sizeof...(I) == N)
		constexpr static bool binary_invoke_dup_all(
			Sequence&                sequence,
			T                        dup_arg,
			Func&&                   func,
			std::index_sequence<I...>)
		noexcept(
			noexcept(
				sequence_invoker::binary_invoke_dup_all_impl(
															std::declval<decltype(sequence)>(),
															std::declval<T>(),
															std::declval<decltype(func)>(),
															std::declval<std::index_sequence<I...>>()

															)
			)
		)
		{
			return sequence_invoker::binary_invoke_dup_all_impl(
																sequence,
																dup_arg,
																std::forward<Func>(func),
																std::index_sequence<I...>{}
																);
		}

		template <
			std::size_t N,
			typename Sequence,
			typename Func,
			typename T
		>
		constexpr static bool binary_invoke_dup_all(
			Sequence& sequence,
			T         dup_arg,
			Func&&    func
			)
		noexcept(
			noexcept(
				sequence_invoker::binary_invoke_dup_all<N>(
															std::declval<decltype(sequence)>(),
															std::declval<T>(),
															std::declval<decltype(func)>(),
															std::declval<std::make_index_sequence<N>>()

														)
			)
		)
		{
			return sequence_invoker::binary_invoke_dup_all<N>(
															sequence,
															dup_arg,
															std::forward<Func>(func),
															std::make_index_sequence<N>{}
															);
		}

		template <
			std::size_t N,
			typename Sequence,
			typename Func,
			typename T,
			std::size_t...I
		>
			requires (sizeof...(I) == N)
		constexpr static bool binary_invoke_dup_any(
			Sequence&                sequence,
			T                        dup_arg,
			Func&&                   func,
			std::index_sequence<I...>)
		noexcept(
			noexcept(
				sequence_invoker::binary_invoke_dup_any_impl(
															std::declval<decltype(sequence)>(),
															std::declval<T>(),
															std::declval<decltype(func)>(),
															std::declval<std::index_sequence<I...>>()
															)
			)
		)
		{
			return sequence_invoker::binary_invoke_dup_any_impl(
																sequence,
																dup_arg,
																std::forward<Func>(func),
																std::index_sequence<I...>{}
																);
		}

		template <
			std::size_t N,
			typename Sequence,
			typename Func,
			typename T
		>
		constexpr static bool binary_invoke_dup_any(
			Sequence& sequence,
			T         dup_arg,
			Func&&    func
			)
		noexcept(
			noexcept(
				sequence_invoker::binary_invoke_dup_any<N>(
															std::declval<decltype(sequence)>(),
															std::declval<T>(),
															std::declval<decltype(func)>(),
															std::declval<std::make_index_sequence<N>>()
														)
			)
		)
		{
			return sequence_invoker::binary_invoke_dup_any<N>(
															sequence,
															dup_arg,
															std::forward<Func>(func),
															std::make_index_sequence<N>{}
															);
		}

		template <
			std::size_t N,
			typename Sequence,
			typename Func,
			std::size_t...I
		>
			requires (sizeof...(I) == N)
		constexpr static void unary_invoke(
			Sequence&                sequence,
			Func&&                   func,
			std::index_sequence<I...>)
		noexcept(
			noexcept(
				sequence_invoker::unary_invoke_impl(
													std::declval<decltype(sequence)>(),
													std::declval<decltype(func)>(),
													std::declval<std::index_sequence<I...>>()
													)
			)
		)
		{
			sequence_invoker::unary_invoke_impl(
												sequence,
												std::forward<Func>(func),
												std::index_sequence<I...>{}
												);
		}

		template <
			std::size_t N,
			typename Sequence,
			typename Func
		>
		constexpr static void unary_invoke(
			Sequence& sequence,
			Func&&    func
			)
		noexcept(
			noexcept(
				sequence_invoker::unary_invoke<N>(
												std::declval<decltype(sequence)>(),
												std::declval<decltype(func)>(),
												std::declval<std::make_index_sequence<N>>()
												)
			)
		)
		{
			sequence_invoker::unary_invoke<N>(
											sequence,
											std::forward<Func>(func),
											std::make_index_sequence<N>{}
											);
		}

		template <
			std::size_t N,
			typename Sequence,
			typename Func,
			std::size_t...I
		>
			requires (sizeof...(I) == N)
		constexpr static bool unary_invoke_all(
			Sequence&                sequence,
			Func&&                   func,
			std::index_sequence<I...>)
		noexcept(
			noexcept(
				sequence_invoker::unary_invoke_all_impl(
														std::declval<decltype(sequence)>(),
														std::declval<decltype(func)>(),
														std::declval<std::index_sequence<I...>>()
														)
			)
		)
		{
			return sequence_invoker::unary_invoke_all_impl(
															sequence,
															std::forward<Func>(func),
															std::index_sequence<I...>{}
														);
		}

		template <
			std::size_t N,
			typename Sequence,
			typename Func
		>
		constexpr static bool unary_invoke_all(
			Sequence& sequence,
			Func&&    func
			)
		noexcept(
			noexcept(
				sequence_invoker::unary_invoke_all<N>(
													std::declval<decltype(sequence)>(),
													std::declval<decltype(func)>(),
													std::declval<std::make_index_sequence<N>>()
													)
			)
		)
		{
			return sequence_invoker::unary_invoke_all<N>(
														sequence,
														std::forward<Func>(func),
														std::make_index_sequence<N>{}
														);
		}

		template <
			std::size_t N,
			typename Sequence,
			typename Func,
			std::size_t...I
		>
			requires (sizeof...(I) == N)
		constexpr static bool unary_invoke_any(
			Sequence&                sequence,
			Func&&                   func,
			std::index_sequence<I...>)
		noexcept(
			noexcept(
				sequence_invoker::unary_invoke_any_impl(
														std::declval<decltype(sequence)>(),
														std::declval<decltype(func)>(),
														std::declval<std::index_sequence<I...>>()
														)
			)
		)
		{
			return sequence_invoker::unary_invoke_any_impl(
															sequence,
															std::forward<Func>(func),
															std::index_sequence<I...>{}
														);
		}

		template <
			std::size_t N,
			typename Sequence,
			typename Func
		>
		constexpr static bool unary_invoke_any(
			Sequence& sequence,
			Func&&    func
			)
		noexcept(
			noexcept(
				sequence_invoker::unary_invoke_any<N>(
													std::declval<decltype(sequence)>(),
													std::declval<decltype(func)>(),
													std::declval<std::make_index_sequence<N>>()
													)
			)
		)
		{
			return sequence_invoker::unary_invoke_any<N>(
														sequence,
														std::forward<Func>(func),
														std::make_index_sequence<N>{}
														);
		}
	};
}
