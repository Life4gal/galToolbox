#pragma once

#include <type_traits>

namespace gal::test::utils
{
	/**
	 * @brief A simple auxiliary class for handling some continuous (accessible through operator[]) containers
	*/
	struct sequence_invoker
	{
	private:
		template <typename Sequence1, typename Sequence2, typename Func, std::size_t...I, typename ...Args>
		constexpr static void ternary_invoke_impl(
			Sequence1&               sequence1,
			const Sequence2&         sequence2,
			Func&&                   func,
			std::index_sequence<I...>,
			Args&&...                args
			)
		noexcept((
			noexcept(
				func(
					std::declval<decltype(sequence2[I])>(),
					std::declval<decltype(std::forward<Args>(args))>())
			) && ...
		))
		{
			((sequence1[I] = func(sequence2[I], args)), ...);
		}

		template <typename Sequence1, typename Sequence2, typename Sequence3, typename Func, std::size_t...I>
		constexpr static void ternary_invoke_seq_impl(
			Sequence1&               sequence1,
			const Sequence2&         sequence2,
			const Sequence3&         sequence3,
			Func&&                   func,
			std::index_sequence<I...>)
		noexcept((
			noexcept(
				func(
					std::declval<decltype(sequence2[I])>(),
					std::declval<decltype(sequence3[I])>())
			) && ...
		))
		{
			((sequence1[I] = func(sequence2[I], sequence3[I])), ...);
		}

