export module gal.toolbox.utils.tuple_maker;

import <tuple>;

namespace gal::toolbox::utils
{
	export
	{
		/**
		 * @brief specialize tuple_maker_trait for tuple_maker support
		 * @note to_tuple will "expand" the target and get the element of the former "size" according to "size" through operator[]
		 * @tparam T specialized type
		*/
		template<typename T>
		struct tuple_maker_trait : std::false_type
		{
			using value_type = T;
			constexpr static std::size_t size = 1;
		};

		struct tuple_maker;
	}

	struct tuple_maker
	{
		/**
		 * @brief extract all elements in a container and put them into a tuple
		 * @tparam T arg type
		 * @param arg arg
		 * @return tuple(rvalue reference) containing all data
		*/
		template <typename T>
		constexpr static auto to_rref_tuple(T&& arg) noexcept
		{
			using trait = tuple_maker_trait<std::remove_cvref_t<T>>;
			if constexpr (trait::value)
			{
				return[&]<std::size_t...I>(std::index_sequence<I...>)
				{
					return std::forward_as_tuple(std::forward<T>(arg)[I]...);
				}(std::make_index_sequence<trait::size>{});
			}
			else
			{
				return std::forward_as_tuple(std::forward<T>(arg));
			}
		}

		/**
		 * @brief extract all elements in a container and put them into a tuple
		 * @tparam T arg type
		 * @param arg arg
		 * @return tuple containing all data
		*/
		template <typename T>
		constexpr static auto to_tuple(const T& arg) noexcept
		{
			using trait = tuple_maker_trait<std::remove_cvref_t<T>>;
			if constexpr (trait::value)
			{
				return[&]<std::size_t...I>(std::index_sequence<I...>)
				{
					return std::make_tuple(arg[I]...);
				}(std::make_index_sequence<trait::size>{});
			}
			else
			{
				return std::make_tuple(arg);
			}
		}

		/**
		 * @brief get N elements in a continuous range starting from begin
		 * @tparam N the number of elements
		 * @tparam Iterator iterator type
		 * @param begin iterator begin
		 * @param stride stride
		 * @return tuple containing all data
		*/
		template <std::size_t N, typename Iterator>
		constexpr static auto to_tuple(Iterator begin, std::size_t stride = 1)
		{
			return[&]<std::size_t...I>(std::index_sequence<I...>)
			{
				return std::tuple_cat(to_tuple(*std::next(begin, stride * I))...);
			}(std::make_index_sequence<N>{});
		}

		/**
		 * @brief construction assistance: can be used when trying to assign all values of the entire container with one parameter
		 * @tparam T arg type
		 * @param arg arg
		 * @return tuple containing repeated parameters
		*/
		template <std::size_t N, typename T>
		constexpr static auto duplicate(const T& arg) noexcept
		{
			if constexpr (N == 0)
			{
				return std::make_tuple();
			}
			else
			{
				using trait = tuple_maker_trait<std::remove_cvref_t<T>>;
				if constexpr (trait::value)
				{
					if constexpr (trait::size >= N)
					{
						return to_tuple(arg);
					}
					else
					{
						return std::tuple_cat(to_tuple(arg), duplicate<N - trait::size>(arg));
					}
				}
				else
				{
					return std::tuple_cat(to_tuple(arg), duplicate<N - 1>(arg));
				}
			}
		}
	};
}