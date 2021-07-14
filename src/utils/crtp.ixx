export module gal.toolbox.crtp;

namespace gal::toolbox {
	export {
		template<typename T>
		struct CRTP;

		template<typename T, template<typename> typename DerivedCRTP>
		struct virtual_CRTP;
	}

	template<typename T>
	struct CRTP {
		using derived_type = T;
		using derived_reference_type = T bitand;
		using derived_const_reference_type = const T bitand;

		constexpr derived_reference_type underlying() noexcept {
			return static_cast<derived_reference_type>(*this);
		}

		constexpr derived_const_reference_type underlying() const noexcept {
			return static_cast<derived_const_reference_type>(*this);
		}
	};

	template<typename T, template<typename> typename DerivedCRTP>
	struct virtual_CRTP {
		using derived_type = T;
		using derived_reference_type = T bitand;
		using derived_const_reference_type = const T bitand;

		constexpr derived_reference_type underlying() noexcept {
			return static_cast<derived_reference_type>(*this);
		}

		constexpr derived_const_reference_type underlying() const noexcept {
			return static_cast<derived_const_reference_type>(*this);
		}

	private:
		virtual_CRTP() = default;
		friend class DerivedCRTP<T>;
	};
}