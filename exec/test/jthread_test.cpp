#include <gtest/gtest.h>

#include <chrono>
#include <thread>

using namespace ::std::literals;

namespace {
	TEST(JTHREAD_TEST, BASIC) {
		int			 v		= 0;
		std::jthread thread = std::jthread([](const std::stop_token& token, int& value) {
			while (true) {
				std::this_thread::sleep_for(0.1s);
				if (token.stop_requested()) {
					break;
				}
				std::cout << "value = " << value << std::endl;
				++value;
			}
		}, std::ref(v));

		std::this_thread::sleep_for(1s);

		std::cerr << "Main thread interrupts jthreads" << std::endl;

		thread.request_stop();
	}

	TEST(JTHREAD_TEST, INTERRUPTABLE) {
		std::jthread not_interruptable([] {
			int counter = 0;
			while (counter < 10) {
				std::this_thread::sleep_for(0.2s);
				std::cerr << "not_interruptable: " << counter << std::endl;
				++counter;
			}
		});

		std::jthread interruptable([](const std::stop_token& token) {
			int counter = 0;
			while (counter < 10) {
				std::this_thread::sleep_for(0.2s);
				if (token.stop_requested()) {
					return;
				}
				std::cerr << "interruptable: " << counter << std::endl;
				++counter;
			}
		});

		std::this_thread::sleep_for(1s);

		std::cerr << "Main thread interrupts both jthreads" << std::endl;
		not_interruptable.request_stop();
		interruptable.request_stop();

		std::cerr << "Main thread interrupts both jthreads done" << std::endl;
	}
}// namespace
