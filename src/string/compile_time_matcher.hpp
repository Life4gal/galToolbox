#pragma once
#include <type_traits>
#include <iterator>
#include <stdexcept>

namespace gal::test
{
	class matcher;
	
	// https://en.cppreference.com/w/cpp/language/string_literal
	template<typename T>
	struct wildcard;

	enum class wildcard_type
	{
		standard,
		extended
	};

	template<>
	struct wildcard<char>
	{
		// default
		explicit constexpr wildcard(const wildcard_type type = wildcard_type::extended)
			: set_enabled_(type == wildcard_type::extended),
		alt_enabled_(type == wildcard_type::extended) {}

		// standard
		constexpr wildcard(const char anything, const char single, const char escape)
			: anything_(anything),
		single_(single),
		escape_(escape){}

		// extended
		constexpr wildcard(
			const char anything,
			const char single,
			const char escape,
			const char set_open,
			const char set_close,
			const char set_not,
			const char alt_open,
			const char alt_close,
			const char alt_or
		)
			: anything_(anything),
		single_(single),
		escape_(escape),
		set_open_(set_open),
		set_close_(set_close),
		set_not_(set_not),
		alt_open_(alt_open),
		alt_close_(alt_close),
		alt_or_(alt_or) {}
		
		// standard
		char anything_{ '*' };
		char single_{ '?' };
		char escape_{ '\\' };

		// extended
		bool set_enabled_{ true };
		char set_open_{ '[' };
		char set_close_{ ']' };
		char set_not_{ '!' };

		bool alt_enabled_{ true };
		char alt_open_{ '(' };
		char alt_close_{ ')' };
		char alt_or_{ '|' };
	};

	template<>
	struct wildcard<wchar_t>
	{
		// default
		explicit constexpr wildcard(const wildcard_type type = wildcard_type::extended)
			: set_enabled_(type == wildcard_type::extended),
			alt_enabled_(type == wildcard_type::extended) {}

		// standard
		constexpr wildcard(const wchar_t anything, const wchar_t single, const wchar_t escape)
			: anything_(anything),
			single_(single),
			escape_(escape) {}

		// extended
		constexpr wildcard(
			const wchar_t anything,
			const wchar_t single,
			const wchar_t escape,
			const wchar_t set_open,
			const wchar_t set_close,
			const wchar_t set_not,
			const wchar_t alt_open,
			const wchar_t alt_close,
			const wchar_t alt_or
		)
			: anything_(anything),
			single_(single),
			escape_(escape),
			set_open_(set_open),
			set_close_(set_close),
			set_not_(set_not),
			alt_open_(alt_open),
			alt_close_(alt_close),
			alt_or_(alt_or) {}

		// standard
		wchar_t anything_{ L'*' };
		wchar_t single_{ L'?' };
		wchar_t escape_{ L'\\' };

		// extended
		bool set_enabled_{ true };
		wchar_t set_open_{ L'[' };
		wchar_t set_close_{ L']' };
		wchar_t set_not_{ L'!' };

		bool alt_enabled_{ true };
		wchar_t alt_open_{ L'(' };
		wchar_t alt_close_{ L')' };
		wchar_t alt_or_{ L'|' };
	};

	template<>
	struct wildcard<char8_t>
	{
		// default
		explicit constexpr wildcard(const wildcard_type type = wildcard_type::extended)
			: set_enabled_(type == wildcard_type::extended),
			alt_enabled_(type == wildcard_type::extended) {}

		// standard
		constexpr wildcard(const char8_t anything, const char8_t single, const char8_t escape)
			: anything_(anything),
			single_(single),
			escape_(escape) {}

		// extended
		constexpr wildcard(
			const char8_t anything,
			const char8_t single,
			const char8_t escape,
			const char8_t set_open,
			const char8_t set_close,
			const char8_t set_not,
			const char8_t alt_open,
			const char8_t alt_close,
			const char8_t alt_or
		)
			: anything_(anything),
			single_(single),
			escape_(escape),
			set_open_(set_open),
			set_close_(set_close),
			set_not_(set_not),
			alt_open_(alt_open),
			alt_close_(alt_close),
			alt_or_(alt_or) {}

