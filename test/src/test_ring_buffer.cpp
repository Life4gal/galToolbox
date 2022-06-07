#include <gtest/gtest.h>

#include <galToolbox/container/ring_buffer.hpp>

using namespace gal::toolbox::container;

TEST(TestRingBuffer, TestSetOrOverwrite)
{
	ring_buffer<int, 16> buffer_i161{std::make_index_sequence<3>{}, 1, 2, 3};
	ASSERT_EQ(buffer_i161.size(), static_cast<decltype(buffer_i161.size())>(3));
	ASSERT_EQ(buffer_i161[0], 1);
	ASSERT_EQ(buffer_i161[1], 2);
	ASSERT_EQ(buffer_i161[2], 3);

	ring_buffer<int, 16> buffer_i162{};

	buffer_i162.set_or_overwrite(0, 42);
	buffer_i162.set_or_overwrite(2, 42);
	buffer_i162.set_or_overwrite(4, 42);
	ASSERT_EQ(buffer_i162.size(), static_cast<decltype(buffer_i161.size())>(3));
	ASSERT_EQ(buffer_i162[0], 42);
	ASSERT_EQ(buffer_i162[2], 42);
	ASSERT_EQ(buffer_i162[4], 42);

	buffer_i162.set_or_overwrite(4, 123);
	ASSERT_EQ(buffer_i162[4], 123);
	ASSERT_EQ(buffer_i162.size(), static_cast<decltype(buffer_i161.size())>(3));
}

TEST(TestRingBuffer, TestGetOrSet)
{
	ring_buffer<double, 8> buffer_d8{};

	ASSERT_DOUBLE_EQ(buffer_d8.get_or_set(0, 3.14), 3.14);
	ASSERT_DOUBLE_EQ(buffer_d8.get_or_set(2, 3.14), 3.14);
	ASSERT_DOUBLE_EQ(buffer_d8.get_or_set(4, 3.14), 3.14);

	ASSERT_DOUBLE_EQ(buffer_d8[0], 3.14);
	ASSERT_DOUBLE_EQ(buffer_d8[2], 3.14);
	ASSERT_DOUBLE_EQ(buffer_d8[4], 3.14);
	ASSERT_EQ(buffer_d8.size(), static_cast<decltype(buffer_d8.size())>(3));

	// if the element of the given index_of has not been constructed, the behavior is undefined,
	// and it will not change the size of buffer
	// EXPECT_DOUBLE_EQ(buffer_d8[1], 0.0);
	// ASSERT_EQ(buffer_d8.size(), 3);
}

TEST(TestRingBuffer, TestIndexOfAndDistance)
{
	ring_buffer<char, 32> buffer_c10{};
	constexpr decltype(buffer_c10)::size_type mask = 32 - 1;

	ASSERT_EQ(buffer_c10.index_of(1), 1 bitand mask);
	ASSERT_EQ(buffer_c10.index_of(10), 10 bitand mask);
	ASSERT_EQ(buffer_c10.index_of(100), 100 bitand mask);
	ASSERT_EQ(buffer_c10.index_of(1000), 1000 bitand mask);

	ASSERT_EQ(buffer_c10.distance(1, 10), (10 bitand mask) - (1 bitand mask));
	ASSERT_EQ(buffer_c10.distance(1, 100), (100 bitand mask) - (1 bitand mask));
	ASSERT_EQ(buffer_c10.distance(1, 1000), (1000 bitand mask) - (1 bitand mask));

	ASSERT_EQ(buffer_c10.distance(10, 1), 32 - ((10 bitand mask) - (1 bitand mask)));
	ASSERT_EQ(buffer_c10.distance(100, 1), 32 - ((100 bitand mask) - (1 bitand mask)));
	ASSERT_EQ(buffer_c10.distance(1000, 1), 32 - ((1000 bitand mask) - (1 bitand mask)));
}

