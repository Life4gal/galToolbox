#pragma once
#include <type_traits>
#include <iterator>
#include <stdexcept>

namespace gal::test
{

	// https://en.cppreference.com/w/cpp/language/string_literal
	/**
	 * default wildcard:
	 *
	 * basic part:
	 * anything -> *
	 * single -> ?
	 * escape -> \
	 *
	 * extend part:
	 *
	 * [] + ! -> the content in [] must appear (at least one) or not appear at all, depending on whether it has `!`
	 * example:
	 *		[abc] means that one of `abc` appears at least once
	 *		[!def] means that none of `def` appears
	 *
	 * () + | -> the content in () must appear (at least one of all the alternatives)
	 * example:
	 *		(a|b|c) means that one of `a`, `b`, `c` appears
	 *		(|d|e|f) means that one of ``, `d`, `e`, `f` appears (`` is means empty, which means empty is acceptable)
	 *
	 *
	 * users can specialize the template type they need, as long as the specified wildcards exist operator==
	*/
	template<typename T>
	struct wildcard_type;

	template<>
	struct wildcard_type<char>
	{
		using value_type = char;
		
		// default
		explicit constexpr wildcard_type() = default;

		// basic
		constexpr wildcard_type(
			const value_type customize_anything, 
			const value_type customize_single, 
			const value_type customize_escape
		)
			:
		anything_(customize_anything),
		single_(customize_single),
		escape_(customize_escape){}

		// basic + extend
		constexpr wildcard_type(
			const value_type customize_anything,
			const value_type customize_single,
			const value_type customize_escape,
					  
			const value_type customize_set_open,
			const value_type customize_set_close,
			const value_type customize_set_not,
					  
			const value_type customize_alt_open,
			const value_type customize_alt_close,
			const value_type customize_alt_or
		)
			:
		anything_(customize_anything),
		single_(customize_single),
		escape_(customize_escape),
		set_open_(customize_set_open),
		set_close_(customize_set_close),
		set_not_(customize_set_not),
		alt_open_(customize_alt_open),
		alt_close_(customize_alt_close),
		alt_or_(customize_alt_or) {}
		
		// standard
		value_type anything_{ '*' };
		value_type single_{ '?' };
		value_type escape_{ '\\' };

		// extended
		value_type set_open_{ '[' };
		value_type set_close_{ ']' };
		value_type set_not_{ '!' };

		value_type alt_open_{ '(' };
		value_type alt_close_{ ')' };
		value_type alt_or_{ '|' };
	};

	template<>
	struct wildcard_type<wchar_t>
	{
		using value_type = wchar_t;

		// default
		explicit constexpr wildcard_type() = default;

		// basic
		constexpr wildcard_type(
			const value_type customize_anything,
			const value_type customize_single,
			const value_type customize_escape
		)
			:
			anything_(customize_anything),
			single_(customize_single),
			escape_(customize_escape) {}

		// basic + extend
		constexpr wildcard_type(
			const value_type customize_anything,
			const value_type customize_single,
			const value_type customize_escape,

			const value_type customize_set_open,
			const value_type customize_set_close,
			const value_type customize_set_not,

			const value_type customize_alt_open,
			const value_type customize_alt_close,
			const value_type customize_alt_or
		)
			:
			anything_(customize_anything),
			single_(customize_single),
			escape_(customize_escape),
			set_open_(customize_set_open),
			set_close_(customize_set_close),
			set_not_(customize_set_not),
			alt_open_(customize_alt_open),
			alt_close_(customize_alt_close),
			alt_or_(customize_alt_or) {}

		// standard
		value_type anything_{ L'*' };
		value_type single_{ L'?' };
		value_type escape_{ L'\\' };

		// extended
		value_type set_open_{ L'[' };
		value_type set_close_{ L']' };
		value_type set_not_{ L'!' };

		value_type alt_open_{ L'(' };
		value_type alt_close_{ L')' };
		value_type alt_or_{ L'|' };
	};

	template<>
	struct wildcard_type<char8_t>
	{
		using value_type = char8_t;

		// default
		explicit constexpr wildcard_type() = default;

		// basic
		constexpr wildcard_type(
			const value_type customize_anything,
			const value_type customize_single,
			const value_type customize_escape
		)
			:
			anything_(customize_anything),
			single_(customize_single),
			escape_(customize_escape) {}

