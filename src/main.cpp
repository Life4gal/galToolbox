import std.core;
import gal.toolbox.assert;

constexpr int test()
{
	constexpr auto i = 42;
	gal::toolbox::assert(i == 42);
	return i;
}

int main()
{
	std::cout << "Hello World!" << std::endl;

	gal::toolbox::assert_set_debug_open();
	constexpr auto i = test();
	gal::toolbox::assert(i == 43);
}
