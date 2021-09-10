export module gal.toolbox.utils.sequence_invoker;

import <type_traits>;
import <concepts>;

namespace gal::toolbox::utils
{
	export
	{
		/**
		 * @brief a simple auxiliary class for handling some continuous (accessible through operator[]) containers
		 * @note all invokes provide two invocation methods, one is to specify index_sequence by user (requires the same length as N), and the other is to use std::make_index_sequence<N>
		*/
		struct sequence_invoker;
	}

	struct sequence_invoker
	{
	private:
		template <
			typename Sequence1,
			typename Sequence2,
			typename Func,
			std::size_t...I1,
			std::size_t...I2,
			typename ...Args>
			constexpr static void ternary_invoke_impl(
				Sequence1& sequence1,
				const Sequence2& sequence2,
				Func&& func,
				std::index_sequence<I1...>,
				std::index_sequence<I2...>,
				Args&&...                 args
			)
			noexcept((
				noexcept(
					// func(
					// 	std::declval<decltype(sequence2[I2])>(),
					// 	std::declval<decltype(std::forward<Args>(args))>())
					std::invoke(
						std::declval<decltype(std::forward<Func>(func))>(),
						std::declval<decltype(sequence2[I2])>(),
						std::declval<decltype(std::forward<Args>(args))>())
					) && ...
				))
		{
			((sequence1[I1] = std::invoke(std::forward<Func>(func), sequence2[I2], std::forward<Args>(args))), ...);
			// ((sequence1[I1] = func(sequence2[I2], std::forward<Args>(args))), ...);
		}

		template <
			typename Sequence1,
			typename Sequence2,
			typename Sequence3,
			typename Func,
			std::size_t...I1,
			std::size_t...I2,
			std::size_t...I3
		>
			constexpr static void ternary_invoke_seq_impl(
				Sequence1& sequence1,
				const Sequence2& sequence2,
				const Sequence3& sequence3,
				Func&& func,
				std::index_sequence<I1...>,
				std::index_sequence<I2...>,
				std::index_sequence<I3...>)
			noexcept((
				noexcept(
					// func(
					// 	std::declval<decltype(sequence2[I2])>(),
					// 	std::declval<decltype(sequence3[I3])>())
					std::invoke(
						std::declval<decltype(std::forward<Func>(func))>(),
						std::declval<decltype(sequence2[I2])>(),
						std::declval<decltype(sequence3[I3])>()
					)
					) && ...
				))
		{
			((sequence1[I1] = std::invoke(std::forward<Func>(func), sequence2[I2], sequence3[I3])), ...);
			// ((sequence1[I1] = func(sequence2[I2], sequence3[I3])), ...);
		}

		template <
			typename Sequence1,
			typename Sequence2,
			typename T,
			typename Func,
			std::size_t...I1,
			std::size_t...I2
		>
			constexpr static void ternary_invoke_dup_impl(
				Sequence1& sequence1,
				const Sequence2& sequence2,
				T                         arg,
				Func&& func,
				std::index_sequence<I1...>,
				std::index_sequence<I2...>)
			noexcept((
				noexcept(
					// func(
					// 	std::declval<decltype(sequence2[I2])>(),
					// 	std::declval<T>())
					std::invoke(
						std::declval<decltype(std::forward<Func>(func))>(),
						std::declval<decltype(sequence2[I2])>(),
						std::declval<T>())
					) && ...
				))
		{
			((sequence1[I1] = std::invoke(std::forward<Func>(func), sequence2[I2], arg)), ...);
			// ((sequence1[I1] = func(sequence2[I2], arg)), ...);
		}

