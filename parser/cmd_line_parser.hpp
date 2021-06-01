#ifndef GAL_TOOLBOX_CMD_LINE_PARSER_HPP
#define GAL_TOOLBOX_CMD_LINE_PARSER_HPP

#include <exception>
#include <memory>
#include <regex>
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

namespace gal::toolbox::parser {
	namespace detail {
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
		constexpr static bool	   is_container_v = is_container<T>::value;

		constexpr static char_type delimiter	  = ',';
		constexpr static char_type begin		  = '-';

		template<typename T>
		void do_parse(const string& text, T& out);
	}// namespace detail

	/*
	class cmd_line_parser {
	public:
		using string_type	   = detail::string;
		using string_view_type = detail::string_view;

		// we don’t care how many `command_begin` there are, only whether they exist
		constexpr static char command_begin = '-';

		struct cmd {
			using string_type = detail::string;

			string_type				   long_form;
			std::optional<string_type> help_message;
			bool					   required;
			std::optional<string_type> short_form;
		};

		template<typename T, size_t N>
		requires std::is_convertible_v<T, detail::string>
		explicit cmd_line_parser(std::span<T, N> args)
			: program_name(args.front()),
			  not_satisfied_args(0) {
			for (auto it = args.begin() + 1; it != args.end(); ++it) {
				all_commands.emplace_back(it.operator*());
			}
		}

		bool add(
				string_type				   name,
				string_type				   long_form,
				std::optional<string_type> help_message = {},
				bool					   required		= false,
				std::optional<string_type> short_form	= {}) {
			cmd c{
					.long_form	  = std::move(long_form),
					.help_message = std::move(help_message),
					.required	  = required,
					.short_form	  = std::move(short_form)};
			if(required)
			{
				not_satisfied_args += 1;
			}
			return valid_commands.emplace(std::move(name), std::move(c)).second;
		}

		bool parse()
		{
			if(all_commands.size() < not_satisfied_args)
			{
				help("too many command added, unable to meet minimum needs!");
			}

			for(decltype(all_commands)::size_type i = 0; i < all_commands.size();)
			{
				string_view_type curr = all_commands[i];

				// is a command
				if(curr.starts_with(command_begin))
				{

				}

				auto it = curr.find_first_not_of('-');

				GAL_ASSERT(it != curr.size() and "Invalid command format!");


			}
		}

		void help(string_view_type help_reason) {
			cmd_line_parser::cerr << "Error: " << help_reason << '\n';
			cmd_line_parser::cerr << "Usage:\n ./"
								  << program_name << "\n\t";

			for (const auto& [name, cmd]: valid_commands) {
				const auto& [long_form, help_message, required, short_form] = cmd;
				if (!required) {
					cmd_line_parser::cerr << '[';
				}

				cmd_line_parser::cerr << long_form;
				if (short_form.has_value()) {
					cmd_line_parser::cerr << ", " << short_form.value();
				}
				if (!required) {
					cmd_line_parser::cerr << ']';
				}
				if (help_message.has_value()) {
					cmd_line_parser::cerr << '\t' << help_message.value();
				}

				cmd_line_parser::cerr << '\n';
			}

			if (!parsed_commands.empty()) {
				cmd_line_parser::cerr << "\nParsed args:\n\t";

				for (const auto& [name, given]: parsed_commands) {
					cmd_line_parser::cerr << name << " -> " << given << "\n\t";
				}
			}

			cmd_line_parser::cerr << "Given args:\n\t";
			for (const auto& command: all_commands) {
				cmd_line_parser::cerr << command << "\n\t";
			}

			cmd_line_parser::cerr << std::endl;
		}

	private:
		bool is_parsed(const string_type& name) const
		{
			auto it = parsed_commands.find(name);

			if(it == parsed_commands.end() or it->second.empty())
			{
				return false;
			}
			return true;
		}

		string_type															program_name;
		std::vector<string_type>											all_commands;
		std::size_t															not_satisfied_args;

		std::unordered_map<string_type, cmd>								valid_commands;
		std::unordered_map<string_type, string_view_type>					parsed_commands;

		inline static std::basic_ostream<typename string_type::value_type>& cerr = std::cerr;
	};
	*/

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
		constexpr static bool						is_container											   = detail::is_container_v<T>;
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
	class abstract_token : public interface_token<T> {
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

		abstract_token()
			: holding_value(std::make_shared<value_type>()),
			  holding_value_pointer(holding_value.get()),
			  has_default_value_if_not_given(abstract_token<value_type>::is_default_ctor),
			  has_default_value_if_not_specified(false),
			  holding_default_value_if_not_given(),
			  holding_default_value_if_not_specified() {}

		explicit abstract_token(value_type_pointer raw_data)
			: holding_value(nullptr),
			  holding_value_pointer(raw_data),
			  has_default_value_if_not_given(abstract_token<value_type>::is_default_ctor),
			  has_default_value_if_not_specified(false),
			  holding_default_value_if_not_given(),
			  holding_default_value_if_not_specified() {}

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
	class basic_token : public abstract_token<T> {
	public:
		using abstract_token<T>::abstract_token;

		[[nodiscard]] interface_shareable::shared_type clone() const final {
			return std::make_shared<basic_token<T>>(*this);
		}
	};

	template<typename T>
	interface_shareable::shared_type make_token() {
		return std::make_shared<basic_token<T>>();
	}

