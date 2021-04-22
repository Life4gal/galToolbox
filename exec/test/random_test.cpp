#include <gtest/gtest.h>

#include <iostream>
#include <random/random_device.hpp>
#include <random>
#include <set>

namespace {
	TEST(TEST_RANDOM, random) {
		using namespace gal::toolbox::random;

		{
			std::cout << "\n# 64-bit bound\n";
			std::cout << "min: " << xorShiftRotate256PlusPlusEngine::min() << "\nmax: " << xorShiftRotate256PlusPlusEngine::max() << std::endl;
		}

		std::cout << "\n# Initialize with a 64-bit seed\n";
		{
			xorShiftRotate256PlusPlusEngine engine{};
			for (auto i = 0; i < 10; ++i) {
				std::cout << engine() << '\n';
			}
		}

		std::cout << "\n# Initialize with a 64-bit seed\n";
		{
			xorShiftRotate256PlusPlusEngine engine{123456};
			for (auto i = 0; i < 10; ++i) {
				std::cout << engine() << '\n';
			}
		}

		std::cout << "\n# Initialize with a given state\n";
		{
			xorShiftRotate256PlusPlusEngine engine{{1, 2, 3, 4}};
			for (auto i = 0; i < 10; ++i) {
				std::cout << engine() << '\n';
			}
		}

		std::cout << "\n# Generate int in the range of [0, 100]\n";
		{
			xorShiftRotate256PlusPlusEngine	   engine{123456};
			std::uniform_int_distribution<int> dist(0, 10);
			for (auto i = 0; i < 5; ++i) {
				std::cout << dist(engine) << '\n';
			}
		}

		std::cout << "\n# Test overlap1 (100000 times) \n";
		{
			for (auto i = 0; i < 10; ++i) {
				xorShiftRotate256PlusPlusEngine engine{};
				std::set<std::uint64_t>			set;
				for (auto j = 0; j < 100000; ++j) {
					set.insert(engine());
				}
				std::cout << set.size() << '\n';
			}
		}

		std::cout << "\n# Test overlap2 (100000 times) \n";
		{
			for (auto i = 0; i < 10; ++i) {
				xorShiftRotate256PlusPlusEngine												engine{};
				std::uniform_int_distribution<xorShiftRotate256PlusPlusEngine::result_type> dist;
				std::set<xorShiftRotate256PlusPlusEngine::result_type>						set;
				for (auto j = 0; j < 100000; ++j) {
					set.insert(dist(engine));
				}
				std::cout << set.size() << '\n';
			}
		}
	}
}// namespace