		template <
			bool HasRet,
			bool All,
			typename Sequence,
			typename Func,
			std::size_t...I,
			typename... Args
		>
			constexpr static auto binary_invoke_impl(
				Sequence& sequence,
				Func&& func,
				std::index_sequence<I...>,
				Args&&...                args
			)
			noexcept((
				noexcept(
					// func(
					// 	std::declval<decltype(sequence[I])>(),
					// 	std::declval<decltype(std::forward<Args>(args))>())
					std::invoke(
						std::declval<decltype(std::forward<Func>(func))>(),
						std::declval<decltype(sequence[I])>(),
						std::declval<decltype(std::forward<Args>(args))>())
					) && ...
				))
		{
			if constexpr (HasRet)
			{
				return [](std::convertible_to<bool> auto ...x)
				{
					if constexpr (All)
					{
						return (x && ...);
					}
					else
					{
						return (x || ...);
					}
				}(std::invoke(std::forward<Func>(func), sequence[I], args)...); // (func(sequence[I], args)...); 
			}
			else
			{
				(std::invoke(std::forward<Func>(func), sequence[I], args), ...);
				// (func(sequence[I], args), ...); 
			}
		}

		template <
			bool HasRet,
			bool All,
			typename Sequence1,
			typename Sequence2,
			typename Func,
			std::size_t...I1,
			std::size_t...I2
		>
			constexpr static auto binary_invoke_seq_impl(
				Sequence1& sequence1,
				const Sequence2& sequence2,
				Func&& func,
				std::index_sequence<I1...>,
				std::index_sequence<I2...>)
			noexcept((
				noexcept(
					// func(
					// 	std::declval<decltype(sequence1[I1])>(),
					// 	std::declval<decltype(sequence2[I2])>())
					std::invoke(
						std::declval<decltype(std::forward<Func>(func))>(),
						std::declval<decltype(sequence1[I1])>(),
						std::declval<decltype(sequence2[I2])>())
					) && ...
				))
		{
			if constexpr (HasRet)
			{
				return [](std::convertible_to<bool> auto ...x)
				{
					if constexpr (All)
					{
						return (x && ...);
					}
					else
					{
						return (x || ...);
					}
				}(std::invoke(std::forward<Func>(func), sequence1[I1], sequence2[I2])...);
				// (func(sequence1[I1], sequence2[I2])...);
			}
			else
			{
				(std::invoke(std::forward<Func>(func), sequence1[I1], sequence2[I2]), ...);
				// (func(sequence1[I1], sequence2[I2]), ...);
			}
		}

		template <
			bool HasRet,
			bool All,
			typename Sequence,
			typename T,
			typename Func,
			std::size_t...I
		>
			constexpr static auto binary_invoke_dup_impl(
				Sequence& sequence,
				T                        arg,
				Func&& func,
				std::index_sequence<I...>)
			noexcept((
				noexcept(
					// func(
					// 	std::declval<decltype(sequence[I])>(),
					// 	std::declval<T>())
					std::invoke(
						std::declval<decltype(std::forward<Func>(func))>(),
						std::declval<decltype(sequence[I])>(),
						std::declval<T>())
					) && ...
				))
		{
			if constexpr (HasRet)
			{
				return [](std::convertible_to<bool> auto ...x)
				{
					if constexpr (All)
					{
						return (x && ...);
					}
					else
					{
						return (x || ...);
					}
				}(std::invoke(std::forward<Func>(func), sequence[I], arg)...); // (func(sequence[I], arg)...);
			}
			else
			{
				(std::invoke(std::forward<Func>(func), sequence[I], arg), ...);
				// (func(sequence[I], arg), ...);
			}
		}

		template <
			bool HasRet,
			bool All,
			typename Sequence,
			typename Func,
			std::size_t...I
		>
			constexpr static auto unary_invoke_impl(
				Sequence& sequence,
				Func&& func,
				std::index_sequence<I...>)
			noexcept((
				noexcept(
					// func(std::declval<decltype(sequence[I])>())
					std::invoke(
						std::forward<Func>(func),
						std::declval<decltype(sequence[I])>())
					) && ...
				))
		{
			if constexpr (HasRet)
			{
				return [](std::convertible_to<bool> auto ...x)
				{
					if constexpr (All)
					{
						return (x && ...);
					}
					else
					{
						return (x || ...);
					}
				}(std::invoke(std::forward<Func>(func), sequence[I])...); // (func(sequence[I])...);
			}
			else
			{
				(std::invoke(std::forward<Func>(func), sequence[I]), ...);
				// (func(sequence[I]), ...);
			}
		}