TEST(TestRingBuffer, TestCopy)
{
	ring_buffer<int, 16> buffer_i16{};

	buffer_i16.set_or_overwrite(0, 42);
	buffer_i16.set_or_overwrite(2, 42);
	buffer_i16.set_or_overwrite(4, 42);
	ASSERT_EQ(buffer_i16.size(), static_cast<decltype(buffer_i16.size())>(3));
	ASSERT_EQ(buffer_i16[0], 42);
	ASSERT_EQ(buffer_i16[2], 42);
	ASSERT_EQ(buffer_i16[4], 42);

	ring_buffer<double, 32> buffer_d32{buffer_i16};
	ASSERT_EQ(buffer_d32.size(), static_cast<decltype(buffer_i16.size())>(3));
	ASSERT_DOUBLE_EQ(buffer_d32[0], 42);
	ASSERT_DOUBLE_EQ(buffer_d32[2], 42);
	ASSERT_DOUBLE_EQ(buffer_d32[4], 42);

	buffer_d32.set_or_overwrite(1, 3.14);
	buffer_d32.set_or_overwrite(3, 3.14);
	buffer_d32.set_or_overwrite(5, 3.14);
	buffer_d32.set_or_overwrite(27, 3.14);
	buffer_d32.set_or_overwrite(29, 3.14);
	buffer_d32.set_or_overwrite(31, 3.14);
	ASSERT_EQ(buffer_d32.size(), static_cast<decltype(buffer_i16.size())>(9));
	ASSERT_DOUBLE_EQ(buffer_d32[1], 3.14);
	ASSERT_DOUBLE_EQ(buffer_d32[3], 3.14);
	ASSERT_DOUBLE_EQ(buffer_d32[5], 3.14);
	ASSERT_DOUBLE_EQ(buffer_d32[27], 3.14);
	ASSERT_DOUBLE_EQ(buffer_d32[29], 3.14);
	ASSERT_DOUBLE_EQ(buffer_d32[31], 3.14);

	ring_buffer<char, 8> buffer_c8{};
	buffer_c8.set_or_overwrite(0, '0');
	buffer_c8.set_or_overwrite(1, '1');
	buffer_c8.set_or_overwrite(2, '2');
	buffer_c8.set_or_overwrite(3, '3');
	buffer_c8.set_or_overwrite(4, '4');
	buffer_c8.set_or_overwrite(5, '5');
	buffer_c8.set_or_overwrite(6, '6');
	buffer_c8.set_or_overwrite(7, '7');
	ASSERT_EQ(buffer_c8.size(), static_cast<decltype(buffer_c8.size())>(8));
	ASSERT_EQ(buffer_c8[0], '0');
	ASSERT_EQ(buffer_c8[1], '1');
	ASSERT_EQ(buffer_c8[2], '2');
	ASSERT_EQ(buffer_c8[3], '3');
	ASSERT_EQ(buffer_c8[4], '4');
	ASSERT_EQ(buffer_c8[5], '5');
	ASSERT_EQ(buffer_c8[6], '6');
	ASSERT_EQ(buffer_c8[7], '7');

	buffer_c8 = buffer_i16;
	ASSERT_EQ(buffer_c8.size(), static_cast<decltype(buffer_c8.size())>(8));
	ASSERT_EQ(buffer_c8[0], 42);
	ASSERT_EQ(buffer_c8[1], '1');
	ASSERT_EQ(buffer_c8[2], 42);
	ASSERT_EQ(buffer_c8[3], '3');
	ASSERT_EQ(buffer_c8[4], 42);
	ASSERT_EQ(buffer_c8[5], '5');
	ASSERT_EQ(buffer_c8[6], '6');
	ASSERT_EQ(buffer_c8[7], '7');

	buffer_d32 = buffer_c8;
	ASSERT_EQ(buffer_d32.size(), static_cast<decltype(buffer_d32.size())>(11));
	ASSERT_DOUBLE_EQ(buffer_d32[0], 42);
	ASSERT_DOUBLE_EQ(buffer_d32[1], '1');
	ASSERT_DOUBLE_EQ(buffer_d32[2], 42);
	ASSERT_DOUBLE_EQ(buffer_d32[3], '3');
	ASSERT_DOUBLE_EQ(buffer_d32[4], 42);
	ASSERT_DOUBLE_EQ(buffer_d32[5], '5');
	ASSERT_DOUBLE_EQ(buffer_d32[6], '6');
	ASSERT_DOUBLE_EQ(buffer_d32[7], '7');
	ASSERT_DOUBLE_EQ(buffer_d32[27], 3.14);
	ASSERT_DOUBLE_EQ(buffer_d32[29], 3.14);
	ASSERT_DOUBLE_EQ(buffer_d32[31], 3.14);
}