		// standard
		char8_t anything_{ u8'*' };
		char8_t single_{ u8'?' };
		char8_t escape_{ u8'\\' };

		// extended
		bool set_enabled_{ true };
		char8_t set_open_{ u8'[' };
		char8_t set_close_{ u8']' };
		char8_t set_not_{ u8'!' };

		bool alt_enabled_{ true };
		char8_t alt_open_{ u8'(' };
		char8_t alt_close_{ u8')' };
		char8_t alt_or_{ u8'|' };
	};

	template<>
	struct wildcard<char16_t>
	{
		// default
		explicit constexpr wildcard(const wildcard_type type = wildcard_type::extended)
			: set_enabled_(type == wildcard_type::extended),
			alt_enabled_(type == wildcard_type::extended) {}

		// standard
		constexpr wildcard(const char16_t anything, const char16_t single, const char16_t escape)
			: anything_(anything),
			single_(single),
			escape_(escape) {}

		// extended
		constexpr wildcard(
			const char16_t anything,
			const char16_t single,
			const char16_t escape,
			const char16_t set_open,
			const char16_t set_close,
			const char16_t set_not,
			const char16_t alt_open,
			const char16_t alt_close,
			const char16_t alt_or
		)
			: anything_(anything),
			single_(single),
			escape_(escape),
			set_open_(set_open),
			set_close_(set_close),
			set_not_(set_not),
			alt_open_(alt_open),
			alt_close_(alt_close),
			alt_or_(alt_or) {}

		// standard
		char16_t anything_{ u'*' };
		char16_t single_{ u'?' };
		char16_t escape_{ u'\\' };

		// extended
		bool set_enabled_{ true };
		char16_t set_open_{ u'[' };
		char16_t set_close_{ u']' };
		char16_t set_not_{ u'!' };

		bool alt_enabled_{ true };
		char16_t alt_open_{ u'(' };
		char16_t alt_close_{ u')' };
		char16_t alt_or_{ u'|' };
	};

	template<>
	struct wildcard<char32_t>
	{
		// default
		explicit constexpr wildcard(const wildcard_type type = wildcard_type::extended)
			: set_enabled_(type == wildcard_type::extended),
			alt_enabled_(type == wildcard_type::extended) {}

		// standard
		constexpr wildcard(const char32_t anything, const char32_t single, const char32_t escape)
			: anything_(anything),
			single_(single),
			escape_(escape) {}

		// extended
		constexpr wildcard(
			const char32_t anything,
			const char32_t single,
			const char32_t escape,
			const char32_t set_open,
			const char32_t set_close,
			const char32_t set_not,
			const char32_t alt_open,
			const char32_t alt_close,
			const char32_t alt_or
		)
			: anything_(anything),
			single_(single),
			escape_(escape),
			set_open_(set_open),
			set_close_(set_close),
			set_not_(set_not),
			alt_open_(alt_open),
			alt_close_(alt_close),
			alt_or_(alt_or) {}

		// standard
		char32_t anything_{ U'*' };
		char32_t single_{ U'?' };
		char32_t escape_{ U'\\' };

		// extended
		bool set_enabled_{ true };
		char32_t set_open_{ U'[' };
		char32_t set_close_{ U']' };
		char32_t set_not_{ U'!' };

		bool alt_enabled_{ true };
		char32_t alt_open_{ U'(' };
		char32_t alt_close_{ U')' };
		char32_t alt_or_{ U'|' };
	};

	template<typename SequenceIterator, typename PatternIterator>
	struct full_match_result
	{
		bool result;
		[[no_unique_address]] SequenceIterator sequence_begin, sequence_end, match_result_sequence;
		[[no_unique_address]] PatternIterator pattern_begin, pattern_end, match_result_pattern;

		// ReSharper disable once CppNonExplicitConversionOperator
		constexpr operator bool() const noexcept
		{
			return result;
		}
	};