		template <typename Sequence1, typename Sequence2, typename T, typename Func, std::size_t...I>
		constexpr static void ternary_invoke_dup_impl(
			Sequence1&               sequence1,
			const Sequence2&         sequence2,
			T                        arg,
			Func&&                   func,
			std::index_sequence<I...>)
		noexcept((
			noexcept(
				func(
					std::declval<decltype(sequence2[I])>(),
					std::declval<T>()
					)
			) && ...
		))
		{
			((sequence1[I] = func(sequence2[I], arg)), ...);
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

		template <typename Sequence1, typename Sequence2, typename Func, std::size_t...I>
		constexpr static void binary_invoke_seq_impl(
			Sequence1&               sequence1,
			const Sequence2&         sequence2,
			Func&&                   func,
			std::index_sequence<I...>)
		noexcept((
			noexcept(
				func(
					std::declval<decltype(sequence1[I])>(),
					std::declval<decltype(sequence2[I])>()
					)
			) && ...
		))
		{
			(func(sequence1[I], sequence2[I]), ...);
		}

		template <typename Sequence1, typename Sequence2, typename Func, std::size_t...I>
		constexpr static bool binary_invoke_seq_all_impl(
			Sequence1&               sequence1,
			const Sequence2&         sequence2,
			Func&&                   func,
			std::index_sequence<I...>)
		noexcept((
			noexcept(
				func(
					std::declval<decltype(sequence1[I])>(),
					std::declval<decltype(sequence2[I])>()
					)
			) && ...
		))
		{
			return [](std::convertible_to<bool> auto ...x)
			{
				return (x && ...);
			}(func(sequence1[I], sequence2[I])...);
		}

		template <typename Sequence1, typename Sequence2, typename Func, std::size_t...I>
		constexpr static bool binary_invoke_seq_any_impl(
			Sequence1&               sequence1,
			const Sequence2&         sequence2,
			Func&&                   func,
			std::index_sequence<I...>)
		noexcept((
			noexcept(
				func(
					std::declval<decltype(sequence1[I])>(),
					std::declval<decltype(sequence2[I])>()
					)
			) && ...
		))
		{
			return [](std::convertible_to<bool> auto ...x)
			{
				return (x || ...);
			}(func(sequence1[I], sequence2[I])...);
		}

		template <typename Sequence, typename Func, std::size_t...I, typename T>
		constexpr static void binary_invoke_dup_impl(
			Sequence&                sequence,
			Func&&                   func,
			std::index_sequence<I...>,
			T                        arg
			)
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

		template <typename Sequence, typename Func, std::size_t...I, typename T>
		constexpr static bool binary_invoke_dup_all_impl(
			Sequence&                sequence,
			Func&&                   func,
			std::index_sequence<I...>,
			T                        arg
			)
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

		template <typename Sequence, typename Func, std::size_t...I, typename T>
		constexpr static bool binary_invoke_dup_any_impl(
			Sequence&                sequence,
			Func&&                   func,
			std::index_sequence<I...>,
			T                        arg
			)
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
		template <std::size_t N, typename Sequence1, typename Sequence2, typename Func, typename... Args>
			requires (sizeof...(Args) == N)
		constexpr static void ternary_invoke(
			Sequence1&       sequence1,
			const Sequence2& sequence2,
			Func&&           func,
			Args&&...        args
			)
		noexcept(
			noexcept(
				sequence_invoker::ternary_invoke_impl(
													std::declval<decltype(sequence1)>(),
													std::declval<decltype(sequence2)>(),
													std::declval<decltype(func)>(),
													std::declval<std::make_index_sequence<N>>(),
													std::declval<decltype(std::forward<Args>(args))>()...
													)
			)
		)
		{
			sequence_invoker::ternary_invoke_impl(
												sequence1,
												sequence2,
												std::forward<Func>(func),
												std::make_index_sequence<N>{},
												std::forward<Args>(args)...
												);
		}

		template <std::size_t N, typename Sequence1, typename Sequence2, typename Sequence3, typename Func>
		constexpr static void ternary_invoke_seq(
			Sequence1&       sequence1,
			const Sequence2& sequence2,
			const Sequence3& sequence3,
			Func&&           func
			)
		noexcept(
			noexcept(
				sequence_invoker::ternary_invoke_seq_impl(
														std::declval<decltype(sequence1)>(),
														std::declval<decltype(sequence2)>(),
														std::declval<decltype(sequence3)>(),
														std::declval<decltype(func)>(),
														std::declval<std::make_index_sequence<N>>()
														)
			)
		)
		{
			sequence_invoker::ternary_invoke_seq_impl(
													sequence1,
													sequence2,
													sequence3,
													std::forward<Func>(func),
													std::make_index_sequence<N>{}
													);
		}

		template <std::size_t N, typename Sequence1, typename Sequence2, typename T, typename Func>
		constexpr static void ternary_invoke_dup(
			Sequence1&       sequence1,
			const Sequence2& sequence2,
			T                dup_arg,
			Func&&           func
			)
		noexcept(
			noexcept(
				sequence_invoker::ternary_invoke_dup_impl(
														std::declval<decltype(sequence1)>(),
														std::declval<decltype(sequence2)>(),
														std::declval<T>(),
														std::declval<decltype(func)>(),
														std::declval<std::make_index_sequence<N>>()
														)
			)
		)
		{
			sequence_invoker::ternary_invoke_dup_impl(
													sequence1,
													sequence2,
													dup_arg,
													std::forward<Func>(func),
													std::make_index_sequence<N>{}
													);
		}

		template <std::size_t N, typename Sequence, typename Func, typename... Args>
			requires (sizeof...(Args) == N)
		constexpr static void binary_invoke(
			Sequence& sequence,
			Func&&    func,
			Args&&... args
			)
		noexcept(
			noexcept(
				sequence_invoker::binary_invoke_impl(
													std::declval<decltype(sequence)>(),
													std::declval<decltype(func)>(),
													std::declval<std::make_index_sequence<N>>(),
													std::declval<decltype(std::forward<Args>(args))>()...
													)
			)
		)
		{
			sequence_invoker::binary_invoke_impl(
												sequence,
												std::forward<Func>(func),
												std::make_index_sequence<N>{},
												std::forward<Args>(args)...
												);
		}

		template <std::size_t N, typename Sequence, typename Func, typename... Args>
			requires (sizeof...(Args) == N)
		constexpr static bool binary_invoke_all(
			Sequence& sequence,
			Func&&    func,
			Args&&... args
			)
		noexcept(
			noexcept(
				sequence_invoker::binary_invoke_all_impl(
														std::declval<decltype(sequence)>(),
														std::declval<decltype(func)>(),
														std::declval<std::make_index_sequence<N>>(),
														std::declval<decltype(std::forward<Args>(args))>()...
														)
			)
		)
		{
			return sequence_invoker::binary_invoke_all_impl(
															sequence,
															std::forward<Func>(func),
															std::make_index_sequence<N>{},
															std::forward<Args>(args)...
															);
		}

		template <std::size_t N, typename Sequence, typename Func, typename... Args>
			requires (sizeof...(Args) == N)
		constexpr static bool binary_invoke_any(
			Sequence& sequence,
			Func&&    func,
			Args&&... args
			)
		noexcept(
			noexcept(
				sequence_invoker::binary_invoke_any_impl(
														std::declval<decltype(sequence)>(),
														std::declval<decltype(func)>(),
														std::declval<std::make_index_sequence<N>>(),
														std::declval<decltype(std::forward<Args>(args))>()...
														)
			)
		)
		{
			return sequence_invoker::binary_invoke_any_impl(
															sequence,
															std::forward<Func>(func),
															std::make_index_sequence<N>{},
															std::forward<Args>(args)...
															);
		}

		template <std::size_t N, typename Sequence1, typename Sequence2, typename Func>
		constexpr static void binary_invoke_seq(Sequence1& sequence1, const Sequence2& sequence2, Func&& func)
		noexcept(
			noexcept(
				sequence_invoker::binary_invoke_seq_impl(
														std::declval<decltype(sequence1)>(),
														std::declval<decltype(sequence2)>(),
														std::declval<decltype(func)>(),
														std::declval<std::make_index_sequence<N>>()
														)
			)
		)
		{
			sequence_invoker::binary_invoke_seq_impl(
													sequence1,
													sequence2,
													std::forward<Func>(func),
													std::make_index_sequence<N>{}
													);
		}

		template <std::size_t N, typename Sequence1, typename Sequence2, typename Func>
		constexpr static bool binary_invoke_seq_all(Sequence1& sequence1, const Sequence2& sequence2, Func&& func)
		noexcept(
			noexcept(
				sequence_invoker::binary_invoke_seq_all_impl(
															std::declval<decltype(sequence1)>(),
															std::declval<decltype(sequence2)>(),
															std::declval<decltype(func)>(),
															std::declval<std::make_index_sequence<N>>()
															)
			)
		)
		{
			return sequence_invoker::binary_invoke_seq_all_impl(
																sequence1,
																sequence2,
																std::forward<Func>(func),
																std::make_index_sequence<N>{}
																);
		}

		template <std::size_t N, typename Sequence1, typename Sequence2, typename Func>
		constexpr static bool binary_invoke_seq_any(Sequence1& sequence1, const Sequence2& sequence2, Func&& func)
		noexcept(
			noexcept(
				sequence_invoker::binary_invoke_seq_any_impl(
															std::declval<decltype(sequence1)>(),
															std::declval<decltype(sequence2)>(),
															std::declval<decltype(func)>(),
															std::declval<std::make_index_sequence<N>>()
															)
			)
		)
		{
			return sequence_invoker::binary_invoke_seq_any_impl(
																sequence1,
																sequence2,
																std::forward<Func>(func),
																std::make_index_sequence<N>{}
																);
		}

		template <std::size_t N, typename Sequence, typename Func, typename T>
		constexpr static void binary_invoke_dup(Sequence& sequence, T dup_arg, Func&& func)
		noexcept(
			noexcept(
				sequence_invoker::binary_invoke_dup_impl(
														std::declval<decltype(sequence)>(),
														std::declval<decltype(func)>(),
														std::declval<std::make_index_sequence<N>>(),
														std::declval<T>()
														)
			)
		)
		{
			sequence_invoker::binary_invoke_dup_impl(
													sequence,
													std::forward<Func>(func),
													std::make_index_sequence<N>{},
													dup_arg
													);
		}

		template <std::size_t N, typename Sequence, typename Func, typename T>
		constexpr static bool binary_invoke_dup_all(Sequence& sequence, T dup_arg, Func&& func)
		noexcept(
			noexcept(
				sequence_invoker::binary_invoke_dup_all_impl(
															std::declval<decltype(sequence)>(),
															std::declval<decltype(func)>(),
															std::declval<std::make_index_sequence<N>>(),
															std::declval<T>()
															)
			)
		)
		{
			return sequence_invoker::binary_invoke_dup_all_impl(
																sequence,
																std::forward<Func>(func),
																std::make_index_sequence<N>{},
																dup_arg
																);
		}

		template <std::size_t N, typename Sequence, typename Func, typename T>
		constexpr static bool binary_invoke_dup_any(Sequence& sequence, T dup_arg, Func&& func)
		noexcept(
			noexcept(
				sequence_invoker::binary_invoke_dup_any_impl(
															std::declval<decltype(sequence)>(),
															std::declval<decltype(func)>(),
															std::declval<std::make_index_sequence<N>>(),
															std::declval<T>()
															)
			)
		)
		{
			return sequence_invoker::binary_invoke_dup_any_impl(
																sequence,
																std::forward<Func>(func),
																std::make_index_sequence<N>{},
																dup_arg
																);
		}

		template <std::size_t N, typename Sequence, typename Func>
		constexpr static void unary_invoke(
			Sequence& sequence,
			Func&&    func
			)
		noexcept(
			noexcept(
				sequence_invoker::unary_invoke_impl(
													std::declval<decltype(sequence)>(),
													std::declval<decltype(func)>(),
													std::declval<std::make_index_sequence<N>>()
													)
			)
		)
		{
			sequence_invoker::unary_invoke_impl(
												sequence,
												std::forward<Func>(func),
												std::make_index_sequence<N>{}
												);
		}

		template <std::size_t N, typename Sequence, typename Func>
		constexpr static bool unary_invoke_all(
			Sequence& sequence,
			Func&&    func
			)
		noexcept(
			noexcept(
				sequence_invoker::unary_invoke_all_impl(
														std::declval<decltype(sequence)>(),
														std::declval<decltype(func)>(),
														std::declval<std::make_index_sequence<N>>()
														)
			)
		)
		{
			return sequence_invoker::unary_invoke_all_impl(
													sequence,
													std::forward<Func>(func),
													std::make_index_sequence<N>{}
													);
		}

		template <std::size_t N, typename Sequence, typename Func>
		constexpr static bool unary_invoke_any(
			Sequence& sequence,
			Func&&    func
			)
		noexcept(
			noexcept(
				sequence_invoker::unary_invoke_any_impl(
														std::declval<decltype(sequence)>(),
														std::declval<decltype(func)>(),
														std::declval<std::make_index_sequence<N>>()
														)
			)
		)
		{
			return sequence_invoker::unary_invoke_any_impl(
													sequence,
													std::forward<Func>(func),
													std::make_index_sequence<N>{}
													);
		}
	};
}