TEST(TestRingBuffer, TestMove)
{
	ring_buffer<int, 16> buffer_i16_1{};

	buffer_i16_1.set_or_overwrite(0, 42);
	buffer_i16_1.set_or_overwrite(2, 42);
	buffer_i16_1.set_or_overwrite(4, 42);
	ASSERT_EQ(buffer_i16_1.size(), static_cast<decltype(buffer_i16_1.size())>(3));
	ASSERT_EQ(buffer_i16_1[0], 42);
	ASSERT_EQ(buffer_i16_1[2], 42);
	ASSERT_EQ(buffer_i16_1[4], 42);

	decltype(buffer_i16_1) buffer_i16_2{std::move(buffer_i16_1)};
	ASSERT_EQ(buffer_i16_1.size(), static_cast<decltype(buffer_i16_1.size())>(0));
	ASSERT_EQ(buffer_i16_2.size(), static_cast<decltype(buffer_i16_2.size())>(3));
	ASSERT_EQ(buffer_i16_2[0], 42);
	ASSERT_EQ(buffer_i16_2[2], 42);
	ASSERT_EQ(buffer_i16_2[4], 42);

	decltype(buffer_i16_1) buffer_i16_3{};
	buffer_i16_3.set_or_overwrite(0, 0);
	buffer_i16_3.set_or_overwrite(1, 1);
	buffer_i16_3.set_or_overwrite(2, 2);
	ASSERT_EQ(buffer_i16_3.size(), static_cast<decltype(buffer_i16_3.size())>(3));
	ASSERT_EQ(buffer_i16_3[0], 0);
	ASSERT_EQ(buffer_i16_3[1], 1);
	ASSERT_EQ(buffer_i16_3[2], 2);

	buffer_i16_3 = std::move(buffer_i16_2);
	ASSERT_EQ(buffer_i16_2.size(), static_cast<decltype(buffer_i16_2.size())>(0));
	ASSERT_EQ(buffer_i16_3.size(), static_cast<decltype(buffer_i16_3.size())>(3));
	ASSERT_EQ(buffer_i16_3[0], 42);
	ASSERT_EQ(buffer_i16_3[2], 42);
	ASSERT_EQ(buffer_i16_3[4], 42);
}

TEST(TestRingBuffer, TestIterator)
{
	ring_buffer<int, 16> buffer_i16_1{};

	buffer_i16_1.set_or_overwrite(0, 41);
	buffer_i16_1.set_or_overwrite(2, 42);
	buffer_i16_1.set_or_overwrite(4, 43);
	ASSERT_EQ(buffer_i16_1.size(), static_cast<decltype(buffer_i16_1.size())>(3));
	ASSERT_EQ(buffer_i16_1[0], 41);
	ASSERT_EQ(buffer_i16_1[2], 42);
	ASSERT_EQ(buffer_i16_1[4], 43);

	auto it = buffer_i16_1.cbegin();
	ASSERT_EQ(*(it + 0), 41);
	ASSERT_EQ(*(it + 2), 42);
	ASSERT_EQ(*(it + 4), 43);
	ASSERT_EQ(it[0], 41);
	ASSERT_EQ(it[2], 42);
	ASSERT_EQ(it[4], 43);
}

TEST(TestRingBuffer, TestEqual)
{
	ring_buffer<int, 16> buffer_i16_1{};
	ring_buffer<long, 16> buffer_l16_1{};
	ring_buffer<long, 16> buffer_l16_2{};

	buffer_i16_1.set_or_overwrite(0, 41);
	buffer_i16_1.set_or_overwrite(2, 42);
	buffer_i16_1.set_or_overwrite(4, 43);
	ASSERT_EQ(buffer_i16_1.size(), static_cast<decltype(buffer_i16_1.size())>(3));
	ASSERT_EQ(buffer_i16_1[0], 41);
	ASSERT_EQ(buffer_i16_1[2], 42);
	ASSERT_EQ(buffer_i16_1[4], 43);

	buffer_l16_1.set_or_overwrite(0, 41);
	buffer_l16_1.set_or_overwrite(2, 42);
	buffer_l16_1.set_or_overwrite(4, 43);
	ASSERT_EQ(buffer_l16_1.size(), static_cast<decltype(buffer_l16_1.size())>(3));
	ASSERT_EQ(buffer_l16_1[0], 41);
	ASSERT_EQ(buffer_l16_1[2], 42);
	ASSERT_EQ(buffer_l16_1[4], 43);

	buffer_l16_2.set_or_overwrite(0, 42);
	buffer_l16_2.set_or_overwrite(2, 42);
	buffer_l16_2.set_or_overwrite(4, 42);
	ASSERT_EQ(buffer_l16_2.size(), static_cast<decltype(buffer_l16_2.size())>(3));
	ASSERT_EQ(buffer_l16_2[0], 42);
	ASSERT_EQ(buffer_l16_2[2], 42);
	ASSERT_EQ(buffer_l16_2[4], 42);

	ASSERT_EQ(buffer_i16_1, buffer_l16_1);
	ASSERT_NE(buffer_l16_1, buffer_l16_2);
}
