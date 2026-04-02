#pragma once

#include "Panel.h"

namespace Sgl::UIElements
{
	class WrapPanel : public Panel
	{
	public:
		WrapPanel() = default;
		WrapPanel(const WrapPanel& other);
		WrapPanel(WrapPanel&& other) noexcept;
		~WrapPanel() = default;

		void SetVerticalSpacing(float value, ValueSource source = ValueSource::Local);
		float GetVerticalSpacing() const { return _verticalSpacing; }

		void SetHorizontalSpacing(float value, ValueSource source = ValueSource::Local);
		float GetHorizontalSpacing() const { return _horizontalSpacing; }

		void SetOrientation(Orientation value, ValueSource source = ValueSource::Local);
		Orientation GetOrientation() const { return _orientation; }

		static inline StyleableProperty VeritcalSpacingProperty { &SetVerticalSpacing, &GetVerticalSpacing };
		static inline StyleableProperty HorizontalSpacingProperty { &SetHorizontalSpacing, &GetHorizontalSpacing};
		static inline StyleableProperty OrientationProperty { &SetOrientation, &GetOrientation };
	protected:
		FSize MeasureContent(FSize avaliableSize) override;
		void ArrangeContent(FRect rect) override;
	private:
		float _verticalSpacing = 0;
		float _horizontalSpacing = 0;
		Orientation _orientation = Orientation::Vertical;

		ValueSource _verticalSpacingSource {};
		ValueSource _horizontalSpacingSource {};
		ValueSource _orientationSource {};
	};
}