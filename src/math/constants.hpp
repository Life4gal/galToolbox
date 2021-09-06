#pragma once

namespace gal::test::math
{
	template <typename T = double>
	constexpr T two_pi() noexcept
	{
		return static_cast<T>(6.28318530717958647692528676655900576);
	}

	template <typename T = double>
	constexpr T root_pi() noexcept
	{
		return static_cast<T>(1.772453850905516027);
	}

	template <typename T = double>
	constexpr T half_pi() noexcept
	{
		return static_cast<T>(1.57079632679489661923132169163975144);
	}

	template <typename T = double>
	constexpr T three_over_two_pi() noexcept
	{
		return static_cast<T>(4.71238898038468985769396507491925432);
	}

	template <typename T = double>
	constexpr T quarter_pi() noexcept
	{
		return static_cast<T>(0.785398163397448309615660845819875721);
	}

	template <typename T = double>
	constexpr T one_over_pi() noexcept
	{
		return static_cast<T>(0.318309886183790671537767526745028724);
	}

	template <typename T = double>
	constexpr T one_over_two_pi() noexcept
	{
		return static_cast<T>(0.159154943091895335768883763372514362);
	}

	template <typename T = double>
	constexpr T two_over_pi() noexcept
	{
		return static_cast<T>(0.636619772367581343075535053490057448);
	}

	template <typename T = double>
	constexpr T four_over_pi() noexcept
	{
		return static_cast<T>(1.273239544735162686151070106980114898);
	}

	template <typename T = double>
	constexpr T two_over_root_pi() noexcept
	{
		return static_cast<T>(1.12837916709551257389615890312154517);
	}

	template <typename T = double>
	constexpr T one_over_root_two() noexcept
	{
		return static_cast<T>(0.707106781186547524400844362104849039);
	}

	template <typename T = double>
	constexpr T root_half_pi() noexcept
	{
		return static_cast<T>(1.253314137315500251);
	}

	template <typename T = double>
	constexpr T root_two_pi() noexcept
	{
		return static_cast<T>(2.506628274631000502);
	}

	template <typename T = double>
	constexpr T root_ln_four() noexcept
	{
		return static_cast<T>(1.17741002251547469);
	}

	template <typename T = double>
	constexpr T e() noexcept
	{
		return static_cast<T>(2.71828182845904523536);
	}

	template <typename T = double>
	constexpr T euler() noexcept
	{
		return static_cast<T>(0.577215664901532860606);
	}

	template <typename T = double>
	constexpr T root_two() noexcept
	{
		return static_cast<T>(1.41421356237309504880168872420969808);
	}

	template <typename T = double>
	constexpr T root_three() noexcept
	{
		return static_cast<T>(1.73205080756887729352744634150587236);
	}

	template <typename T = double>
	constexpr T root_five() noexcept
	{
		return static_cast<T>(2.23606797749978969640917366873127623);
	}

	template <typename T = double>
	constexpr T ln_two() noexcept
	{
		return static_cast<T>(0.693147180559945309417232121458176568);
	}

	template <typename T = double>
	constexpr T ln_ten() noexcept
	{
		return static_cast<T>(2.30258509299404568401799145468436421);
	}

	template <typename T = double>
	constexpr T ln_ln_two() noexcept
	{
		return static_cast<T>(-0.3665129205816643);
	}

	template <typename T = double>
	constexpr T third() noexcept
	{
		return static_cast<T>(0.3333333333333333333333333333333333333333);
	}

	template <typename T = double>
	constexpr T two_third() noexcept
	{
		return static_cast<T>(0.666666666666666666666666666666666666667);
	}

	template <typename T = double>
	constexpr T golden_ratio() noexcept
	{
		return static_cast<T>(1.61803398874989484820458683436563811);
	}
}