	template<typename SequenceIterator, typename PatternIterator>
	struct match_result
	{
		bool result;
		[[no_unique_address]] SequenceIterator sequence;
		[[no_unique_address]] PatternIterator pattern;

		// ReSharper disable once CppNonExplicitConversionOperator
		constexpr operator bool() const noexcept
		{
			return result;
		}
	};

	template<typename SequenceIterator, typename PatternIterator>
	constexpr full_match_result<SequenceIterator, PatternIterator> make_full_result(
		SequenceIterator sequence_begin, SequenceIterator sequence_end,
		PatternIterator pattern_begin, PatternIterator pattern_end,
		match_result<SequenceIterator, PatternIterator> match_result
	)
	{
		return {
			match_result.result,
			sequence_begin, sequence_end, match_result.sequence,
			pattern_begin, pattern_end, match_result.pattern
		};
	}

	template<typename ErrorType, typename T>
	constexpr auto check_string(const char* args, T t = {})
	{
		if constexpr (std::is_same_v<T, bool>)
		{
			return args == nullptr ? false : throw ErrorType(args);
		}
		else
		{
			return args == nullptr ? t : throw ErrorType(args);
		}
	}

	template<typename ErrorType, typename T>
	constexpr auto emit_or_dumb(const char* message, T t = {})
	{
		return message == nullptr ? t : throw ErrorType(message);
	}

	enum class match_set_state
	{
		open,
		not_or_first,
		first,
		next
	};

	/**
	 * @brief check set
	 * @tparam EmitError if true, throw a std::invalid_argument instead return end
	 * @tparam PatternIterator iterator type
	 * @param begin pattern begin
	 * @param end pattern end
	 * @param wild wildcard
	 * @param state state
	 * @return if the check is passed, it returns the last matched position and iterates one position backward, otherwise it returns end
	*/
	template<bool EmitError, typename PatternIterator>
	constexpr PatternIterator check_set(
		PatternIterator begin,
		PatternIterator end,
		const  wildcard<std::remove_cv_t<std::remove_reference_t<decltype(*std::declval<PatternIterator>())>>>bitand wild = wildcard<std::remove_cv_t<std::remove_reference_t<decltype(*std::declval<PatternIterator>())>>>{},
		match_set_state state = match_set_state::open
	)
	{
		if (not wild.set_enabled_)
		{
			if constexpr (EmitError)
			{
				throw std::invalid_argument("the use of sets is disabled");
			}
			else
			{
				return end;
			}
		}

		while (begin not_eq end)
		{
			switch (state)
			{
			case match_set_state::open:
			{
				if (*begin not_eq wild.set_open_)
				{
					if constexpr (EmitError)
					{
						throw std::invalid_argument("the given pattern is not a valid set");
					}
					else
					{
						return end;
					}
				}
				state = match_set_state::not_or_first;
				break;
			}
			case match_set_state::not_or_first:
			{
				if (*begin == wild.set_not_)
				{
					state = match_set_state::first;
				}
				else
				{
					state = match_set_state::next;
				}
				break;
			}
			case match_set_state::first:
			{
				state = match_set_state::next;
				break;
			}
			case match_set_state::next:
			{
				if (*begin == wild.set_close_)
				{
					return std::next(begin);
				}
			}
			}

			std::advance(begin, 1);
		}

		if constexpr (EmitError)
		{
			throw std::invalid_argument("the given pattern is not a valid set");
		}
		else
		{
			return end;
		}
	}

	enum class match_state
	{
		open,
		not_or_first_in,
		first_out,
		next_in,
		next_out
	};

