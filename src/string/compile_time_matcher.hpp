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
	template <typename T>
	struct wildcard_type;

	template <>
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
			: anything_(customize_anything),
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
			: anything_(customize_anything),
			single_(customize_single),
			escape_(customize_escape),
			set_open_(customize_set_open),
			set_close_(customize_set_close),
			set_not_(customize_set_not),
			alt_open_(customize_alt_open),
			alt_close_(customize_alt_close),
			alt_or_(customize_alt_or) {}

		// standard
		value_type anything_{'*'};
		value_type single_{'?'};
		value_type escape_{'\\'};

		// extended
		value_type set_open_{'['};
		value_type set_close_{']'};
		value_type set_not_{'!'};

		value_type alt_open_{'('};
		value_type alt_close_{')'};
		value_type alt_or_{'|'};
	};

	template <>
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
			: anything_(customize_anything),
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
			: anything_(customize_anything),
			single_(customize_single),
			escape_(customize_escape),
			set_open_(customize_set_open),
			set_close_(customize_set_close),
			set_not_(customize_set_not),
			alt_open_(customize_alt_open),
			alt_close_(customize_alt_close),
			alt_or_(customize_alt_or) {}

		// standard
		value_type anything_{L'*'};
		value_type single_{L'?'};
		value_type escape_{L'\\'};

		// extended
		value_type set_open_{L'['};
		value_type set_close_{L']'};
		value_type set_not_{L'!'};

		value_type alt_open_{L'('};
		value_type alt_close_{L')'};
		value_type alt_or_{L'|'};
	};

	template <>
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
			: anything_(customize_anything),
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
			: anything_(customize_anything),
			single_(customize_single),
			escape_(customize_escape),
			set_open_(customize_set_open),
			set_close_(customize_set_close),
			set_not_(customize_set_not),
			alt_open_(customize_alt_open),
			alt_close_(customize_alt_close),
			alt_or_(customize_alt_or) {}

		// standard
		value_type anything_{u8'*'};
		value_type single_{u8'?'};
		value_type escape_{u8'\\'};

		// extended
		value_type set_open_{u8'['};
		value_type set_close_{u8']'};
		value_type set_not_{u8'!'};

		value_type alt_open_{u8'('};
		value_type alt_close_{u8')'};
		value_type alt_or_{u8'|'};
	};

	template <>
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
			: anything_(customize_anything),
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
			: anything_(customize_anything),
			single_(customize_single),
			escape_(customize_escape),
			set_open_(customize_set_open),
			set_close_(customize_set_close),
			set_not_(customize_set_not),
			alt_open_(customize_alt_open),
			alt_close_(customize_alt_close),
			alt_or_(customize_alt_or) {}

		// standard
		value_type anything_{u'*'};
		value_type single_{u'?'};
		value_type escape_{u'\\'};

		// extended
		value_type set_open_{u'['};
		value_type set_close_{u']'};
		value_type set_not_{u'!'};

		value_type alt_open_{u'('};
		value_type alt_close_{u')'};
		value_type alt_or_{u'|'};
	};

	template <>
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
			: anything_(customize_anything),
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
			: anything_(customize_anything),
			single_(customize_single),
			escape_(customize_escape),
			set_open_(customize_set_open),
			set_close_(customize_set_close),
			set_not_(customize_set_not),
			alt_open_(customize_alt_open),
			alt_close_(customize_alt_close),
			alt_or_(customize_alt_or) {}

		// standard
		value_type anything_{U'*'};
		value_type single_{U'?'};
		value_type escape_{U'\\'};

		// extended
		value_type set_open_{U'['};
		value_type set_close_{U']'};
		value_type set_not_{U'!'};

		value_type alt_open_{U'('};
		value_type alt_close_{U')'};
		value_type alt_or_{U'|'};
	};

	template <typename SequenceIterator, typename PatternIterator>
	struct full_match_result
	{
		bool result;

		[[no_unique_address]] SequenceIterator sequence_begin;
		[[no_unique_address]] SequenceIterator sequence_end;

		[[no_unique_address]] PatternIterator pattern_begin;
		[[no_unique_address]] PatternIterator pattern_end;

		[[no_unique_address]] SequenceIterator match_result_sequence;
		[[no_unique_address]] PatternIterator  match_result_pattern;

		// ReSharper disable once CppNonExplicitConversionOperator
		constexpr operator bool() const noexcept
		{
			return result;
		}
	};

	template <typename SequenceIterator, typename PatternIterator>
	struct match_result
	{
		bool result;

		[[no_unique_address]] SequenceIterator sequence;
		[[no_unique_address]] PatternIterator  pattern;

		// ReSharper disable once CppNonExplicitConversionOperator
		constexpr operator bool() const noexcept
		{
			return result;
		}
	};

	template <typename SequenceIterator, typename PatternIterator>
	constexpr full_match_result<SequenceIterator, PatternIterator> make_full_result(
		SequenceIterator                                sequence_begin, SequenceIterator sequence_end,
		PatternIterator                                 pattern_begin, PatternIterator   pattern_end,
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
	template <typename Iterator>
	using iterator_value_t = std::remove_cv_t<std::remove_reference_t<decltype(*std::declval<Iterator>())>>;
	template <typename Container>
	using container_value_t = std::remove_cv_t<std::remove_reference_t<decltype(*std::begin(std::declval<Container>())
	)>>;
	template <typename Container>
	using container_iterator_t = std::remove_cv_t<std::remove_reference_t<decltype(
		std::cbegin(std::declval<Container>()))>>;

	// todo: this shouldn't be needed
	// gcc&clang: well, we do not restrict you to use throw in constexpr functions, as long as it is not actually executed
	// MSVC: NO WAY!!!
	template <typename T = std::nullptr_t>
	constexpr auto just_throw_it(const char* message, T t = T{})
	{
		return message == nullptr
					? t
					: throw std::invalid_argument(message);
	}

	enum class check_set_state
	{
		open,
		// -> `[`
		not_or_first,
		// -> `!` the first option
		first,
		// the first option
		next // other options
	};

	enum class match_set_state
	{
		open,
		// -> `[`
		not_or_first_in,
		// -> `!` or the first option
		first_out,
		// the first option that should be excluded
		next_in,
		// other options that should be excluded
		next_out // other options that should be included
	};

	/**
	 * @brief determine whether the target pattern has a legal set
	 * @note after detecting the first `[`, we always think that the next position is either the first option or `!` (further, the same is true after detecting `!`)
	 * so the following `]` will always be ignored (so `[]` and `[!]` Is not a complete set)
	 * and ignore all the `[` contained in it (so nested sets are not supported)
	 * @tparam EmitError if true, throw a std::invalid_argument instead return end
	 * @tparam PatternIterator iterator type
	 * @param begin pattern begin
	 * @param end pattern end
	 * @param wildcard wildcard
	 * @param state users do not need to know the state, and generally start from the beginning (`[`) to parse
	 * @return if it is, return the position after the last valid position (`]`) of the set, otherwise it returns begin (perhaps returning end is more intuitive, but in fact it may happen to reach end and the match is successful, so returning end is not necessarily a failure, but it is absolutely impossible to return begin when it succeeds)
	*/
	template <bool EmitError, typename PatternIterator>
	constexpr PatternIterator check_set_exist(
		PatternIterator                                              begin,
		PatternIterator                                              end,
		const wildcard_type<iterator_value_t<PatternIterator>>bitand wildcard = wildcard_type<iterator_value_t<
			PatternIterator>>{},
		check_set_state state = check_set_state::open
		)
	{
		auto begin_bak = begin;
		while (begin not_eq end)
		{
			switch (state)
			{
				case check_set_state::open: // -> `[`
				{
					// this character not_eq `[`
					if (*begin not_eq wildcard.set_open_)
					{
						// emit error or not
						if constexpr (EmitError)
						{
							return just_throw_it("the given pattern is not a valid set", begin_bak);
							// throw std::invalid_argument("the given pattern is not a valid set");
						}
						else
						{
							return begin_bak;
						}
					}
					// `[` is detected, then the next character should be `!` or the first option
					state = check_set_state::not_or_first;
					break;
				}
				case check_set_state::not_or_first: // -> '!'
				{
					if (*begin == wildcard.set_not_)
					{
						// `!` is detected, then the next character should be the first option
						state = check_set_state::first;
					}
					else
					{
						// the first option is detected, then the next character should be the next option
						state = check_set_state::next;
					}
					break;
				}
				case check_set_state::first:
				{
					// the first option is detected, then the next character should be the next option
					state = check_set_state::next;
					break;
				}
				case check_set_state::next: // -> `]`
				{
					if (*begin == wildcard.set_close_)
					{
						// `]` is detected, then the next character should be the next option
						return std::next(begin);
					}
					break;
				}
			}

			std::advance(begin, 1);
		}

		if constexpr (EmitError)
		{
			return just_throw_it("the given pattern is not a valid set", begin_bak);
			// throw std::invalid_argument("the given pattern is not a valid set");
		}
		else
		{
			return begin_bak;
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
	 * @param state state users do not need to know the state, and generally start from the beginning (`[`) to parse
	 * @return return a match_result, if the match is successful, the match_result has the position where the sequence matches the pattern, otherwise there is no guarantee
	*/
	template <typename SequenceIterator, typename PatternIterator, typename EqualTo = std::equal_to<>>
	constexpr match_result<SequenceIterator, PatternIterator> match_set(
		SequenceIterator                                             sequence_begin,
		SequenceIterator                                             sequence_end,
		PatternIterator                                              pattern_begin,
		PatternIterator                                              pattern_end,
		const wildcard_type<iterator_value_t<PatternIterator>>bitand wildcard = wildcard_type<iterator_value_t<
			PatternIterator>>{},
		const EqualTo bitand equal_to = EqualTo{},
		match_set_state     state    = match_set_state::open
		)
	{
		while (pattern_begin not_eq pattern_end)
		{
			switch (state)
			{
				case match_set_state::open: // -> '[`
				{
					// this character not_eq `[`
					if (*pattern_begin not_eq wildcard.set_open_)
					{
						return just_throw_it("the given pattern is not a valid set",
											match_result{false, sequence_begin, pattern_begin});
						// throw std::invalid_argument("the given pattern is not a valid set");
					}

					// `[` is detected, then the next character should be `!` or the first option
					state = match_set_state::not_or_first_in;
					break;
				}
				case match_set_state::not_or_first_in: // -> `!`
				{
					if (*pattern_begin == wildcard.set_not_)
					{
						// `!` is detected, then the next character should be the first option
						state = match_set_state::first_out;
					}
					else
					{
						// the first option is detected, then the next character should be the next option
						// not a valid sequence
						if (sequence_begin == sequence_end)
						{
							// just return match failed
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

						// sequence is valid but match failed, continue to parse
						state = match_set_state::next_in;
					}
					break;
				}
				case match_set_state::first_out:
				{
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

					state = match_set_state::next_out;
					break;
				}
				case match_set_state::next_in:
				{
					if (
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
				case match_set_state::next_out:
				{
					// pattern just ended
					if (*pattern_begin == wildcard.set_close_)
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

		return just_throw_it("the given pattern is not a valid set",
							match_result{false, sequence_begin, pattern_begin});
		// throw std::invalid_argument("the given pattern is not a valid set");
	}

	enum class check_alt_state
	{
		open,
		// -> `(`
		next,
		// not the first
		escape
		// the next option after the `\\` does not have a special meaning (represented as the token we specified) and skip it (in short, treat it as a normal option)
	};

	enum class match_alt_sub_state
	{
		next,
		// not the first
		escape
		// the next option after the `\\` does not have a special meaning (represented as the token we specified) and skip it (in short, treat it as a normal option)
	};

	/**
	 * @brief determine whether the target pattern has a legal alt
	 * @note the detection will ignore the nested set and alt, and return the position of the terminator corresponding to the outermost alt
	 * @tparam EmitError if true, throw a std::invalid_argument instead return end
	 * @tparam PatternIterator iterator type
	 * @param begin pattern begin
	 * @param end pattern end
	 * @param wildcard wildcard
	 * @param state users do not need to know the state, and generally start from the beginning (`(`) to parse
	 * @param depth users do not need to know the depth, this is just to confirm whether we have processed all the nested content
	 * @param is_sub users do not need to know the is_sub, this just indicates whether it is currently detecting sub-alts (`|`)
	 * @return if it is, return the position after the last valid position (`)`) of the alt, otherwise it returns begin (perhaps returning end is more intuitive, but in fact it may happen to reach end and the match is successful, so returning end is not necessarily a failure, but it is absolutely impossible to return begin when it succeeds)
	*/
	template <bool EmitError, typename PatternIterator>
	constexpr PatternIterator check_alt_exist(
		PatternIterator                                              begin,
		PatternIterator                                              end,
		const wildcard_type<iterator_value_t<PatternIterator>>bitand wildcard = wildcard_type<iterator_value_t<
			PatternIterator>>{},
		check_alt_state state  = check_alt_state::open,
		std::size_t     depth  = 0,
		bool            is_sub = false
		)
	{
		auto begin_bak = begin;
		while (begin not_eq end)
		{
			switch (state)
			{
				case check_alt_state::open: // -> `(`
				{
					// this character not_eq `(`
					if (*begin not_eq wildcard.alt_open_)
					{
						// emit error or not
						if constexpr (EmitError)
						{
							return just_throw_it("the given pattern is not a valid alternative", begin_bak);
							// throw std::invalid_argument("the given pattern is not a valid alternative");
						}
						else
						{
							return begin_bak;
						}
					}

					// `(` is detected, then the next character should the first option
					state = check_alt_state::next;
					++depth;
					break;
				}
				case check_alt_state::next:
				{
					if (*begin == wildcard.escape_)
					{
						state = check_alt_state::escape;
					}
					else if (*begin == wildcard.set_open_)
					{
						// if there is a nested set
						if (auto pattern_set_end = check_set_exist<false>(std::next(begin),
																		end,
																		wildcard,
																		check_set_state::not_or_first); pattern_set_end
							not_eq std::next(begin))
						{
							// and it really exist
							// reposition begin to a position where `]` is located after skipping this set
							// why take prev one step back? because we will go one step further below
							begin = std::prev(pattern_set_end);
						}
					}
					else if (*begin == wildcard.alt_open_)
					{
						// another nested alt
						++depth;
					}
					else if (*begin == wildcard.alt_close_)
					{
						// current alt finished
						--depth;
						// all possible nested alts are matched
						if (depth == 0)
						{
							if (is_sub)
							{
								return begin;
							}
							return std::next(begin);
						}
					}
					else if (is_sub and *begin == wildcard.alt_or_)
					{
						// current alt finished
						if (depth == 1)
						{
							return begin;
						}
					}
					break;
				}
				case check_alt_state::escape:
				{
					state = check_alt_state::next;
					break;
				}
			}

			std::advance(begin, 1);
		}

		if constexpr (EmitError)
		{
			return just_throw_it("of sets is disabled", begin_bak);
			// throw std::invalid_argument("the use of sets is disabled");
		}
		else
		{
			return begin_bak;
		}
	}

	/**
	 * @brief determine whether the target pattern has sub-alternatives
	 * @tparam PatternIterator iterator type 
	 * @param begin pattern begin
	 * @param end pattern end
	 * @param wildcard wildcard
	 * @return guarantee to return the end position of the sub-alternative, and throw an exception if it does not exist
	*/
	template <typename PatternIterator>
	constexpr PatternIterator check_sub_alt_exist(
		PatternIterator                                              begin,
		PatternIterator                                              end,
		const wildcard_type<iterator_value_t<PatternIterator>>bitand wildcard = wildcard_type<iterator_value_t<
			PatternIterator>>{})
	{
		return check_alt_exist<true>(begin, end, wildcard, check_alt_state::next, 1, true);
	}

	/**
	 * @brief determine whether the target sequence can match the pattern
	 * @tparam SequenceIterator sequence iterator type
	 * @tparam PatternIterator pattern iterator type
	 * @tparam EqualTo equal to type
	 * @param sequence_begin sequence begin
	 * @param sequence_end sequence end
	 * @param pattern_begin pattern begin
	 * @param pattern_end pattern end
	 * @param wildcard wildcard
	 * @param equal_to equal to
	 * @param partial it is a partial match
	 * @param escape escape means that the next content we will directly compare without considering it as a possible token we defined
	 * @return does the target sequence can match the pattern?
	*/
	template <typename SequenceIterator, typename PatternIterator, typename EqualTo = std::equal_to<>>
	constexpr match_result<SequenceIterator, PatternIterator> match(
		SequenceIterator                                             sequence_begin,
		SequenceIterator                                             sequence_end,
		PatternIterator                                              pattern_begin,
		PatternIterator                                              pattern_end,
		const wildcard_type<iterator_value_t<PatternIterator>>bitand wildcard = wildcard_type<iterator_value_t<
			PatternIterator>>{},
		const EqualTo bitand equal_to = EqualTo{},
		bool                partial  = false,
		bool                escape   = false
		);

	/**
	 * @brief determine whether the target sequence can match the pattern of the two parts
	 * @tparam SequenceIterator sequence iterator type
	 * @tparam PatternIterator pattern iterator type
	 * @tparam EqualTo equal to type
	 * @param sequence_begin sequence begin
	 * @param sequence_end sequence end
	 * @param pattern1_begin pattern1 begin
	 * @param pattern1_end pattern1 end
	 * @param pattern2_begin pattern2 begin
	 * @param pattern2_end pattern2 end
	 * @param wildcard wildcard
	 * @param equal_to equal to
	 * @param partial it is a partial match
	 * @return does the target sequence can match the pattern of the two parts?
	*/
	template <typename SequenceIterator, typename PatternIterator, typename EqualTo = std::equal_to<>>
	constexpr match_result<SequenceIterator, PatternIterator> match_alt(
		SequenceIterator                                             sequence_begin,
		SequenceIterator                                             sequence_end,
		PatternIterator                                              pattern1_begin,
		PatternIterator                                              pattern1_end,
		PatternIterator                                              pattern2_begin,
		PatternIterator                                              pattern2_end,
		const wildcard_type<iterator_value_t<PatternIterator>>bitand wildcard = wildcard_type<iterator_value_t<
			PatternIterator>>{},
		const EqualTo bitand equal_to = EqualTo{},
		bool                partial  = false
		)
	{
		// is the target sequence partial matches pattern1
		if (auto result1 = match(sequence_begin,
								sequence_end,
								pattern1_begin,
								pattern1_end,
								wildcard,
								equal_to,
								true); result1)
		{
			// is the target sequence matches pattern2
			if (auto result2 = match(result1.sequence,
									sequence_end,
									pattern2_begin,
									pattern2_end,
									wildcard,
									equal_to,
									partial); result2)
			{
				return result2;
			}
		}

		pattern1_begin = std::next(pattern1_end);

		// pattern1 and pattern2 are two connected parts
		if (pattern1_begin == pattern2_begin)
		{
			return {
				false,
				sequence_begin,
				pattern1_end
			};
		}

		// the current position is relatively successful, continue to compare the next position
		return match_alt(
						sequence_begin,
						sequence_end,
						pattern1_begin,
						check_sub_alt_exist(pattern1_begin, pattern2_begin, wildcard),
						pattern2_begin,
						pattern2_end,
						wildcard,
						equal_to,
						partial
						);
	}

	template <typename SequenceIterator, typename PatternIterator, typename EqualTo>
	constexpr match_result<SequenceIterator, PatternIterator> match(
		SequenceIterator                                             sequence_begin,
		SequenceIterator                                             sequence_end,
		PatternIterator                                              pattern_begin,
		PatternIterator                                              pattern_end,
		const wildcard_type<iterator_value_t<PatternIterator>>bitand wildcard,
		const EqualTo bitand                                          equal_to,
		bool                                                         partial,
		bool                                                         escape
		)
	{
		// not a valid pattern
		if (pattern_begin == pattern_end)
		{
			return {
				// if it is a partial match or is not a valid sequence, the match is considered successful
				partial or
				sequence_begin == sequence_end,
				sequence_begin,
				pattern_begin
			};
		}

		if (escape)
		{
			if (
					// not a valid sequence
				sequence_begin == sequence_end or
				// the match not succeed here
				not equal_to(*sequence_begin, *pattern_begin)
			)
			{
				return {
					false,
					sequence_begin,
					pattern_begin
				};
			}

			// the current position is relatively successful, continue to compare the next position
			return match(
						std::next(sequence_begin),
						sequence_end,
						std::next(pattern_begin),
						pattern_end,
						wildcard,
						equal_to,
						partial
						);
		}

		if (*pattern_begin == wildcard.anything_)
		{
			// if the current position of the pattern is `*`, try to match the next position of the pattern
			// if the match is still successful, skip the current `*`
			if (auto result = match(
									sequence_begin,
									sequence_end,
									std::next(pattern_begin),
									pattern_end,
									wildcard,
									equal_to,
									partial);
				result
			)
			{
				return result;
			}

			// not a valid sequence
			if (sequence_begin == sequence_end)
			{
				return {
					false,
					sequence_begin,
					pattern_begin
				};
			}

			// if the match is not successful, skip the current position of sequence
			return match(
						std::next(sequence_begin),
						sequence_end,
						pattern_begin,
						pattern_end,
						wildcard,
						equal_to,
						partial
						);
		}

		if (*pattern_begin == wildcard.single_)
		{
			// if the current position of the pattern is `?`
			// not a valid sequence
			if (sequence_begin == sequence_end)
			{
				return {
					false,
					sequence_begin,
					pattern_begin
				};
			}

			// try to match the next position of the pattern and sequence
			return match(
						std::next(sequence_begin),
						sequence_end,
						std::next(pattern_begin),
						pattern_end,
						wildcard,
						equal_to,
						partial
						);
		}

		if (*pattern_begin == wildcard.escape_)
		{
			// match the next position of the pattern
			return match(
						sequence_begin,
						sequence_end,
						std::next(pattern_begin),
						pattern_end,
						wildcard,
						equal_to,
						partial,
						true
						);
		}

		if (*pattern_begin == wildcard.set_open_)
		{
			// if there is a nested set
			if (auto pattern_set_end = check_set_exist<false>(
															std::next(pattern_begin),
															pattern_end,
															wildcard,
															check_set_state::not_or_first);
				pattern_set_end not_eq std::next(pattern_begin)
			)
			{
				// and it really exist

				// if the nested set does not match successfully, the result will be returned directly (the match failed)
				if (auto result = match_set(
											sequence_begin,
											sequence_end,
											std::next(pattern_begin),
											pattern_end,
											wildcard,
											equal_to,
											match_set_state::not_or_first_in);
					not result
				)
				{
					return result;
				}

				// after the match is successful, skip this nested set to continue matching
				return match(
							std::next(sequence_begin),
							sequence_end,
							pattern_set_end,
							pattern_end,
							wildcard,
							equal_to,
							partial
							);
			}
		}

		if (*pattern_begin == wildcard.alt_open_)
		{
			// if there is a nested alt
			if (auto pattern_alt_end = check_alt_exist<false>(
															std::next(pattern_begin),
															pattern_end,
															wildcard,
															check_alt_state::next,
															1);
				pattern_alt_end not_eq std::next(pattern_begin)
			)
			{
				// and it really exist

				// match one of the alternatives
				return match_alt(
								sequence_begin,
								sequence_end,
								std::next(pattern_begin),
								check_sub_alt_exist(std::next(pattern_begin), pattern_alt_end, wildcard),
								pattern_alt_end,
								pattern_end,
								wildcard,
								equal_to,
								partial
								);
			}
		}

		if (
				// not a valid sequence
			sequence_begin == sequence_end or
			// the match not succeed here
			not equal_to(*sequence_begin, *pattern_begin)
		)
		{
			return {
				false,
				sequence_begin,
				pattern_begin
			};
		}

		// the current position is relatively successful, continue to compare the next position
		return match(
					std::next(sequence_begin),
					sequence_end,
					std::next(pattern_begin),
					pattern_end,
					wildcard,
					equal_to,
					partial
					);
	}


	//================================
	template <typename Sequence, typename Pattern, typename EqualTo = std::equal_to<>>
	constexpr full_match_result<container_iterator_t<Sequence>, container_iterator_t<Pattern>>
	match(
		Sequence and                                           sequence,
		Pattern and                                            pattern,
		const wildcard_type<container_value_t<Pattern>>bitand wildcard = wildcard_type<container_value_t<Pattern>>{},
		const EqualTo bitand                                   equal_to = EqualTo{}
		)
	{
		return make_full_result(
								std::cbegin(std::forward<Sequence>(sequence)),
								std::cend(std::forward<Sequence>(sequence)),
								std::cbegin(std::forward<Pattern>(pattern)),
								std::cend(std::forward<Pattern>(pattern)),
								match(
									std::cbegin(std::forward<Sequence>(sequence)),
									std::cend(std::forward<Sequence>(sequence)),
									std::cbegin(std::forward<Pattern>(pattern)),
									std::cend(std::forward<Pattern>(pattern)),
									wildcard,
									equal_to
									)
								);
	}

	template <typename Pattern, typename EqualTo = std::equal_to<>>
	class compile_time_matcher
	{
	public:
		using pattern_type = Pattern;
		using pattern_reference = Pattern and;

		template <typename Sequence>
		using sequence_type = Sequence;
		template <typename Sequence>
		using sequence_reference = Sequence and;

		using value_type = container_value_t<pattern_type>;
		using iterator = container_iterator_t<Pattern>;

		using default_wildcard_type = wildcard_type<value_type>;
		using default_wildcard_reference = const wildcard_type<value_type>bitand;

		using default_equal_to_type = EqualTo;
		using default_equal_to_reference = const EqualTo bitand;

		constexpr explicit compile_time_matcher(
			pattern_reference          pattern,
			default_wildcard_reference wildcard,
			default_equal_to_reference equal_to
			)
			: begin_(std::cbegin(std::forward<pattern_type>(pattern))),
			end_(std::cend(std::forward<pattern_type>(pattern))),
			wildcard_(wildcard),
			equal_to_(equal_to) {}

		constexpr compile_time_matcher(
			pattern_reference          pattern,
			default_equal_to_reference equal_to
			) : compile_time_matcher(std::forward<pattern_type>(pattern), wildcard_type<value_type>{}, equal_to) {}

		constexpr compile_time_matcher(
			pattern_reference          pattern,
			default_wildcard_reference wildcard
			) : compile_time_matcher(std::forward<pattern_type>(pattern), wildcard, std::equal_to{}) {}

		constexpr explicit compile_time_matcher(
			pattern_reference pattern
			)
			: compile_time_matcher(std::forward<pattern_type>(pattern), wildcard_type<value_type>{}, std::equal_to{}) {}

		template <typename Sequence>
		constexpr full_match_result<container_iterator_t<Sequence>, iterator>
		operator()(sequence_reference<Sequence> sequence) const
		{
			return make_full_result(
									std::cbegin(std::forward<Sequence>(sequence)),
									std::cend(std::forward<Sequence>(sequence)),
									begin_,
									end_,
									match(
										std::cbegin(std::forward<Sequence>(sequence)),
										std::cend(std::forward<Sequence>(sequence)),
										begin_,
										end_,
										wildcard_,
										equal_to_
										)
									);
		}

	private:
		iterator                      begin_;
		iterator                      end_;
		wildcard_type<value_type>     wildcard_;
		[[no_unique_address]] EqualTo equal_to_;
	};

	template <typename Pattern>
	constexpr compile_time_matcher<Pattern, std::equal_to<>> make_matcher(Pattern and pattern)
	{
		return compile_time_matcher<Pattern, std::equal_to<>>(std::forward<Pattern>(pattern),
															wildcard_type<container_value_t<Pattern>>{},
															std::equal_to{});
	}

	template <typename Pattern, typename EqualTo = std::equal_to<>>
		requires requires(EqualTo equal_to)
		{
			{
				equal_to(std::declval<typename compile_time_matcher<Pattern, EqualTo>::value_type>(),
						std::declval<typename compile_time_matcher<Pattern, EqualTo>::value_type>())
			} -> std::same_as<bool>;
		}
	constexpr compile_time_matcher<Pattern, EqualTo> make_matcher(
		Pattern and                                            pattern,
		const wildcard_type<container_value_t<Pattern>>bitand wildcard,
		const EqualTo bitand                                   equal_to = EqualTo{}
		)
	{
		return compile_time_matcher<Pattern, EqualTo>(std::forward<Pattern>(pattern), wildcard, equal_to);
	}

	template <typename Pattern, typename EqualTo = std::equal_to<>>
		requires requires(EqualTo equal_to)
		{
			{
				equal_to(std::declval<typename compile_time_matcher<Pattern, EqualTo>::value_type>(),
						std::declval<typename compile_time_matcher<Pattern, EqualTo>::value_type>())
			} -> std::same_as<bool>;
		}
	constexpr compile_time_matcher<Pattern, EqualTo> make_matcher(
		Pattern and                                            pattern,
		const EqualTo bitand                                   equal_to,
		const wildcard_type<container_value_t<Pattern>>bitand wildcard = wildcard_type<container_value_t<Pattern>>{}
		)
	{
		return compile_time_matcher<Pattern, EqualTo>(std::forward<Pattern>(pattern), wildcard, equal_to);
	}

	namespace literals
	{
		constexpr auto operator""_sm(const char* str, std::size_t size)
		-> decltype(make_matcher(std::basic_string_view{str, size + 1}))
		{
			return make_matcher(std::basic_string_view{str, size + 1});
		}

		constexpr auto operator""_sm(const char8_t* str, std::size_t size)
		-> decltype(make_matcher(std::basic_string_view{str, size + 1}))
		{
			return make_matcher(std::basic_string_view{str, size + 1});
		}

		constexpr auto operator""_sm(const char16_t* str, std::size_t size)
		-> decltype(make_matcher(std::basic_string_view{str, size + 1}))
		{
			return make_matcher(std::basic_string_view{str, size + 1});
		}

		constexpr auto operator""_sm(const char32_t* str, std::size_t size)
		-> decltype(make_matcher(std::basic_string_view{str, size + 1}))
		{
			return make_matcher(std::basic_string_view{str, size + 1});
		}
	}
}
