#include <gtest/gtest.h>

#include "../src/math/vector3.hpp"
#include "../src/math/vector2.hpp"
#include "../src/math/vector1.hpp"

TEST(TestVector3, TestBasic)
{
	using namespace gal::test;

	static_assert(vector3<int>::data_size == 3);
	static_assert(vector3<int>::data_index == 2);
	// static_assert(vector3<int>::size() == 3);
}
