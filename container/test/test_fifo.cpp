#include <gtest/gtest.h>

#include "../src/fifo.hpp"

using namespace gal::toolbox::container;

TEST(TestFifo, TestPushAndPop)
{
	struct foo
	{
		int a;
		int b;
	};

	fifo<foo, 4> fifo_f16{};

	ASSERT_TRUE(fifo_f16.empty());

	ASSERT_TRUE(fifo_f16.push());
	ASSERT_TRUE(fifo_f16.push(1));
	ASSERT_TRUE(fifo_f16.push(1, 2));
	ASSERT_TRUE(fifo_f16.push());
	ASSERT_EQ(fifo_f16.size(), 4);

	// already full
	ASSERT_FALSE(fifo_f16.push());
	ASSERT_EQ(fifo_f16.size(), 4);

	// we do not know the value of popped foo
	ASSERT_TRUE(fifo_f16.pop());
	ASSERT_EQ(fifo_f16.size(), 3);

	foo foo1{};
	ASSERT_TRUE(fifo_f16.pop(foo1));
	ASSERT_EQ(foo1.a, 1);
	ASSERT_TRUE(fifo_f16.pop(foo1));
	ASSERT_EQ(foo1.a, 1);
	ASSERT_EQ(foo1.b, 2);
	ASSERT_TRUE(fifo_f16.pop());

	ASSERT_FALSE(fifo_f16.pop(1000));
	ASSERT_TRUE(fifo_f16.empty());
}
