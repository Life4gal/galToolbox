#include <cxxabi.h>
#include <gtest/gtest.h>

#include <iostream>
#include <random/random_device.hpp>
#include <set>

namespace {
	std::string demangle(const char* mangled) {
		int										 status;
		std::unique_ptr<char[], void (*)(void*)> result(
				abi::__cxa_demangle(mangled, nullptr, nullptr, &status),
				std::free);
		return result ? std::string(result.get()) : "error occurred";
	}

	TEST(TEST_RANDOM_DEVICE, WHY_I_CANNOT_USE_FORMAT) {
		using namespace gal::toolbox::random;

		constexpr auto test_times = 1000000;

		std::cout << std::fixed << std::setprecision(3);

		std::cout << "\n# Test overlap1 (" << test_times << " times) \n";
		{
			{
				xorShiftRotate256PlusEngine						   engine{};
				std::set<xorShiftRotate256PlusEngine::result_type> set;
				for (auto j = 0; j < test_times; ++j) {
					set.insert(engine());
				}
				std::cout << "\n# Test " << xorShiftRotate256PlusEngine::bits_of_this << " bits type <" << demangle(typeid(xorShiftRotate256PlusEngine::result_type).name()) << "> overlap(bound: " << xorShiftRotate256PlusEngine::min() << " -> " << xorShiftRotate256PlusEngine::max() << ").\nresult: " << set.size() << '(' << (static_cast<float>(set.size()) / test_times * 100) << "%)" << std::endl;
			}
			{
				xorShiftRotate256PlusPlusEngine						   engine{};
				std::set<xorShiftRotate256PlusPlusEngine::result_type> set;
				for (auto j = 0; j < test_times; ++j) {
					set.insert(engine());
				}
				std::cout << "\n# Test " << xorShiftRotate256PlusPlusEngine::bits_of_this << " bits type <" << demangle(typeid(xorShiftRotate256PlusPlusEngine::result_type).name()) << "> overlap(bound: " << xorShiftRotate256PlusPlusEngine::min() << " -> " << xorShiftRotate256PlusPlusEngine::max() << ").\nresult: " << set.size() << '(' << (static_cast<float>(set.size()) / test_times * 100) << "%)" << std::endl;
			}
			{
				xorShiftRotate256StarStarEngine						   engine{};
				std::set<xorShiftRotate256StarStarEngine::result_type> set;
				for (auto j = 0; j < test_times; ++j) {
					set.insert(engine());
				}
				std::cout << "\n# Test " << xorShiftRotate256StarStarEngine::bits_of_this << " bits type <" << demangle(typeid(xorShiftRotate256StarStarEngine::result_type).name()) << "> overlap(bound: " << xorShiftRotate256StarStarEngine::min() << " -> " << xorShiftRotate256StarStarEngine::max() << ").\nresult: " << set.size() << '(' << (static_cast<float>(set.size()) / test_times * 100) << "%)" << std::endl;
			}
			{
				xorShiftRotate128PlusEngine						   engine{};
				std::set<xorShiftRotate128PlusEngine::result_type> set;
				for (auto j = 0; j < test_times; ++j) {
					set.insert(engine());
				}
				std::cout << "\n# Test " << xorShiftRotate128PlusEngine::bits_of_this << " bits type <" << demangle(typeid(xorShiftRotate128PlusEngine::result_type).name()) << "> overlap(bound: " << xorShiftRotate128PlusEngine::min() << " -> " << xorShiftRotate128PlusEngine::max() << ").\nresult: " << set.size() << '(' << (static_cast<float>(set.size()) / test_times * 100) << "%)" << std::endl;
			}
			{
				xorShiftRotate128PlusPlusEngine						   engine{};
				std::set<xorShiftRotate128PlusPlusEngine::result_type> set;
				for (auto j = 0; j < test_times; ++j) {
					set.insert(engine());
				}
				std::cout << "\n# Test " << xorShiftRotate128PlusPlusEngine::bits_of_this << " bits type <" << demangle(typeid(xorShiftRotate128PlusPlusEngine::result_type).name()) << "> overlap(bound: " << xorShiftRotate128PlusPlusEngine::min() << " -> " << xorShiftRotate128PlusPlusEngine::max() << ").\nresult: " << set.size() << '(' << (static_cast<float>(set.size()) / test_times * 100) << "%)" << std::endl;
			}
			{
				xorShiftRotate128StarStarEngine						   engine{};
				std::set<xorShiftRotate128StarStarEngine::result_type> set;
				for (auto j = 0; j < test_times; ++j) {
					set.insert(engine());
				}
				std::cout << "\n# Test " << xorShiftRotate128StarStarEngine::bits_of_this << " bits type <" << demangle(typeid(xorShiftRotate128StarStarEngine::result_type).name()) << "> overlap(bound: " << xorShiftRotate128StarStarEngine::min() << " -> " << xorShiftRotate128StarStarEngine::max() << ").\nresult: " << set.size() << '(' << (static_cast<float>(set.size()) / test_times * 100) << "%)" << std::endl;
			}
			{
				xorRotateShiftRotate128PlusEngine						 engine{};
				std::set<xorRotateShiftRotate128PlusEngine::result_type> set;
				for (auto j = 0; j < test_times; ++j) {
					set.insert(engine());
				}
				std::cout << "\n# Test " << xorRotateShiftRotate128PlusEngine::bits_of_this << " bits type <" << demangle(typeid(xorRotateShiftRotate128PlusEngine::result_type).name()) << "> overlap(bound: " << xorRotateShiftRotate128PlusEngine::min() << " -> " << xorRotateShiftRotate128PlusEngine::max() << ").\nresult: " << set.size() << '(' << (static_cast<float>(set.size()) / test_times * 100) << "%)" << std::endl;
			}
			{
				xorRotateShiftRotate128PlusPlusEngine						 engine{};
				std::set<xorRotateShiftRotate128PlusPlusEngine::result_type> set;
				for (auto j = 0; j < test_times; ++j) {
					set.insert(engine());
				}
				std::cout << "\n# Test " << xorRotateShiftRotate128PlusPlusEngine::bits_of_this << " bits type <" << demangle(typeid(xorRotateShiftRotate128PlusPlusEngine::result_type).name()) << "> overlap(bound: " << xorRotateShiftRotate128PlusPlusEngine::min() << " -> " << xorRotateShiftRotate128PlusPlusEngine::max() << ").\nresult: " << set.size() << '(' << (static_cast<float>(set.size()) / test_times * 100) << "%)" << std::endl;
			}
			{
				xorRotateShiftRotate128StarStarEngine						 engine{};
				std::set<xorRotateShiftRotate128StarStarEngine::result_type> set;
				for (auto j = 0; j < test_times; ++j) {
					set.insert(engine());
				}
				std::cout << "\n# Test " << xorRotateShiftRotate128StarStarEngine::bits_of_this << " bits type <" << demangle(typeid(xorRotateShiftRotate128StarStarEngine::result_type).name()) << "> overlap(bound: " << xorRotateShiftRotate128StarStarEngine::min() << " -> " << xorRotateShiftRotate128StarStarEngine::max() << ").\nresult: " << set.size() << '(' << (static_cast<float>(set.size()) / test_times * 100) << "%)" << std::endl;
			}
		}

		std::cout << "\n# Test overlap2 (" << test_times << " times) \n";
		{
			for (auto i = 0; i < 5; ++i) {
				xorShiftRotate256PlusPlusEngine												engine{};
				std::uniform_int_distribution<xorShiftRotate256PlusPlusEngine::result_type> dist;
				std::set<xorShiftRotate256PlusPlusEngine::result_type>						set;
				for (auto j = 0; j < test_times; ++j) {
					set.insert(dist(engine));
				}
				std::cout << set.size() << '\n';
			}
		}
	}
}// namespace