	template<typename T>
	interface_shareable::shared_type make_token(T& value) {
		return std::make_shared<basic_token<T>>(&value);
	}


	namespace detail {
		constexpr const char_type* integer_pattern = "(-)?(0x|0X)?([0-9a-zA-Z]+)|((0x|0X)?0)";
		constexpr const char_type* truthy_pattern  = "(t|T)(rue)?|1";
		constexpr const char_type* falsy_pattern   = "(f|F)(alse)?|0";

		bool					   get_text_boolean_result(const string& text) {
			  static std::basic_regex<char_type>		 true_regex{truthy_pattern};
			  static std::basic_regex<char_type>		 false_regex{falsy_pattern};
			  std::match_results<string::const_iterator> result;

			  std::regex_match(text, result, true_regex);
			  if (!result.empty()) {
				  return true;
			  }

			  std::regex_match(text, result, false_regex);
			  if (!result.empty()) {
				  return false;
			  }

			  // todo: throw a certain exception
			  throw parser_exception{};
		}

		char get_text_char_result(const string& text) {
			if (text.length() == 1) {
				return text[0];
			}

			// todo: throw a certain exception
			throw parser_exception{};
		}

		struct integer {
			bool   negative;
			bool   hex;
			string value;
		};

		integer get_text_integer_result(const string& text) {
			static std::basic_regex<char_type>		   integer_regex{integer_pattern};

			std::match_results<string::const_iterator> match;
			std::regex_match(text, match, integer_regex);

			if (match.length() == 0) {
				// todo: throw a certain exception
				throw parser_exception{};
			}

			integer ret{
					.negative = match[1].matched,
					.hex	  = match[2].matched,
					.value	  = match[3]};

			if (match.length(4) > 0) {
				ret.hex	  = match[5].matched;
				ret.value = std::to_string(0);
			}

			return ret;
		}

		template<typename To, typename From>
		auto get_text_integer_result(const string& text, bool negative, bool hex) {
			using real_to	= std::remove_cvref_t<To>;
			using real_from = std::remove_cvref_t<From>;
			real_to total	= 0;
			for (auto c: text) {
				real_to curr;
				if (c >= '0' and c <= '9') {
					curr = static_cast<To>(c - '0');
				} else if (hex and c >= 'a' and c <= 'f') {
					curr = static_cast<To>(c - 'a' + 10);
				} else if (hex and c >= 'A' and c <= 'F') {
					curr = static_cast<To>(c - 'A' + 10);
				} else {
					// todo: throw a certain exception
					throw parser_exception{};
				}

				auto next = static_cast<real_to>(total * (hex ? 16 : 10) + curr);
				if (total > next) {
					// todo: throw a certain exception
					throw parser_exception{};
				}
				total = next;
			}

			if constexpr (std::is_signed_v<real_from>) {
				if (negative) {
					if (total > static_cast<real_to>(std::numeric_limits<real_from>::min())) {
						// todo: throw a certain exception
						throw parser_exception{};
					}
				} else {
					if (total > static_cast<real_to>(std::numeric_limits<real_from>::max())) {
						// todo: throw a certain exception
						throw parser_exception{};
					}
				}
			}

			if (negative) {
				return static_cast<real_from>(-static_cast<real_from>(total - 1) - 1);
			} else {
				return static_cast<real_from>(total);
			}
		}

		template<std::floating_point T>
		T get_text_floating_point_result(const string& text) {
			using real_type = std::remove_cvref_t<T>;
			if constexpr (std::is_same_v<real_type, float>) {
				return stof(text, nullptr);
			} else if constexpr (std::is_same_v<real_type, double>) {
				return stod(text, nullptr);
			} else if constexpr (std::is_same_v<real_type, long double>) {
				return stold(text, nullptr);
			} else {
				// todo: throw a certain exception
				throw parser_exception{};
			}
		}

		template<typename T>
		void get_text_container_result(const string& text, std::vector<T>& result) {
			auto it = text.find_first_not_of(begin);

			GAL_ASSERT(it != string::npos and "Invalid command!");

			for (;;) {
				auto next = text.find_first_of(delimiter, it);

				if (next == string::npos) {
					break;
				}

				result.template emplace_back(text.substr(it, next));
				it = next + sizeof(delimiter);
			}

			if (it != text.size()) {
				result.template emplace_back(text.substr(it));
			}
		}

		template<typename T>
		void do_parse(const string& text, T& out) {
			if constexpr (std::is_integral_v<T>) {
				if constexpr (std::is_same_v<T, bool>) {
					out = get_text_boolean_result(text);
					return;
				} else if constexpr (std::is_same_v<T, char>) {
					out = get_text_char_result(text);
					return;
				} else {
					auto result						   = get_text_integer_result(text);

					const auto& [negative, hex, value] = result;
					out								   = get_text_integer_result<std::make_unsigned_t<T>, T>(value, negative, hex);
				}
			} else if constexpr (std::is_floating_point_v<T>) {
				out = get_text_floating_point_result<T>(text);
			} else if constexpr (is_container_v<T>) {
				get_text_container_result(text, out);
			} else {
				out = text;
			}
		}
	}// namespace detail
}// namespace gal::toolbox::parser

#endif//GAL_TOOLBOX_CMD_LINE_PARSER_HPP