	template<typename SequenceIterator, typename PatternIterator, typename EqualTo = std::equal_to<>>
	constexpr match_result<SequenceIterator, PatternIterator> match_set(
		SequenceIterator sequence_begin, SequenceIterator sequence_end,
		PatternIterator pattern_begin, PatternIterator pattern_end,
		const  wildcard<std::remove_cv_t<std::remove_reference_t<decltype(*std::declval<PatternIterator>())>>>bitand wild = wildcard<std::remove_cv_t<std::remove_reference_t<decltype(*std::declval<PatternIterator>())>>>{},
		const EqualTo bitand equal_to = {},
		match_state state = match_state::open
	)
	{
		if(not wild.set_enabled_)
		{
			throw std::invalid_argument("the use of sets is disabled");
		}

		while(pattern_begin not_eq pattern_end)
		{
			switch (state)
			{
			case match_state::open:
			{
				if(*pattern_begin not_eq wild.set_open_)
				{
					throw std::invalid_argument("the given pattern is not a valid set");
				}
				state = match_state::not_or_first_in;
				break;
			}
			case match_state::not_or_first_in:
			{
				if(*pattern_begin == wild.set_not_)
				{
					state = match_state::first_out;
				}
				else
				{
					if(sequence_begin == sequence_end)
					{
						return {
							false,
							sequence_begin,
							pattern_begin
						};
					}
					if(equal_to(*sequence_begin, *pattern_begin))
					{
						return {
							true,
							sequence_begin,
							pattern_begin
						};
					}

					state = match_state::next_in;
				}
				break;
			}
			case match_state::first_out:
			{
				if(sequence_begin == sequence_end or equal_to(*sequence_begin, *pattern_begin))
				{
					return {
							false,
							sequence_begin,
							pattern_begin
					};
				}

				state = match_state::next_in;
				break;
			}
			case match_state::next_in:
			{
				if(*pattern_begin == wild.set_close_ or sequence_begin == sequence_end)
				{
					return {
							false,
							sequence_begin,
							pattern_begin
					};
				}
				if (equal_to(*sequence_begin, *pattern_begin))
				{
					return {
						true,
						sequence_begin,
						pattern_begin
					};
				}
				break;
			}
			case match_state::next_out:
			{
				if(*pattern_begin == wild.set_close_)
				{
					return {
						true,
						sequence_begin,
						pattern_begin
					};
				}
				if (sequence_begin == sequence_end or equal_to(*sequence_begin, *pattern_begin))
				{
					return {
							false,
							sequence_begin,
							pattern_begin
					};
				}
				break;
			}
			}

			std::advance(pattern_begin, 1);
		}

		throw std::invalid_argument("the given pattern is not a valid set");
	}

	enum class match_alt_state
	{
		open,
		next,
		escape
	};

	template<bool EmitError, typename PatternIterator>
	constexpr PatternIterator check_alt(
		PatternIterator begin,
		PatternIterator end,
		const  wildcard<std::remove_cv_t<std::remove_reference_t<decltype(*std::declval<PatternIterator>())>>>bitand wild = wildcard<std::remove_cv_t<std::remove_reference_t<decltype(*std::declval<PatternIterator>())>>>{},
		match_alt_state state = match_alt_state::open,
		int depth = 0
	)
	{
		if (not wild.alt_enabled_)
		{
			if constexpr (EmitError)
			{
				throw std::invalid_argument("the use of alternatives is disabled");
			}
			else
			{
				return end;
			}
		}

		while(begin not_eq end)
		{
			switch (state)
			{
			case match_alt_state::open:
			{
				if(*begin not_eq wild.alt_open_)
				{
					if constexpr (EmitError)
					{
						throw std::invalid_argument("the given pattern is not a valid alternative");
					}
					else
					{
						return end;
					}
				}

				state = match_alt_state::next;
				++depth;
				break;
			}
			case match_alt_state::next:
			{
				if(*begin == wild.escape_)
				{
					state = match_alt_state::escape;
				}
				else if(
					wild.set_enabled_ and 
					*begin == wild.set_open_ and 
					check_set<false>(std::next(begin), end, wild, match_set_state::not_or_first) not_eq end
					)
				{
					begin = std::prev(check_set<true>(std::next(begin), end, wild, match_set_state::not_or_first));
				}
				else if(*begin == wild.alt_open_)
				{
					++depth;
				}else if(*begin == wild.alt_close_)
				{
					--depth;
					if(depth == 0)
					{
						return std::next(begin, 1);
					}
				}
				break;
			}
			case match_alt_state::escape:
			{
				state = match_alt_state::next;
				break;
			}
			}

			std::advance(begin, 1);
		}

		if constexpr (EmitError)
		{
			throw std::invalid_argument("the use of sets is disabled");
		}
		else
		{
			return end;
		}
	}

