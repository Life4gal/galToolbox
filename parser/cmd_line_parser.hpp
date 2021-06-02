#ifndef GAL_TOOLBOX_CMD_LINE_PARSER_HPP
#define GAL_TOOLBOX_CMD_LINE_PARSER_HPP

#include <exception>
#include <memory>
#include <parser/string_parser.hpp>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <vector>

#if !defined(GAL_ASSERT)
#if __has_include(<cassert>)
#define GAL_ASSERT assert
#else
#define GAL_ASSERT
#endif
#endif

namespace gal::toolbox::parser::cmd_line_parser {
	inline namespace detail {
		using char_type	  = char;

		using string	  = std::basic_string<char_type>;
		using string_view = std::basic_string_view<char_type>;

		template<typename T>
		struct is_container {
			static constexpr bool value = false;
		};

		template<typename T>
		struct is_container<std::vector<T>> {
			static constexpr bool value = true;
		};

		template<typename T>
		constexpr static bool is_container_v = is_container<T>::value;
	}// namespace detail

	class parser_exception : std::exception {
	public:
		[[nodiscard]] const char* what() const noexcept override {
			return "parser exception";
		}
	};

	class interface_shareable : std::enable_shared_from_this<interface_shareable> {
	public:
		using shared_type				  = std::shared_ptr<interface_shareable>;
		using const_shared_type			  = std::shared_ptr<const interface_shareable>;

		virtual ~interface_shareable()	  = default;

		virtual shared_type clone() const = 0;
	};

	template<typename T>
	class interface_token : public interface_shareable {
	public:
		using string																						   = detail::string;
		using string_view																					   = detail::string_view;

		constexpr static bool						is_default_ctor											   = std::is_default_constructible_v<T>;
		constexpr static bool						is_container											   = is_container_v<T>;
		constexpr static bool						is_boolean												   = std::is_same_v<T, bool>;

		virtual void								parse() const											   = 0;
		virtual void								parse(const string&)									   = 0;

		// user use the command --command_name but not given a certain arg
		[[nodiscard]] constexpr virtual bool		has_default_value_if_not_given_value() const noexcept	   = 0;
		// user even not use the command
		[[nodiscard]] constexpr virtual bool		has_default_value_if_not_specified_value() const noexcept  = 0;

		[[nodiscard]] constexpr virtual string_view get_default_value_if_not_given() const					   = 0;
		[[nodiscard]] constexpr virtual string_view get_default_value_if_not_specified() const				   = 0;
		constexpr virtual void						set_default_value_if_not_given(string value)			   = 0;
		constexpr virtual void						set_default_value_if_not_specified(string value)		   = 0;
		[[nodiscard]] virtual shared_type			get_after_set_default_value_if_not_given(string value)	   = 0;
		[[nodiscard]] virtual shared_type			get_after_set_default_value_if_not_specified(string value) = 0;
	};

	template<typename T>
	requires std::is_default_constructible_v<T>
	class basic_token : public interface_token<T> {
	public:
		using value_type				 = T;
		using value_type_pointer		 = T*;
		using value_type_const_pointer	 = const T*;
		using value_type_reference		 = T&;
		using value_type_const_reference = const T&;
		using shared_value_type			 = std::shared_ptr<value_type>;
		using shared_const_value_type	 = std::shared_ptr<const value_type>;

		using shared_self_type			 = interface_shareable::shared_type;
		using shared_const_self_type	 = interface_shareable::const_shared_type;

		using string					 = typename interface_token<value_type>::string;
		using string_view				 = typename interface_token<value_type>::string_view;

		basic_token()
			: holding_value(std::make_shared<value_type>()),
			  holding_value_pointer(holding_value.get()),
			  has_default_value_if_not_given(basic_token<value_type>::is_default_ctor),
			  has_default_value_if_not_specified(false),
			  holding_default_value_if_not_given(),
			  holding_default_value_if_not_specified() {}

		explicit basic_token(value_type_pointer raw_data)
			: holding_value(nullptr),
			  holding_value_pointer(raw_data),
			  has_default_value_if_not_given(basic_token<value_type>::is_default_ctor),
			  has_default_value_if_not_specified(false),
			  holding_default_value_if_not_given(),
			  holding_default_value_if_not_specified() {}

		[[nodiscard]] interface_shareable::shared_type clone() const {
			return std::make_shared<basic_token<T>>(*this);
		}

		void parse() const override {
			// todo
		}

		void parse(const string& text) const override {
			// todo
		}

		[[nodiscard]] constexpr bool has_default_value_if_not_given_value() const noexcept final {
			return has_default_value_if_not_given;
		}

		[[nodiscard]] constexpr bool has_default_value_if_not_specified_value() const noexcept final {
			return has_default_value_if_not_specified;
		}

		[[nodiscard]] constexpr string_view get_default_value_if_not_given() const final {
			return holding_default_value_if_not_given;
		}
		[[nodiscard]] constexpr string_view get_default_value_if_not_specified() const final {
			return holding_default_value_if_not_specified;
		}
		constexpr void set_default_value_if_not_given(string value) final {
			has_default_value_if_not_given	   = true;
			holding_default_value_if_not_given = std::move(value);
		}
		constexpr void set_default_value_if_not_specified(string value) final {
			has_default_value_if_not_specified	   = true;
			holding_default_value_if_not_specified = std::move(value);
		}
		[[nodiscard]] shared_self_type get_after_set_default_value_if_not_given(string value) final {
			set_default_value_if_not_given(std::move(value));
			return interface_shareable::shared_from_this();
		}
		[[nodiscard]] shared_self_type get_after_set_default_value_if_not_specified(string value) final {
			set_default_value_if_not_specified(std::move(value));
			return interface_shareable::shared_from_this();
		}

		value_type_const_reference get() const {
			if (holding_value_pointer == nullptr) {
				return holding_value.operator*();
			}
			return *holding_value_pointer;
		}

	protected:
		shared_value_type  holding_value;
		value_type_pointer holding_value_pointer;

		bool			   has_default_value_if_not_given;
		bool			   has_default_value_if_not_specified;

		string			   holding_default_value_if_not_given;
		string			   holding_default_value_if_not_specified;
	};

	template<typename T>
	interface_shareable::shared_type make_token() {
		return std::make_shared<basic_token<T>>();
	}

	template<typename T>
	interface_shareable::shared_type make_token(T& value) {
		return std::make_shared<basic_token<T>>(&value);
	}
}// namespace gal::toolbox::parser::cmd_line_parser

#endif//GAL_TOOLBOX_CMD_LINE_PARSER_HPP