	public:
		/**
		 * @brief process a sequence with a sequence and a parameters pack
		 * @tparam N sequence length
		 * @tparam Sequence1 sequence1 type
		 * @tparam Sequence2 sequence2 type
		 * @tparam Func processing function type, it is required to accept two parameters (respectively the value_type of sequenc2 and parameters pack)
		 * @tparam I1 user-defined index_sequence instead of using std::make_index_sequence
		 * @tparam I2 user-defined index_sequence instead of using std::make_index_sequence
		 * @tparam Args parameters pack type
		 * @param sequence1 pending sequence1
		 * @param sequence2 auxiliary processing sequence2
		 * @param func processing function
		 * @param  args auxiliary processing args
		*/
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
				Sequence1& sequence1,
				const Sequence2& sequence2,
				Func&& func,
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

		/**
		 * @brief process a sequence with a sequence and a parameters pack
		 * @tparam N sequence length
		 * @tparam Sequence1 sequence1 type
		 * @tparam Sequence2 sequence2 type
		 * @tparam Func processing function type, it is required to accept two parameters (respectively the value_type of sequenc2 and parameters pack)
		 * @tparam Args parameters pack type
		 * @param sequence1 pending sequence1
		 * @param sequence2 auxiliary processing sequence2
		 * @param func processing function
		 * @param  args auxiliary processing args
		*/
		template <
			std::size_t N,
			typename Sequence1,
			typename Sequence2,
			typename Func,
			typename... Args
		>
			requires (sizeof...(Args) == N)
			constexpr static void ternary_invoke(
				Sequence1& sequence1,
				const Sequence2& sequence2,
				Func&& func,
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

		/**
		 * @brief process a sequence with two sequence
		 * @tparam N sequence length
		 * @tparam Sequence1 sequence1 type
		 * @tparam Sequence2 sequence2 type
		 * @tparam Sequence3 sequence3 type
		 * @tparam Func processing function type, it is required to accept two parameters (respectively the value_type of sequenc2 and sequence3)
		 * @tparam I1 user-defined index_sequence instead of using std::make_index_sequence
		 * @tparam I2 user-defined index_sequence instead of using std::make_index_sequence
		 * @tparam I3 user-defined index_sequence instead of using std::make_index_sequence
		 * @param sequence1 pending sequence1
		 * @param sequence2 auxiliary processing sequence2
		 * @param sequence3 auxiliary processing sequence3
		 * @param func processing function
		*/
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
				Sequence1& sequence1,
				const Sequence2& sequence2,
				const Sequence3& sequence3,
				Func&& func,
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

		/**
		 * @brief process a sequence with two sequence
		 * @tparam N sequence length
		 * @tparam Sequence1 sequence1 type
		 * @tparam Sequence2 sequence2 type
		 * @tparam Sequence3 sequence3 type
		 * @tparam Func processing function type, it is required to accept two parameters (respectively the value_type of sequenc2 and sequence3)
		 * @param sequence1 pending sequence1
		 * @param sequence2 auxiliary processing sequence2
		 * @param sequence3 auxiliary processing sequence3
		 * @param func processing function
		*/
		template <
			std::size_t N,
			typename Sequence1,
			typename Sequence2,
			typename Sequence3,
			typename Func
		>
			constexpr static void ternary_invoke_seq(
				Sequence1& sequence1,
				const Sequence2& sequence2,
				const Sequence3& sequence3,
				Func&& func
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

		/**
		 * @brief process a sequence with two sequence
		 * @tparam N sequence length
		 * @tparam Sequence1 sequence1 type
		 * @tparam Sequence2 sequence2 type
		 * @tparam T duplicate value type
		 * @tparam Func processing function type, it is required to accept two parameters (respectively the value_type of sequenc2 and T)
		 * @tparam I1 user-defined index_sequence instead of using std::make_index_sequence
		 * @tparam I2 user-defined index_sequence instead of using std::make_index_sequence
		 * @param sequence1 pending sequence1
		 * @param sequence2 auxiliary processing sequence2
		 * @param dup_arg arg for duplicate
		 * @param func processing function
		*/
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
				Sequence1& sequence1,
				const Sequence2& sequence2,
				T                         dup_arg,
				Func&& func,
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

		/**
		 * @brief process a sequence with a sequence and a value
		 * @tparam N sequence length
		 * @tparam Sequence1 sequence1 type
		 * @tparam Sequence2 sequence2 type
		 * @tparam T duplicate value type
		 * @tparam Func processing function type, it is required to accept two parameters (respectively the value_type of sequenc2 and T)
		 * @param sequence1 pending sequence1
		 * @param sequence2 auxiliary processing sequence2
		 * @param dup_arg arg for duplicate
		 * @param func processing function
		*/
		template <
			std::size_t N,
			typename Sequence1,
			typename Sequence2,
			typename T,
			typename Func
		>
			constexpr static void ternary_invoke_dup(
				Sequence1& sequence1,
				const Sequence2& sequence2,
				T                dup_arg,
				Func&& func
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

		/**
		 * @brief process a sequence with a parameters pack
		 * @tparam N sequence length
		 * @tparam HasRet does the function require a return value
		 * @tparam All the superposition method of the return value (bool), is it All(&&) or Any(||)
		 * @tparam Sequence sequence type
		 * @tparam Func processing function type, it is required to accept two parameters (respectively the value_type of sequence and parameters pack)
		 * @tparam I user-defined index_sequence instead of using std::make_index_sequence
		 * @tparam Args parameters pack type
		 * @param sequence pending sequence
		 * @param func processing function
		 * @param  args auxiliary processing args
		*/
		template <
			std::size_t N,
			bool HasRet = false,
			bool All = true,
			typename Sequence,
			typename Func,
			std::size_t ...I,
			typename... Args
		>
			requires (sizeof...(I) == N) && (sizeof...(Args) == N)
			constexpr static auto binary_invoke(
				Sequence& sequence,
				Func&& func,
				std::index_sequence<I...>,
				Args&&...                args
			)
			noexcept(
				noexcept(
					sequence_invoker::binary_invoke_impl<HasRet, All>(
						std::declval<decltype(sequence)>(),
						std::declval<decltype(func)>(),
						std::declval<std::index_sequence<I...>>(),
						std::declval<decltype(std::forward<Args>(args))>()...
						)
					)
				)
		{
			return sequence_invoker::binary_invoke_impl<HasRet, All>(
				sequence,
				std::forward<Func>(func),
				std::index_sequence<I...>{},
				std::forward<Args>(args)...
				);
		}

		/**
		 * @brief process a sequence with a parameters pack
		 * @tparam N sequence length
		 * @tparam HasRet does the function require a return value
		 * @tparam All the superposition method of the return value (bool), is it All(&&) or Any(||)
		 * @tparam Sequence sequence type
		 * @tparam Func processing function type, it is required to accept two parameters (respectively the value_type of sequence and parameters pack)
		 * @tparam Args parameters pack type
		 * @param sequence pending sequence
		 * @param func processing function
		 * @param  args auxiliary processing args
		*/
		template <
			std::size_t N,
			bool HasRet = false,
			bool All = true,
			typename Sequence,
			typename Func,
			typename... Args
		>
			requires (sizeof...(Args) == N)
			constexpr static auto binary_invoke(
				Sequence& sequence,
				Func&& func,
				Args&&... args
			)
			noexcept(
				noexcept(
					sequence_invoker::binary_invoke<N, HasRet, All>(
						std::declval<decltype(sequence)>(),
						std::declval<decltype(func)>(),
						std::declval<std::make_index_sequence<N>>(),
						std::declval<decltype(std::forward<Args>(args))>()...
						)
					)
				)
		{
			return sequence_invoker::binary_invoke<N, HasRet, All>(
				sequence,
				std::forward<Func>(func),
				std::make_index_sequence<N>{},
				std::forward<Args>(args)...
				);
		}

		/**
		 * @brief process a sequence with a sequence
		 * @tparam N sequence length
		 * @tparam HasRet does the function require a return value
		 * @tparam All the superposition method of the return value (bool), is it All(&&) or Any(||)
		 * @tparam Sequence1 sequence1 type
		 * @tparam Sequence2 sequence2 type
		 * @tparam Func processing function type, it is required to accept two parameters (respectively the value_type of sequence1 and sequence2)
		 * @tparam I1 user-defined index_sequence instead of using std::make_index_sequence
		 * @tparam I2 user-defined index_sequence instead of using std::make_index_sequence
		 * @param sequence1 pending sequence1
		 * @param sequence2 auxiliary processing sequence2
		 * @param func processing function
		*/
		template <
			std::size_t N,
			bool HasRet = false,
			bool All = true,
			typename Sequence1,
			typename Sequence2,
			typename Func,
			std::size_t...I1,
			std::size_t...I2
		>
			requires (sizeof...(I1) == N) && (sizeof...(I2) == N)
			constexpr static auto binary_invoke_seq(
				Sequence1& sequence1,
				const Sequence2& sequence2,
				Func&& func,
				std::index_sequence<I1...>,
				std::index_sequence<I2...>)
			noexcept(
				noexcept(
					sequence_invoker::binary_invoke_seq_impl<HasRet, All>(
						std::declval<decltype(sequence1)>(),
						std::declval<decltype(sequence2)>(),
						std::declval<decltype(func)>(),
						std::declval<std::index_sequence<I1...>>(),
						std::declval<std::index_sequence<I2...>>()
						)
					)
				)
		{
			return sequence_invoker::binary_invoke_seq_impl<HasRet, All>(
				sequence1,
				sequence2,
				std::forward<Func>(func),
				std::index_sequence<I1...>{},
				std::index_sequence<I1...>{}
			);
		}

		/**
		 * @brief process a sequence with a sequence
		 * @tparam N sequence length
		 * @tparam HasRet does the function require a return value
		 * @tparam All the superposition method of the return value (bool), is it All(&&) or Any(||)
		 * @tparam Sequence1 sequence1 type
		 * @tparam Sequence2 sequence2 type
		 * @tparam Func processing function type, it is required to accept two parameters (respectively the value_type of sequence1 and sequence2)
		 * @param sequence1 pending sequence1
		 * @param sequence2 auxiliary processing sequence2
		 * @param func processing function
		*/
		template <
			std::size_t N,
			bool HasRet = false,
			bool All = true,
			typename Sequence1,
			typename Sequence2,
			typename Func
		>
			constexpr static auto binary_invoke_seq(
				Sequence1& sequence1,
				const Sequence2& sequence2,
				Func&& func
			)
			noexcept(
				noexcept(
					sequence_invoker::binary_invoke_seq<N, HasRet, All>(
						std::declval<decltype(sequence1)>(),
						std::declval<decltype(sequence2)>(),
						std::declval<decltype(func)>(),
						std::declval<std::make_index_sequence<N>>(),
						std::declval<std::make_index_sequence<N>>()
						)
					)
				)
		{
			return sequence_invoker::binary_invoke_seq<N, HasRet, All>(
				sequence1,
				sequence2,
				std::forward<Func>(func),
				std::make_index_sequence<N>{},
				std::make_index_sequence<N>{}
			);
		}

		/**
		 * @brief process a sequence with a value
		 * @tparam N sequence length
		 * @tparam HasRet does the function require a return value
		 * @tparam All the superposition method of the return value (bool), is it All(&&) or Any(||)
		 * @tparam Sequence sequence type
		 * @tparam T duplicate value type
		 * @tparam Func processing function type, it is required to accept two parameters (respectively the value_type of sequence1 and T)
		 * @tparam I user-defined index_sequence instead of using std::make_index_sequence
		 * @param sequence pending sequence1
		 * @param dup_arg arg for duplicate
		 * @param func processing function
		*/
		template <
			std::size_t N,
			bool HasRet = false,
			bool All = true,
			typename Sequence,
			typename T,
			typename Func,
			std::size_t...I
		>
			requires (sizeof...(I) == N)
			constexpr static auto binary_invoke_dup(
				Sequence& sequence,
				T                        dup_arg,
				Func&& func,
				std::index_sequence<I...>)
			noexcept(
				noexcept(
					sequence_invoker::binary_invoke_dup_impl<HasRet, All>(
						std::declval<decltype(sequence)>(),
						std::declval<T>(),
						std::declval<decltype(func)>(),
						std::declval<std::index_sequence<I...>>()
						)
					)
				)
		{
			return sequence_invoker::binary_invoke_dup_impl<HasRet, All>(
				sequence,
				dup_arg,
				std::forward<Func>(func),
				std::index_sequence<I...>{}
			);
		}

		/**
		 * @brief process a sequence with a sequence
		 * @tparam N sequence length
		 * @tparam HasRet does the function require a return value
		 * @tparam All the superposition method of the return value (bool), is it All(&&) or Any(||)
		 * @tparam Sequence sequence type
		 * @tparam T duplicate value type
		 * @tparam Func processing function type, it is required to accept two parameters (respectively the value_type of sequence1 and T)
		 * @param sequence pending sequence1
		 * @param dup_arg arg for duplicate
		 * @param func processing function
		*/
		template <
			std::size_t N,
			bool HasRet = false,
			bool All = true,
			typename Sequence,
			typename T,
			typename Func
		>
			constexpr static auto binary_invoke_dup(
				Sequence& sequence,
				T         dup_arg,
				Func&& func
			)
			noexcept(
				noexcept(
					sequence_invoker::binary_invoke_dup<N, HasRet, All>(
						std::declval<decltype(sequence)>(),
						std::declval<T>(),
						std::declval<decltype(func)>(),
						std::declval<std::make_index_sequence<N>>()
						)
					)
				)
		{
			return sequence_invoker::binary_invoke_dup<N, HasRet, All>(
				sequence,
				dup_arg,
				std::forward<Func>(func),
				std::make_index_sequence<N>{}
			);
		}

		/**
		 * @brief process a sequence
		 * @tparam N sequence length
		 * @tparam HasRet does the function require a return value
		 * @tparam All the superposition method of the return value (bool), is it All(&&) or Any(||)
		 * @tparam Sequence sequence type
		 * @tparam Func processing function type, it is required to accept two parameters (respectively the value_type of sequence)
		 * @tparam I user-defined index_sequence instead of using std::make_index_sequence
		 * @param sequence pending sequence
		 * @param func processing function
		*/
		template <
			std::size_t N,
			bool HasRet = false,
			bool All = true,
			typename Sequence,
			typename Func,
			std::size_t...I
		>
			requires (sizeof...(I) == N)
			constexpr static auto unary_invoke(
				Sequence& sequence,
				Func&& func,
				std::index_sequence<I...>)
			noexcept(
				noexcept(
					sequence_invoker::unary_invoke_impl<HasRet, All>(
						std::declval<decltype(sequence)>(),
						std::declval<decltype(func)>(),
						std::declval<std::index_sequence<I...>>()
						)
					)
				)
		{
			return sequence_invoker::unary_invoke_impl<HasRet, All>(
				sequence,
				std::forward<Func>(func),
				std::index_sequence<I...>{}
			);
		}

		/**
		 * @brief process a sequence
		 * @tparam N sequence length
		 * @tparam HasRet does the function require a return value
		 * @tparam All the superposition method of the return value (bool), is it All(&&) or Any(||)
		 * @tparam Sequence sequence type
		 * @tparam Func processing function type, it is required to accept two parameters (respectively the value_type of sequence)
		 * @param sequence pending sequence
		 * @param func processing function
		*/
		template <
			std::size_t N,
			bool HasRet = false,
			bool All = true,
			typename Sequence,
			typename Func
		>
			constexpr static auto unary_invoke(
				Sequence& sequence,
				Func&& func
			)
			noexcept(
				noexcept(
					sequence_invoker::unary_invoke<N, HasRet, All>(
						std::declval<decltype(sequence)>(),
						std::declval<decltype(func)>(),
						std::declval<std::make_index_sequence<N>>()
						)
					)
				)
		{
			return sequence_invoker::unary_invoke<N, HasRet, All>(
				sequence,
				std::forward<Func>(func),
				std::make_index_sequence<N>{}
			);
		}
	};
}