	enum class match_alt_sub_state
	{
		next,
		escape
	};

	template<typename PatternIterator>
	constexpr PatternIterator check_alt_sub(
		PatternIterator begin,
		PatternIterator end,
		const  wildcard<std::remove_cv_t<std::remove_reference_t<decltype(*std::declval<PatternIterator>())>>>bitand wild = wildcard<std::remove_cv_t<std::remove_reference_t<decltype(*std::declval<PatternIterator>())>>>{},
		match_alt_sub_state state = match_alt_sub_state::next,
		int depth = 1
	)
	{
		if(not wild.alt_enabled_)
		{
			throw std::invalid_argument("the use of alternatives is disabled");
		}

		while(begin not_eq end)
		{
			switch (state)
			{
			case match_alt_sub_state::next:
			{
				if(*begin == wild.escape_)
				{
					state = match_alt_sub_state::escape;
				}
				else if (
					wild.set_enabled_ and
					*begin == wild.set_open_ and
					check_set<false>(std::next(begin), end, wild, match_set_state::not_or_first) not_eq end
					)
				{
					begin = std::prev(check_set<true>(std::next(begin), end, wild, match_set_state::not_or_first));
				}
				else if(*begin == wild.alt_open_)
				{
					++depth;
				}
				else if(*begin == wild.alt_close_)
				{
					--depth;

					if(depth == 0)
					{
						return begin;
					}
				}
				else if(*begin == wild.alt_or_)
				{
					if(depth == 1)
					{
						return begin;
					}
				}

				break;
			}
			case match_alt_sub_state::escape:
			{
				state = match_alt_sub_state::next;
				break;
			}
			}

			std::advance(begin, 1);
		}

		throw std::invalid_argument("the given pattern is not a valid alternative");
	}

	template<typename SequenceIterator, typename PatternIterator, typename EqualTo = std::equal_to<>>
	constexpr match_result<SequenceIterator, PatternIterator> match(
		SequenceIterator sequence_begin, SequenceIterator sequence_end,
		PatternIterator pattern_begin, PatternIterator pattern_end,
		const wildcard<std::remove_cv_t<std::remove_reference_t<decltype(*std::declval<PatternIterator>())>>>bitand wild = wildcard<std::remove_cv_t<std::remove_reference_t<decltype(*std::declval<PatternIterator>())>>>{},
		const EqualTo bitand equal_to = EqualTo{},
		bool partial = false,
		bool escape = false
	);

	template<typename SequenceIterator, typename PatternIterator, typename EqualTo = std::equal_to<>>
	constexpr match_result<SequenceIterator, PatternIterator> match_alt(
		SequenceIterator sequence_begin, SequenceIterator sequence_end,
		PatternIterator pattern1_begin, PatternIterator pattern1_end,
		PatternIterator pattern2_begin, PatternIterator pattern2_end,
		const wildcard<std::remove_cv_t<std::remove_reference_t<decltype(*std::declval<PatternIterator>())>>>bitand wild = wildcard<std::remove_cv_t<std::remove_reference_t<decltype(*std::declval<PatternIterator>())>>>{},
		const EqualTo bitand equal_to = EqualTo{},
		bool partial = false
	)
	{
		if(auto result1 = match(sequence_begin, sequence_end, pattern1_begin, pattern1_end, wild, equal_to, true); result1)
		{
			if(auto result2 = match(result1.sequence, sequence_end, pattern2_begin, pattern2_end, wild, equal_to, partial); result2)
			{
				return result2;
			}
		}

		pattern1_begin = std::next(pattern1_end);

		if(pattern1_begin == pattern2_begin)
		{
			return {
				false,
				sequence_begin,
				pattern1_end
			};
		}

		return match_alt(sequence_begin, sequence_end, pattern1_begin, check_alt_sub(pattern1_begin, pattern2_begin, wild), pattern2_begin, pattern2_end, wild, equal_to, partial);
	}

