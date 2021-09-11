#include <gtest/gtest.h>

#include <set>
#include <iostream>
#include <random>

#include "../src/random.hpp"

TEST(TestRandom, TestAllEngine)
{
	using namespace gal::toolbox::utils;

	constexpr auto test_times = 10000;

	std::cout << std::fixed << std::setprecision(3);

	std::cout << "\n# Test overlap1 (" << test_times << " times) \n";
	{
		{
			xor_shift_rotate_256_plus_engine                        engine{};
			std::set<xor_shift_rotate_256_plus_engine::result_type> set;
			for (auto j = 0; j < test_times; ++j)
			{
				set.insert(engine());
			}
			std::cout << "\n# Test " << xor_shift_rotate_256_plus_engine::bits_of_this <<
					" bits type <" << typeid(xor_shift_rotate_256_plus_engine::result_type).name() <<
					"> overlap(bound: " << xor_shift_rotate_256_plus_engine::min() << " -> " <<
					xor_shift_rotate_256_plus_engine::max() <<
					").\nresult: " << set.size() << '(' << (static_cast<float>(set.size()) / test_times * 100) << "%)" <<
					std::endl;
		}
		{
			xor_shift_rotate_256_plus_plus_engine                        engine{};
			std::set<xor_shift_rotate_256_plus_plus_engine::result_type> set;
			for (auto j = 0; j < test_times; ++j)
			{
				set.insert(engine());
			}
			std::cout << "\n# Test " << xor_shift_rotate_256_plus_plus_engine::bits_of_this <<
					" bits type <" <<
					typeid(xor_shift_rotate_256_plus_plus_engine::result_type).name() <<
					"> overlap(bound: " << xor_shift_rotate_256_plus_plus_engine::min() << " -> " <<
					xor_shift_rotate_256_plus_plus_engine::max() <<
					").\nresult: " << set.size() << '(' << (static_cast<float>(set.size()) / test_times * 100) << "%)" <<
					std::endl;
		}
		{
			xor_shift_rotate_256_star_star_engine                        engine{};
			std::set<xor_shift_rotate_256_star_star_engine::result_type> set;
			for (auto j = 0; j < test_times; ++j)
			{
				set.insert(engine());
			}
			std::cout << "\n# Test " << xor_shift_rotate_256_star_star_engine::bits_of_this <<
					" bits type <" << typeid(xor_shift_rotate_256_star_star_engine::result_type).name() <<
					"> overlap(bound: " << xor_shift_rotate_256_star_star_engine::min() << " -> " <<
					xor_shift_rotate_256_star_star_engine::max() <<
					").\nresult: " << set.size() << '(' << (static_cast<float>(set.size()) / test_times * 100) << "%)" <<
					std::endl;
		}
		{
			xor_shift_rotate_128_plus_engine                        engine{};
			std::set<xor_shift_rotate_128_plus_engine::result_type> set;
			for (auto j = 0; j < test_times; ++j)
			{
				set.insert(engine());
			}
			std::cout << "\n# Test " << xor_shift_rotate_128_plus_engine::bits_of_this <<
					" bits type <" << typeid(xor_shift_rotate_128_plus_engine::result_type).name() <<
					"> overlap(bound: " << xor_shift_rotate_128_plus_engine::min() << " -> " <<
					xor_shift_rotate_128_plus_engine::max() <<
					").\nresult: " << set.size() << '(' << (static_cast<float>(set.size()) / test_times * 100) << "%)" <<
					std::endl;
		}
		{
			xor_shift_rotate_128_plus_plus_engine                        engine{};
			std::set<xor_shift_rotate_128_plus_plus_engine::result_type> set;
			for (auto j = 0; j < test_times; ++j)
			{
				set.insert(engine());
			}
			std::cout << "\n# Test " << xor_shift_rotate_128_plus_plus_engine::bits_of_this <<
					" bits type <" <<
					typeid(xor_shift_rotate_128_plus_plus_engine::result_type).name() <<
					"> overlap(bound: " << xor_shift_rotate_128_plus_plus_engine::min() << " -> " <<
					xor_shift_rotate_128_plus_plus_engine::max() <<
					").\nresult: " << set.size() << '(' << (static_cast<float>(set.size()) / test_times * 100) << "%)" <<
					std::endl;
		}
		{
			xor_shift_rotate_128_star_star_engine                        engine{};
			std::set<xor_shift_rotate_128_star_star_engine::result_type> set;
			for (auto j = 0; j < test_times; ++j)
			{
				set.insert(engine());
			}
			std::cout << "\n# Test " << xor_shift_rotate_128_star_star_engine::bits_of_this <<
					" bits type <" << typeid(xor_shift_rotate_128_star_star_engine::result_type).name() <<
					"> overlap(bound: " << xor_shift_rotate_128_star_star_engine::min() << " -> " <<
					xor_shift_rotate_128_star_star_engine::max() <<
					").\nresult: " << set.size() << '(' << (static_cast<float>(set.size()) / test_times * 100) << "%)" <<
					std::endl;
		}
		{
			xor_rotate_shift_rotate_128_plus_engine                        engine{};
			std::set<xor_rotate_shift_rotate_128_plus_engine::result_type> set;
			for (auto j = 0; j < test_times; ++j)
			{
				set.insert(engine());
			}
			std::cout << "\n# Test " << xor_rotate_shift_rotate_128_plus_engine::bits_of_this <<
					" bits type <" << typeid(xor_rotate_shift_rotate_128_plus_engine::result_type).name() <<
					"> overlap(bound: " << xor_rotate_shift_rotate_128_plus_engine::min() << " -> " <<
					xor_rotate_shift_rotate_128_plus_engine::max() <<
					").\nresult: " << set.size() << '(' << (static_cast<float>(set.size()) / test_times * 100) << "%)" <<
					std::endl;
		}
		{
			xor_rotate_shift_rotate_128_plus_plus_engine                        engine{};
			std::set<xor_rotate_shift_rotate_128_plus_plus_engine::result_type> set;
			for (auto j = 0; j < test_times; ++j)
			{
				set.insert(engine());
			}
			std::cout << "\n# Test " << xor_rotate_shift_rotate_128_plus_plus_engine::bits_of_this <<
					" bits type <" << typeid(xor_rotate_shift_rotate_128_plus_plus_engine::result_type).name() <<
					"> overlap(bound: " << xor_rotate_shift_rotate_128_plus_plus_engine::min() << " -> " <<
					xor_rotate_shift_rotate_128_plus_plus_engine::max() <<
					").\nresult: " << set.size() << '(' << (static_cast<float>(set.size()) / test_times * 100) << "%)" <<
					std::endl;
		}
		{
			xor_rotate_shift_rotate_128_star_star_engine                        engine{};
			std::set<xor_rotate_shift_rotate_128_star_star_engine::result_type> set;
			for (auto j = 0; j < test_times; ++j)
			{
				set.insert(engine());
			}
			std::cout << "\n# Test " << xor_rotate_shift_rotate_128_star_star_engine::bits_of_this <<
					" bits type <" << typeid(xor_rotate_shift_rotate_128_star_star_engine::result_type).name() <<
					"> overlap(bound: " << xor_rotate_shift_rotate_128_star_star_engine::min() << " -> " <<
					xor_rotate_shift_rotate_128_star_star_engine::max() <<
					").\nresult: " << set.size() << '(' << (static_cast<float>(set.size()) / test_times * 100) << "%)" <<
					std::endl;
		}
	}

	std::cout << "\n# Test overlap2 (" << test_times << " times) \n";
	{
		for (auto i = 0; i < 5; ++i)
		{
			xor_shift_rotate_256_plus_plus_engine                                             engine{};
			std::uniform_int_distribution<xor_shift_rotate_256_plus_plus_engine::result_type> dist;
			std::set<xor_shift_rotate_256_plus_plus_engine::result_type>                      set;
			for (auto j = 0; j < test_times; ++j)
			{
				set.insert(dist(engine));
			}
			std::cout << set.size() << '\n';
		}
	}
}