		// basic + extend
		constexpr wildcard_type(
			const value_type customize_anything,
			const value_type customize_single,
			const value_type customize_escape,

			const value_type customize_set_open,
			const value_type customize_set_close,
			const value_type customize_set_not,

			const value_type customize_alt_open,
			const value_type customize_alt_close,
			const value_type customize_alt_or
		)
			:
			anything_(customize_anything),
			single_(customize_single),
			escape_(customize_escape),
			set_open_(customize_set_open),
			set_close_(customize_set_close),
			set_not_(customize_set_not),
			alt_open_(customize_alt_open),
			alt_close_(customize_alt_close),
			alt_or_(customize_alt_or) {}

		// standard
		value_type anything_{ u8'*' };
		value_type single_{ u8'?' };
		value_type escape_{ u8'\\' };

		// extended
		value_type set_open_{ u8'[' };
		value_type set_close_{ u8']' };
		value_type set_not_{ u8'!' };

		value_type alt_open_{ u8'(' };
		value_type alt_close_{ u8')' };
		value_type alt_or_{ u8'|' };
	};

	template<>
	struct wildcard_type<char16_t>
	{
		using value_type = char16_t;

		// default
		explicit constexpr wildcard_type() = default;

		// basic
		constexpr wildcard_type(
			const value_type customize_anything,
			const value_type customize_single,
			const value_type customize_escape
		)
			:
			anything_(customize_anything),
			single_(customize_single),
			escape_(customize_escape) {}

		// basic + extend
		constexpr wildcard_type(
			const value_type customize_anything,
			const value_type customize_single,
			const value_type customize_escape,

			const value_type customize_set_open,
			const value_type customize_set_close,
			const value_type customize_set_not,

			const value_type customize_alt_open,
			const value_type customize_alt_close,
			const value_type customize_alt_or
		)
			:
			anything_(customize_anything),
			single_(customize_single),
			escape_(customize_escape),
			set_open_(customize_set_open),
			set_close_(customize_set_close),
			set_not_(customize_set_not),
			alt_open_(customize_alt_open),
			alt_close_(customize_alt_close),
			alt_or_(customize_alt_or) {}

		// standard
		value_type anything_{ u'*' };
		value_type single_{ u'?' };
		value_type escape_{ u'\\' };

		// extended
		value_type set_open_{ u'[' };
		value_type set_close_{ u']' };
		value_type set_not_{ u'!' };

		value_type alt_open_{ u'(' };
		value_type alt_close_{ u')' };
		value_type alt_or_{ u'|' };
	};
	template<>
	struct wildcard_type<char32_t>
	{
		using value_type = char32_t;

		// default
		explicit constexpr wildcard_type() = default;

		// basic
		constexpr wildcard_type(
			const value_type customize_anything,
			const value_type customize_single,
			const value_type customize_escape
		)
			:
			anything_(customize_anything),
			single_(customize_single),
			escape_(customize_escape) {}

		// basic + extend
		constexpr wildcard_type(
			const value_type customize_anything,
			const value_type customize_single,
			const value_type customize_escape,

			const value_type customize_set_open,
			const value_type customize_set_close,
			const value_type customize_set_not,

			const value_type customize_alt_open,
			const value_type customize_alt_close,
			const value_type customize_alt_or
		)
			:
			anything_(customize_anything),
			single_(customize_single),
			escape_(customize_escape),
			set_open_(customize_set_open),
			set_close_(customize_set_close),
			set_not_(customize_set_not),
			alt_open_(customize_alt_open),
			alt_close_(customize_alt_close),
			alt_or_(customize_alt_or) {}

		// standard
		value_type anything_{ U'*' };
		value_type single_{ U'?' };
		value_type escape_{ U'\\' };

		// extended
		value_type set_open_{ U'[' };
		value_type set_close_{ U']' };
		value_type set_not_{ U'!' };

		value_type alt_open_{ U'(' };
		value_type alt_close_{ U')' };
		value_type alt_or_{ U'|' };
	};

	template<typename SequenceIterator, typename PatternIterator>
	struct full_match_result
	{
		bool result;
		