	template <typename SequenceIterator, typename PatternIterator, typename EqualTo>
	constexpr match_result<SequenceIterator, PatternIterator> match(
		SequenceIterator                                      sequence_begin, SequenceIterator sequence_end, 
		PatternIterator                                       pattern_begin, PatternIterator   pattern_end, 
		const wildcard<std::remove_cv_t<std::remove_reference_t<decltype(*std::declval<PatternIterator>())>>>bitand wild,
		const EqualTo bitand                                        equal_to, 
		bool                                                  partial,
		bool                                            escape
	)
	{
		if(pattern_begin == pattern_end)
		{
			return {
				partial or sequence_begin == sequence_end,
				sequence_begin,
				pattern_begin
			};
		}

		if(escape)
		{
			if(sequence_begin == sequence_end or not equal_to(*sequence_begin, *pattern_begin))
			{
				return {
					false,
					sequence_begin,
					pattern_begin
				};
			}

			return match(std::next(sequence_begin), sequence_end, std::next(pattern_begin), pattern_end, wild, equal_to, partial);
		}

		if(*pattern_begin == wild.anything_)
		{
			if(auto result = match(sequence_begin, sequence_end, std::next(pattern_begin), pattern_end, wild, equal_to, partial); result)
			{
				return result;
			}

			if(sequence_begin == sequence_end)
			{
				return {
					false,
					sequence_begin,
					pattern_begin
				};
			}

			return match(std::next(sequence_begin), sequence_end, pattern_begin, pattern_end, wild, equal_to, partial);
		}

		if(*pattern_begin == wild.single_)
		{
			if(sequence_begin == sequence_end)
			{
				return {
					false,
					sequence_begin,
					pattern_begin
				};
			}

			return match(std::next(sequence_begin), sequence_end, std::next(pattern_begin), pattern_end, wild, equal_to, partial);
		}

		if(*pattern_begin == wild.escape_)
		{
			return match(sequence_begin, sequence_end, std::next(pattern_begin), pattern_end, wild, equal_to, partial, true);
		}

		if (
			wild.set_enabled_ and
			*pattern_begin == wild.set_open_ and
			check_set<false>(std::next(pattern_begin), pattern_end, wild, match_set_state::not_or_first) not_eq pattern_end
			)
		{
			if(auto result = match_set(sequence_begin, sequence_end, std::next(pattern_begin), pattern_end, wild, equal_to, match_state::not_or_first_in); not result)
			{
				return result;
			}

			return match(std::next(sequence_begin), sequence_end, check_set<true>(std::next(pattern_begin), pattern_end, wild, match_set_state::not_or_first), pattern_end, wild, equal_to, partial);
		}

		if (
			wild.alt_enabled_ and
			*pattern_begin == wild.alt_open_ and
			check_alt<false>(std::next(pattern_begin), pattern_end, wild, match_alt_state::next, 1)
			)
		{
			auto pattern_alt_end = check_alt<true>(std::next(pattern_begin), pattern_end, wild, match_alt_state::next, 1);

			return match_alt(sequence_begin, sequence_end, std::next(pattern_begin), check_alt_sub(std::next(pattern_begin), pattern_alt_end, wild), pattern_alt_end, pattern_end, wild, equal_to, partial);
		}

		if(sequence_begin == sequence_end or not equal_to(*sequence_begin, *pattern_begin))
		{
			return {
				false,
				sequence_begin,
				pattern_begin
			};
		}

		return match(std::next(sequence_begin), sequence_end, std::next(pattern_begin), pattern_end, wild, equal_to, partial);
	}


