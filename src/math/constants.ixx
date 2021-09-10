export module gal.toolbox.math.constants;

namespace gal::toolbox::math
{
	export
	{
		// pi
		template<typename T = double>
		constexpr T pi() noexcept
		{
			return static_cast<T>(3.141'592'653'589'793'238'462'643'383'279'502'88);
		}

		// 2 * pi
		template <typename T = double>
		constexpr T two_pi() noexcept
		{
			return static_cast<T>(6.283'185'307'179'586'476'925'286'766'559'005'76);
		}

		// pi ^ (1/2)
		template <typename T = double>
		constexpr T root_pi() noexcept
		{
			return static_cast<T>(1.772'453'850'905'516'027);
		}

		// pi / 2
		template <typename T = double>
		constexpr T half_pi() noexcept
		{
			return static_cast<T>(1.570'796'326'794'896'619'231'321'691'639'751'44);
		}

		// (3/2) * pi
		template <typename T = double>
		constexpr T three_over_two_pi() noexcept
		{
			return static_cast<T>(4.712'388'980'384'689'857'693'965'074'919'254'32);
		}

		// pi / 4
		template <typename T = double>
		constexpr T quarter_pi() noexcept
		{
			return static_cast<T>(0.785'398'163'397'448'309'615'660'845'819'875'721);
		}

		// 1 / pi
		template <typename T = double>
		constexpr T one_over_pi() noexcept
		{
			return static_cast<T>(0.318'309'886'183'790'671'537'767'526'745'028'724);
		}

		// 1 / (2 * pi)
		template <typename T = double>
		constexpr T one_over_two_pi() noexcept
		{
			return static_cast<T>(0.159'154'943'091'895'335'768'883'763'372'514'362);
		}

		// 2 / pi
		template <typename T = double>
		constexpr T two_over_pi() noexcept
		{
			return static_cast<T>(0.636'619'772'367'581'343'075'535'053'490'057'448);
		}

		// 4 / pi
		template <typename T = double>
		constexpr T four_over_pi() noexcept
		{
			return static_cast<T>(1.273'239'544'735'162'686'151'070'106'980'114'898);
		}

		// 2 / (pi ^ (1/2))
		template <typename T = double>
		constexpr T two_over_root_pi() noexcept
		{
			return static_cast<T>(1.128'379'167'095'512'573'896'158'903'121'545'17);
		}

		// 1 / (2 ^ (1/2))
		template <typename T = double>
		constexpr T one_over_root_two() noexcept
		{
			return static_cast<T>(0.707'106'781'186'547'524'400'844'362'104'849'039);
		}

		// (pi / 2) ^ (1/2)
		template <typename T = double>
		constexpr T root_half_pi() noexcept
		{
			return static_cast<T>(1.253'314'137'315'500'251);
		}

		// (2 * pi) ^ (1/2)
		template <typename T = double>
		constexpr T root_two_pi() noexcept
		{
			return static_cast<T>(2.506'628'274'631'000'502);
		}

		// ln4 ^ (1/2)
		template <typename T = double>
		constexpr T root_ln_four() noexcept
		{
			return static_cast<T>(1.177'410'022'515'474'69);
		}

		// e
		template <typename T = double>
		constexpr T e() noexcept
		{
			return static_cast<T>(2.718'281'828'459'045'235'36);
		}

		// euler
		template <typename T = double>
		constexpr T euler() noexcept
		{
			return static_cast<T>(0.577'215'664'901'532'860'606);
		}

		// 2 ^ (1/2)
		template <typename T = double>
		constexpr T root_two() noexcept
		{
			return static_cast<T>(1.414'213'562'373'095'048'801'688'724'209'698'08);
		}

		// 3 ^ (1/2)
		template <typename T = double>
		constexpr T root_three() noexcept
		{
			return static_cast<T>(1.732'050'807'568'877'293'527'446'341'505'872'36);
		}

		// 5 ^ (1/2)
		template <typename T = double>
		constexpr T root_five() noexcept
		{
			return static_cast<T>(2.236'067'977'499'789'696'409'173'668'731'276'23);
		}

		// ln2
		template <typename T = double>
		constexpr T ln_two() noexcept
		{
			return static_cast<T>(0.693'147'180'559'945'309'417'232'121'458'176'568);
		}

		// ln10
		template <typename T = double>
		constexpr T ln_ten() noexcept
		{
			return static_cast<T>(2.302'585'092'994'045'684'017'991'454'684'364'21);
		}

		// ln(ln2)
		template <typename T = double>
		constexpr T ln_ln_two() noexcept
		{
			return static_cast<T>(-0.366'512'920'581'664'3);
		}

		// 1 / 3
		template <typename T = double>
		constexpr T third() noexcept
		{
			return static_cast<T>(0.333'333'333'333'333'333'333'333'333'333'333'333'333'3);
		}

		// 2 / 3
		template <typename T = double>
		constexpr T two_third() noexcept
		{
			return static_cast<T>(0.666'666'666'666'666'666'666'666'666'666'666'666'667);
		}

		// golden ratio
		template <typename T = double>
		constexpr T golden_ratio() noexcept
		{
			return static_cast<T>(1.618'033'988'749'894'848'204'586'834'365'638'11);
		}
	}
}