		[[no_unique_address]] SequenceIterator sequence_begin;
		[[no_unique_address]] SequenceIterator sequence_end;
		
		[[no_unique_address]] PatternIterator pattern_begin;
		[[no_unique_address]] PatternIterator pattern_end;

		[[no_unique_address]] SequenceIterator  match_result_sequence;
		[[no_unique_address]] PatternIterator  match_result_pattern;

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
			sequence_begin, sequence_end,
			pattern_begin, pattern_end,
			match_result.sequence,
			match_result.pattern
		};
	}

	// support type traits
	template<typename Iterator>
	using iterator_value_t = std::remove_cv_t<std::remove_reference_t<decltype(*std::declval<Iterator>())>>;
	template<typename Container>
	using container_value_t = std::remove_cv_t<std::remove_reference_t<decltype(*std::begin(std::declval<Container>()))>>;
	
	enum class match_set_state
	{
		open, // -> `(`
		not_or_first, // -> `!` or first
		first,
		next // -> `)`
	};

	enum class match_state
	{
		open, // -> `(`
		not_or_first_in, // -> `!` or first
		first_out, // first out
		next_in, // next
		next_out
	};

	/**
	 * @brief determine whether the target pattern has a legal set
	 * @tparam EmitError if true, throw a std::invalid_argument instead return end
	 * @tparam PatternIterator iterator type
	 * @param begin pattern begin
	 * @param end pattern end
	 * @param wildcard wildcard
	 * @param state users do not need to know the state, and generally start from the beginning (`(`) to parse
	 * @return if it is, return the position after the last valid position (`)`) of the set, otherwise it returns end
	*/
	template<bool EmitError, typename PatternIterator>
	constexpr PatternIterator check_set_exist(
		PatternIterator begin,
		PatternIterator end,
		const  wildcard_type<iterator_value_t<PatternIterator>>bitand wildcard = wildcard_type<iterator_value_t<PatternIterator>>{},
		match_set_state state = match_set_state::open
	)
	{
		while (begin not_eq end)
		{
			switch (state)
			{
			case match_set_state::open: // -> `(`
			{
				// this character not_eq `(`
				if (*begin not_eq wildcard.set_open_)
				{
					// emit error or not
					if constexpr (EmitError)
					{
						throw std::invalid_argument("the given pattern is not a valid set");
					}
					else
					{
						return end;
					}
				}
				// `(` is detected, then the next character should be `!` or the first option
				state = match_set_state::not_or_first;
				break;
			}
			case match_set_state::not_or_first: // -> '!'
			{
				if (*begin == wildcard.set_not_)
				{
					// `!` is detected, then the next character should be the first option
					state = match_set_state::first;
				}
				else
				{
					// the first option is detected, then the next character should be the next option
					state = match_set_state::next;
				}
				break;
			}
			case match_set_state::first:
			{
				// the first option is detected, then the next character should be the next option
				state = match_set_state::next;
				break;
			}
			case match_set_state::next: // -> `)`
			{
				if (*begin == wildcard.set_close_)
				{
					// `)` is detected, then the next character should be the next option
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

	/**
	 * @brief Determine whether the target pattern can match the sequence
	 * @tparam SequenceIterator sequence iterator type
	 * @tparam PatternIterator pattern iterator type
	 * @tparam EqualTo used to determine whether two wildcards are equal, and can also be customized
	 * @param sequence_begin sequence iterator begin
	 * @param sequence_end sequence iterator end
	 * @param pattern_begin pattern iterator begin
	 * @param pattern_end pattern iterator end
	 * @param wildcard wildcard
	 * @param equal_to equal
	 * @param state state users do not need to know the state, and generally start from the beginning (`(`) to parse
	 * @return return a match_result, if the match is successful, the match_result has the position where the sequence matches the pattern, otherwise there is no guarantee
	*/
	template<typename SequenceIterator, typename PatternIterator, typename EqualTo = std::equal_to<>>
	constexpr match_result<SequenceIterator, PatternIterator> match_set(
		SequenceIterator sequence_begin, SequenceIterator sequence_end,
		PatternIterator pattern_begin, PatternIterator pattern_end,
		const  wildcard_type<iterator_value_t<PatternIterator>>bitand wildcard = wildcard_type<iterator_value_t<PatternIterator>>{},
		const EqualTo bitand equal_to = EqualTo{},
		match_state state = match_state::open
	)
	{
		while(pattern_begin not_eq pattern_end)
		{
			switch (state)
			{
			case match_state::open: // -> '(`
			{
				// this character not_eq `(`
				if(*pattern_begin not_eq wildcard.set_open_)
				{
					throw std::invalid_argument("the given pattern is not a valid set");
				}

				// `(` is detected, then the next character should be `!` or the first option
				state = match_state::not_or_first_in;
				break;
			}
			case match_state::not_or_first_in: // -> `!`
			{
				if(*pattern_begin == wildcard.set_not_)
				{
					// `!` is detected, then the next character should be the first option
					state = match_state::first_out;
				}
				else
				{
					// the first option is detected, then the next character should be the next option
					// not a valid sequence
					if(sequence_begin == sequence_end)
					{
						// just return match failed
						return {
							false,
							sequence_begin,
							pattern_begin
						};
					}
					// match succeed
					if(equal_to(*sequence_begin, *pattern_begin))
					{
						return {
							true,
							sequence_begin,
							pattern_begin
						};
					}

					// sequence is valid but match failed, continue to parse
					state = match_state::next_in;
				}
				break;
			}
			case match_state::first_out:
			{
				if(
					// not a valid sequence
					sequence_begin == sequence_end or
					// the match should not succeed here
					equal_to(*sequence_begin, *pattern_begin)
					)
				{
					return {
							false,
							sequence_begin,
							pattern_begin
					};
				}

				state = match_state::next_out;
				break;
			}
			case match_state::next_in:
			{
				if(
					// not a valid sequence
					sequence_begin == sequence_end or
					// pattern ended early
					*pattern_begin == wildcard.set_close_
					)
				{
					return {
							false,
							sequence_begin,
							pattern_begin
					};
				}
				// match succeed
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
				// pattern just ended
				if(*pattern_begin == wildcard.set_close_)
				{
					return {
						true,
						sequence_begin,
						pattern_begin
					};
				}
				if (
					// not a valid sequence
					sequence_begin == sequence_end or
					// the match should not succeed here
					equal_to(*sequence_begin, *pattern_begin)
					)
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

			// move forward 1 step
			std::advance(pattern_begin, 1);
		}

		throw std::invalid_argument("the given pattern is not a valid set");
	}

	enum class match_alt_state
	{
		open, // -> `[`
		next, // -> `|`
		escape
	};

	enum class match_alt_sub_state
	{
		next,
		escape
	};

	/**
	 * @brief determine whether the target pattern has a legal alt
	 * @tparam EmitError if true, throw a std::invalid_argument instead return end
	 * @tparam PatternIterator iterator type
	 * @param begin pattern begin
	 * @param end pattern end
	 * @param wildcard wildcard
	 * @param state users do not need to know the state, and generally start from the beginning (`[`) to parse
	 * @param depth users do not need to know the depth, this is just to confirm whether we have processed all the nested content
	 * @return if it is, return the position after the last valid position (`]`) of the alt, otherwise it returns end
	*/
	template<bool EmitError, typename PatternIterator>
	constexpr PatternIterator check_alt_exist(
		PatternIterator begin,
		PatternIterator end,
		const  wildcard_type<iterator_value_t<PatternIterator>>bitand wildcard = wildcard_type<iterator_value_t<PatternIterator>>{},
		match_alt_state state = match_alt_state::open,
		std::size_t depth = 0
	)
	{
		while(begin not_eq end)
		{
			switch (state)
			{
			case match_alt_state::open: // -> `[`
			{
				// this character not_eq `[`
				if(*begin not_eq wildcard.alt_open_)
				{
					// emit error or not
					if constexpr (EmitError)
					{
						throw std::invalid_argument("the given pattern is not a valid alternative");
					}
					else
					{
						return end;
					}
				}

				// `[` is detected, then the next character should the first option
				state = match_alt_state::next;
				++depth;
				break;
			}
			case match_alt_state::next:
			{
				if(*begin == wildcard.escape_)
				{
					state = match_alt_state::escape;
				}
				else if(
					// if there is a nested set
					*begin == wildcard.set_open_ and
					// and it really exist
					check_set_exist<false>(std::next(begin), end, wildcard, match_set_state::not_or_first) not_eq end
					)
				{
					// reposition begin to a position where `)` is located after skipping this set
					begin = std::prev(check_set_exist<true>(std::next(begin), end, wildcard, match_set_state::not_or_first));
				}
				else if(*begin == wildcard.alt_open_)
				{
					// another nested alt
					++depth;
				}
				else if(*begin == wildcard.alt_close_)
				{
					// current alt finished
					--depth;
					// all possible nested alts are matched
					if(depth == 0)
					{
						return std::next(begin);
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

	/**
	 * @brief 
	 * @tparam PatternIterator 
	 * @param begin 
	 * @param end 
	 * @param wildcard 
	 * @param state 
	 * @param depth 
	 * @return 
	*/
	template<typename PatternIterator>
	constexpr PatternIterator check_alt_sub(
		PatternIterator begin,
		PatternIterator end,
		const  wildcard_type<iterator_value_t<PatternIterator>>bitand wildcard = wildcard_type<iterator_value_t<PatternIterator>>{},
		match_alt_sub_state state = match_alt_sub_state::next,
		std::size_t depth = 1
	)
	{
		while(begin not_eq end)
		{
			switch (state)
			{
			case match_alt_sub_state::next:
			{
				if(*begin == wildcard.escape_)
				{
					state = match_alt_sub_state::escape;
				}
				else if (
					// if there is a nested set
					*begin == wildcard.set_open_ and
					// and it really exist
					check_set_exist<false>(std::next(begin), end, wildcard, match_set_state::not_or_first) not_eq end
					)
				{
					// reposition begin to a position where `)` is located after skipping this set
					begin = std::prev(check_set_exist<true>(std::next(begin), end, wildcard, match_set_state::not_or_first));
				}
				else if(*begin == wildcard.alt_open_)
				{
					// another nested alt
					++depth;
				}
				else if(*begin == wildcard.alt_close_)
				{
					// current alt finished
					--depth;
					// all possible nested alts are matched
					if(depth == 0)
					{
						return begin;
					}
				}
				else if(*begin == wildcard.alt_or_)
				{
					// current alt finished
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
		const wildcard_type<iterator_value_t<PatternIterator>>bitand wildcard = wildcard_type<iterator_value_t<PatternIterator>>{},
		const EqualTo bitand equal_to = EqualTo{},
		bool partial = false,
		bool escape = false
	);

	template<typename SequenceIterator, typename PatternIterator, typename EqualTo = std::equal_to<>>
	constexpr match_result<SequenceIterator, PatternIterator> match_alt(
		SequenceIterator sequence_begin, SequenceIterator sequence_end,
		PatternIterator pattern1_begin, PatternIterator pattern1_end,
		PatternIterator pattern2_begin, PatternIterator pattern2_end,
		const wildcard_type<iterator_value_t<PatternIterator>>bitand wildcard = wildcard_type<iterator_value_t<PatternIterator>>{},
		const EqualTo bitand equal_to = EqualTo{},
		bool partial = false
	)
	{
		if(auto result1 = match(sequence_begin, sequence_end, pattern1_begin, pattern1_end, wildcard, equal_to, true); result1)
		{
			if(auto result2 = match(result1.sequence, sequence_end, pattern2_begin, pattern2_end, wildcard, equal_to, partial); result2)
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

		return match_alt(sequence_begin, sequence_end, pattern1_begin, check_alt_sub(pattern1_begin, pattern2_begin, wildcard), pattern2_begin, pattern2_end, wildcard, equal_to, partial);
	}

	template <typename SequenceIterator, typename PatternIterator, typename EqualTo>
	constexpr match_result<SequenceIterator, PatternIterator> match(
		SequenceIterator                                      sequence_begin, SequenceIterator sequence_end, 
		PatternIterator                                       pattern_begin, PatternIterator   pattern_end, 
		const wildcard_type<iterator_value_t<PatternIterator>>bitand wildcard,
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

			return match(std::next(sequence_begin), sequence_end, std::next(pattern_begin), pattern_end, wildcard, equal_to, partial);
		}

		if(*pattern_begin == wildcard.anything_)
		{
			if(auto result = match(sequence_begin, sequence_end, std::next(pattern_begin), pattern_end, wildcard, equal_to, partial); result)
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

			return match(std::next(sequence_begin), sequence_end, pattern_begin, pattern_end, wildcard, equal_to, partial);
		}

		if(*pattern_begin == wildcard.single_)
		{
			if(sequence_begin == sequence_end)
			{
				return {
					false,
					sequence_begin,
					pattern_begin
				};
			}

			return match(std::next(sequence_begin), sequence_end, std::next(pattern_begin), pattern_end, wildcard, equal_to, partial);
		}

		if(*pattern_begin == wildcard.escape_)
		{
			return match(sequence_begin, sequence_end, std::next(pattern_begin), pattern_end, wildcard, equal_to, partial, true);
		}

		if (
			*pattern_begin == wildcard.set_open_ and
			check_set_exist<false>(std::next(pattern_begin), pattern_end, wildcard, match_set_state::not_or_first) not_eq pattern_end
			)
		{
			if(auto result = match_set(sequence_begin, sequence_end, std::next(pattern_begin), pattern_end, wildcard, equal_to, match_state::not_or_first_in); not result)
			{
				return result;
			}

			return match(std::next(sequence_begin), sequence_end, check_set_exist<true>(std::next(pattern_begin), pattern_end, wildcard, match_set_state::not_or_first), pattern_end, wildcard, equal_to, partial);
		}

		if (
			*pattern_begin == wildcard.alt_open_ and
			check_alt_exist<false>(std::next(pattern_begin), pattern_end, wildcard, match_alt_state::next, 1)
			)
		{
			auto pattern_alt_end = check_alt_exist<true>(std::next(pattern_begin), pattern_end, wildcard, match_alt_state::next, 1);

			return match_alt(sequence_begin, sequence_end, std::next(pattern_begin), check_alt_sub(std::next(pattern_begin), pattern_alt_end, wildcard), pattern_alt_end, pattern_end, wildcard, equal_to, partial);
		}

		if(sequence_begin == sequence_end or not equal_to(*sequence_begin, *pattern_begin))
		{
			return {
				false,
				sequence_begin,
				pattern_begin
			};
		}

		return match(std::next(sequence_begin), sequence_end, std::next(pattern_begin), pattern_end, wildcard, equal_to, partial);
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
		const wildcard_type<container_value_t<Pattern>>bitand wildcard = wildcard_type<container_value_t<Pattern>>{},
		const EqualTo bitand equal_to = EqualTo{}
	)
	{
		return make_full_result(
			std::cbegin(sequence), std::cend(sequence),
			std::cbegin(pattern), std::cend(pattern),
			match(std::cbegin(sequence), std::cend(std::forward<Sequence>(sequence)), std::cbegin(pattern), std::cend(std::forward<Pattern>(pattern)), wildcard, equal_to)
		);
	}

	template<typename Pattern, typename EqualTo = std::equal_to<>>
	class compile_time_matcher
	{
	public:
		using value_type = container_value_t<Pattern>;
		using iterator = std::remove_cv_t<std::remove_reference_t<decltype(std::begin(std::declval<Pattern>()))>>;
		using const_iterator = std::remove_cv_t<std::remove_reference_t<decltype(std::cbegin(std::declval<Pattern>()))>>;

		constexpr explicit compile_time_matcher(
			Pattern and pattern,
			const wildcard_type<value_type> bitand wildcard = wildcard_type<value_type>{},
			const EqualTo bitand equal_to = {}
		) : begin_(std::cbegin(pattern)),
		end_(std::cend(std::forward<Pattern>(pattern))),
		wildcard_(wildcard),
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
		wildcard_type<value_type> wildcard_;
		[[no_unique_address]] EqualTo equal_to_;
	};

	template<typename Pattern, typename EqualTo = std::equal_to<>>
	constexpr compile_time_matcher<Pattern, EqualTo> make_matcher(
		Pattern and pattern,
		const wildcard_type<container_value_t<Pattern>>bitand wild = wildcard_type<container_value_t<Pattern>>{},
		const EqualTo bitand equal_to = EqualTo{}
	)
	{
		return compile_time_matcher<Pattern, EqualTo>(std::forward<Pattern>(pattern), wild, equal_to);
	}
}