	//================================
	template<typename Sequence, typename Pattern, typename EqualTo = std::equal_to<>>
	constexpr full_match_result<
		std::remove_cv_t<std::remove_reference_t<decltype(std::cbegin(std::declval<Sequence>()))>>,
		std::remove_cv_t<std::remove_reference_t<decltype(std::cbegin(std::declval<Pattern>()))>>
	>
	match(
		Sequence and sequence,
		Pattern and pattern,
		const wildcard<std::remove_cv_t<std::remove_reference_t<decltype(*std::begin(std::declval<Pattern>()))>>>bitand wild = wildcard<std::remove_cv_t<std::remove_reference_t<decltype(*std::begin(std::declval<Pattern>()))>>>{},
		const EqualTo bitand equal_to = EqualTo{}
	)
	{
		return make_full_result(
			std::cbegin(sequence), std::cend(sequence),
			std::cbegin(pattern), std::cend(pattern),
			match(std::cbegin(sequence), std::cend(std::forward<Sequence>(sequence)), std::cbegin(pattern), std::cend(std::forward<Pattern>(pattern)), wild, equal_to)
		);
	}

	template<typename Sequence, typename Pattern, typename EqualTo = std::equal_to<>, typename = std::enable_if_t<not std::is_same_v<EqualTo, wildcard_type>>>
		constexpr full_match_result<
		std::remove_cv_t<std::remove_reference_t<decltype(std::cbegin(std::declval<Sequence>()))>>,
		std::remove_cv_t<std::remove_reference_t<decltype(std::cbegin(std::declval<Pattern>()))>>
		>
		match(
			Sequence and sequence,
			Pattern and pattern,
			const EqualTo bitand equal_to
	)
	{
		return match(
			std::forward<Sequence>(sequence),
			std::forward<Pattern>(pattern),
			wildcard<std::remove_cv_t<std::remove_reference_t<decltype(*std::begin(std::declval<Pattern>()))>>>{},
			equal_to);
	}

	template<typename Pattern, typename EqualTo = std::equal_to<>>
	class compile_time_matcher
	{
	public:
		using value_type = std::remove_cv_t<std::remove_reference_t<decltype(*std::begin(std::declval<Pattern>()))>>;
		using iterator = std::remove_cv_t<std::remove_reference_t<decltype(std::begin(std::declval<Pattern>()))>>;
		using const_iterator = std::remove_cv_t<std::remove_reference_t<decltype(std::cbegin(std::declval<Pattern>()))>>;

		constexpr explicit compile_time_matcher(
			Pattern and pattern,
			const wildcard<value_type> bitand wild = wildcard<value_type>{},
			const EqualTo bitand equal_to = {}
		) : begin_(std::cbegin(pattern)),
		end_(std::cend(std::forward<Pattern>(pattern))),
		wildcard_(wild),
		equal_to_(equal_to) {}

		constexpr compile_time_matcher(
			Pattern and pattern,
			const EqualTo bitand equal_to
		) : compile_time_matcher(std::forward<Pattern>(pattern), {}, equal_to) {}

		template<typename Sequence>
		constexpr full_match_result<const_iterator, std::remove_cv_t<std::remove_reference_t<decltype(std::cbegin(std::declval<Sequence>()))>>>
		operator()(Sequence and sequence) const
		{
			return make_full_result(
				std::cbegin(sequence), std::cend(sequence),
				begin_, end_,
				match(std::cbegin(sequence), std::cend(std::forward<Sequence>(sequence)), begin_, end_, wildcard_, equal_to_)
			);
		}
	
	private:
		const_iterator begin_;
		const_iterator end_;
		wildcard<value_type> wildcard_;
		[[no_unique_address]] EqualTo equal_to_;
	};

	template<typename Pattern, typename EqualTo = std::equal_to<>>
	constexpr compile_time_matcher<Pattern, EqualTo> make_matcher(
		Pattern and pattern,
		const wildcard<std::remove_cv_t<std::remove_reference_t<decltype(*std::begin(std::declval<Pattern>()))>>>bitand wild = wildcard<std::remove_cv_t<std::remove_reference_t<decltype(*std::begin(std::declval<Pattern>()))>>>{},
		const EqualTo bitand equal_to = EqualTo{}
	)
	{
		return compile_time_matcher<Pattern, EqualTo>(std::forward<Pattern>(pattern), wild, equal_to);
	}
}
