#include <gtest/gtest.h>

#include <coroutine>

namespace {
	template<typename T>
	class generator {
	public:
		struct promise_type;
		using handle_type = std::coroutine_handle<promise_type>;

		explicit generator(handle_type h) : handle(h) {}

		~generator() {
			if (handle) handle.destroy();
		}

		generator(const generator&) = delete;
		generator& operator=(const generator&) = delete;

		generator(generator&& oth) noexcept : handle(oth.handle) {
			oth.handle = nullptr;
		}
		generator& operator=(generator&& oth) noexcept {
			handle	   = oth.handle;
			oth.handle = nullptr;
			return *this;
		}

		T getValue() {
			return handle.promise().current_value;
		}

		bool next() {
			handle.resume();
			return not handle.done();
		}

	private:
		handle_type handle;

	public:
		struct promise_type {
			promise_type()					  = default;
			promise_type(const promise_type&) = delete;
			promise_type(promise_type&&)	  = delete;
			promise_type& operator=(const promise_type&) = delete;
			promise_type& operator=(promise_type&&) = delete;

			auto		  initial_suspend() noexcept {
				 return std::suspend_always{};
			}
			auto final_suspend() noexcept {
				return std::suspend_always{};
			}
			auto get_return_object() noexcept {
				return generator{handle_type::from_promise(*this)};
			}
			auto return_void() noexcept {
				return std::suspend_never{};
			}

			auto yield_value(const T value) noexcept {
				current_value = value;
				return std::suspend_always{};
			}

			void unhandled_exception() {
				std::exit(1);
			}

			T current_value;
		};
	};

	generator<int> gen_sequence(int start = 0, int step = 1) noexcept {
		auto value = start;
		for (int i = 0;; ++i) {
			co_yield value;
			value += step;
		}
	}

	generator<int> gen_fibonacci(int max) noexcept {
		int day1 = 0;
		int day2 = 1;
		co_yield day1;

		if (day1 < max) {
			do {
				co_yield day2;
				int tmp = day2;
				day2 += day1;
				day1 = tmp;
			} while (day2 <= max);
		}
	}

	TEST(COROUTINES_TEST, YIELD_TEST) {
		std::cout << "gen_sequence(): ";
		auto gen = gen_sequence(100, -5);
		for (auto i = 0; i <= 20; ++i) {
			gen.next();
			std::cout << ' ' << gen.getValue();
		}
		std::cout << std::endl;

		std::cout << "gen_fibonacci(): ";
		auto fib = gen_fibonacci(100000);
		while (fib.next()) {
			std::cout << ' ' << fib.getValue();
		}
		std::cout << std::endl;
	}

	class ignore_return {
	public:
		struct promise_type;

		explicit ignore_return(const promise_type*) {
			std::cout << "ctor of ignore_return called" << std::endl;
		}

		~ignore_return() {
			std::cout << "dtor of ignore_return called" << std::endl;
		}

		struct promise_type {
			promise_type()					  = default;
			promise_type(const promise_type&) = delete;
			promise_type(promise_type&&)	  = delete;
			promise_type& operator=(const promise_type&) = delete;
			promise_type& operator=(promise_type&&) = delete;

			auto		  initial_suspend() noexcept {
				 return std::suspend_never{};
			}
			auto final_suspend() noexcept {
				return std::suspend_never{};
			}
			auto get_return_object() noexcept {
				return this;
			}
			void return_void() noexcept {}

			void unhandled_exception() {
				std::exit(1);
			}
		};
	};

	auto always() -> ignore_return {
		std::cout << "always before await\n";
		co_await std::suspend_always{};
		std::cout << "always after await\n";
	}

	auto never() -> ignore_return {
		std::cout << "never before await\n";
		co_await std::suspend_never{};
		std::cout << "never after await\n";
	}

	TEST(COROUTINES_TEST, AWAIT_TEST_ALWAYS) {
		std::cout << "AWAIT_TEST_ALWAYS before call\n";
		always();
		std::cout << "AWAIT_TEST_ALWAYS after call\n";
	}

	TEST(COROUTINES_TEST, AWAIT_TEST_NEVER) {
		std::cout << "AWAIT_TEST_NEVER before call\n";
		never();
		std::cout << "AWAIT_TEST_NEVER after call\n";
	}
}// namespace
