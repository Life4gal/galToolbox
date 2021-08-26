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
		template <typename Sequence, typename Func, std::size_t...I, typename... Args>
		constexpr static void binary_invoke_impl(
			Sequence& sequence,
			Func&& func,
			std::index_sequence<I...>,
			Args&&...                args
		)
			noexcept((noexcept(func(std::declval<decltype(sequence[I])>())) && ...))
		{
			(func(sequence[I], args), ...);
		}

		template <typename Sequence, typename Func, std::size_t...I, typename... Args>
		constexpr static bool binary_invoke_r_impl(
			Sequence& sequence,
			Func&& func,
			std::index_sequence<I...>,
			Args&&...                args
		)
			noexcept((noexcept(func(std::declval<decltype(sequence[I])>())) && ...))
		{
			return (func(sequence[I], args) && ...);
		}

		template <typename Sequence1, typename Sequence2, typename Func, std::size_t...I>
		constexpr static void binary_invoke_impl(
			Sequence1& sequence1,
			const Sequence2& sequence2,
			Func&& func,
			std::index_sequence<I...>)
			noexcept((noexcept(func(std::declval<decltype(sequence1[I])>(), std::declval<decltype(sequence2[I])>())) && ...)
				)
		{
			(func(sequence1[I], sequence2[I]), ...);
		}

		template <typename Sequence1, typename Sequence2, typename Func, std::size_t...I>
		constexpr static bool binary_invoke_r_impl(
			Sequence1& sequence1,
			const Sequence2& sequence2,
			Func&& func,
			std::index_sequence<I...>)
			noexcept((noexcept(func(std::declval<decltype(sequence1[I])>(), std::declval<decltype(sequence2[I])>())) && ...)
				)
		{
			return (func(sequence1[I], sequence2[I]) && ...);
		}

		template <typename Sequence, typename Func, std::size_t...I, typename T>
		constexpr static void binary_invoke_impl(
			Sequence& sequence,
			Func&& func,
			std::index_sequence<I...>,
			T                        arg
		)
			noexcept((noexcept(func(std::declval<decltype(sequence[I])>(), std::declval<T>())) && ...))
		{
			(func(sequence[I], arg), ...);
		}

		template <typename Sequence, typename Func, std::size_t...I, typename T>
		constexpr static bool binary_invoke_r_impl(
			Sequence& sequence,
			Func&& func,
			std::index_sequence<I...>,
			T                        arg
		)
			noexcept((noexcept(func(std::declval<decltype(sequence[I])>(), std::declval<T>())) && ...))
		{
			return (func(sequence[I], arg) && ...);
		}

		template <typename Sequence, typename Func, std::size_t...I>
		constexpr static void unary_invoke_impl(
			Sequence& sequence,
			Func&& func,
			std::index_sequence<I...>)
			noexcept((noexcept(func(std::declval<decltype(sequence[I])>())) && ...))
		{
			(func(sequence[I]), ...);
		}

		template <typename Sequence, typename Func, std::size_t...I>
		constexpr static bool unary_invoke_r_impl(
			Sequence& sequence,
			Func&& func,
			std::index_sequence<I...>)
			noexcept((noexcept(func(std::declval<decltype(sequence[I])>())) && ...))
		{
			return (func(sequence[I]) && ...);
		}

	public:
		template <std::size_t N, typename Sequence, typename Func>
		constexpr static void invoke(
			Sequence& sequence,
			Func&& func
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
			sequence_invoker::unary_invoke_impl(sequence, std::forward<Func>(func), std::make_index_sequence<N>{});
		}

		template <std::size_t N, typename Sequence, typename Func>
		constexpr static bool invoke_r(
			Sequence& sequence,
			Func&& func
		)
			noexcept(
				noexcept(
					sequence_invoker::unary_invoke_r_impl(
						std::declval<decltype(sequence)>(),
						std::declval<decltype(func)>(),
						std::declval<std::make_index_sequence<N>>()
					)
					)
				)
		{
			return sequence_invoker::unary_invoke_r_impl(sequence,
				std::forward<Func>(func),
				std::make_index_sequence<N>{});
		}

		template <std::size_t N, typename Sequence, typename Func, typename... Args>
		constexpr static void invoke(
			Sequence& sequence,
			Func&& func,
			Args&&... args
		)
			noexcept(
				noexcept(
					sequence_invoker::binary_invoke_impl(
						std::declval<decltype(sequence)>(),
						std::declval<decltype(func)>(),
						std::declval<std::make_index_sequence<N>>(),
						std::declval<Args>()...
					)
					)
				)
		{
			sequence_invoker::binary_invoke_impl(sequence,
				std::forward<Func>(func),
				std::make_index_sequence<N>{},
				std::forward<Args>(args)...);
		}

		template <std::size_t N, typename Sequence, typename Func, typename... Args>
		constexpr static bool invoke_r(
			Sequence& sequence,
			Func&& func,
			Args&&... args
		)
			noexcept(
				noexcept(
					sequence_invoker::binary_invoke_r_impl(
						std::declval<decltype(sequence)>(),
						std::declval<decltype(func)>(),
						std::declval<std::make_index_sequence<N>>(),
						std::declval<Args>()...
					)
					)
				)
		{
			return sequence_invoker::binary_invoke_r_impl(sequence,
				std::forward<Func>(func),
				std::make_index_sequence<N>{},
				std::forward<Args>(args)...);
		}

		template <std::size_t N, typename Sequence1, typename Sequence2, typename Func>
		constexpr static void invoke_seq(Sequence1& sequence1, Func&& func, const Sequence2& sequence2)
			noexcept(
				noexcept(
					sequence_invoker::binary_invoke_r_impl(
						std::declval<decltype(sequence1)>(),
						std::declval<decltype(sequence2)>(),
						std::declval<decltype(func)>(),
						std::declval<std::make_index_sequence<N>>()
					)
					)
				)
		{
			sequence_invoker::binary_invoke_impl(sequence1,
				sequence2,
				std::forward<Func>(func),
				std::make_index_sequence<N>{});
		}

		template <std::size_t N, typename Sequence1, typename Sequence2, typename Func>
		constexpr static bool invoke_seq_r(Sequence1& sequence1, Func&& func, const Sequence2& sequence2)
			noexcept(
				noexcept(
					sequence_invoker::binary_invoke_r_impl(
						std::declval<decltype(sequence1)>(),
						std::declval<decltype(sequence2)>(),
						std::declval<decltype(func)>(),
						std::declval<std::make_index_sequence<N>>()
					)
					)
				)
		{
			return sequence_invoker::binary_invoke_r_impl(sequence1,
				sequence2,
				std::forward<Func>(func),
				std::make_index_sequence<N>{});
		}

		template <std::size_t N, typename Sequence, typename Func, typename T>
		constexpr static void invoke_dup(Sequence& sequence, Func&& func, T dup_arg)
			noexcept(
				noexcept(
					sequence_invoker::binary_invoke_impl(
						std::declval<decltype(sequence)>(),
						std::declval<decltype(func)>(),
						std::declval<std::make_index_sequence<N>>(),
						std::declval<T>()
					)
					)
				)
		{
			sequence_invoker::binary_invoke_impl(sequence,
				std::forward<Func>(func),
				std::make_index_sequence<N>{},
				dup_arg);
		}

		template <std::size_t N, typename Sequence, typename Func, typename T>
		constexpr static bool invoke_dup_r(Sequence& sequence, Func&& func, T dup_arg)
			noexcept(
				noexcept(
					sequence_invoker::binary_invoke_r_impl(
						std::declval<decltype(sequence)>(),
						std::declval<decltype(func)>(),
						std::declval<std::make_index_sequence<N>>(),
						std::declval<T>()
					)
					)
				)
		{
			return sequence_invoker::binary_invoke_r_impl(sequence,
				std::forward<Func>(func),
				std::make_index_sequence<N>{},
				dup_arg);
		}
	};
}
