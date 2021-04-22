#include <gtest/gtest.h>

#include <iostream>
#include <random/random.hpp>
#include <random>
#include <set>

namespace {
	TEST(TEST_RANDOM, random) {
		using namespace gal::toolbox::random;

		{
			std::cout << "\n# 64-bit bound\n";
			std::cout << "min: " << random256PlusPlus::min() << "\nmax: " << random256PlusPlus::max() << std::endl;
		}

		std::cout << "\n# Initialize with a 64-bit seed\n";
		{
			random256PlusPlus engine{};
			for (auto i = 0; i < 10; ++i) {
				std::cout << engine() << '\n';
			}
		}

		std::cout << "\n# Initialize with a 64-bit seed\n";
		{
			random256PlusPlus engine{123456};
			for (auto i = 0; i < 10; ++i) {
				std::cout << engine() << '\n';
			}
		}

		std::cout << "\n# Initialize with a given state\n";
		{
			random256PlusPlus engine{{1, 2, 3, 4}};
			for (auto i = 0; i < 10; ++i) {
				std::cout << engine() << '\n';
			}
		}

		std::cout << "\n# Generate int in the range of [0, 100]\n";
		{
			random256PlusPlus				   engine{123456};
			std::uniform_int_distribution<int> dist(0, 10);
			for (auto i = 0; i < 5; ++i) {
				std::cout << dist(engine) << '\n';
			}
		}

		std::cout << "\n# Test overlap1 (1000000 times) \n";
		{
			for (auto i = 0; i < 10; ++i) {
				random256PlusPlus		engine{};
				std::set<std::uint64_t> set;
				for (auto j = 0; j < 1000000; ++j) {
					set.insert(engine());
				}
				std::cout << set.size() << '\n';
			}
		}

		std::cout << "\n# Test overlap2 (1000000 times) \n";
		{
			for (auto i = 0; i < 10; ++i) {
				random256PlusPlus				   engine{};
				std::uniform_int_distribution<std::uint64_t> dist;
				std::set<std::uint64_t>			   set;
				for (auto j = 0; j < 1000000; ++j) {
					set.insert(dist(engine));
				}
				std::cout << set.size() << '\n';
			}
		}
	}
}// namespace