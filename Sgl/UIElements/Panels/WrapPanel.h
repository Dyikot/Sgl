#pragma once

#include "Panel.h"

namespace Sgl::UIElements
{
	class WrapPanel : public Panel
	{
	public:
		WrapPanel();

		void SetOrientation(Orientation value, ValueSource source = ValueSource::Local);
		Orientation GetOrientation() const { return _orientation; }

		void SetVerticalSpacing(float value, ValueSource source = ValueSource::Local);
		float GetVerticalSpacing() const { return _verticalSpacing; }

		void SetHorizontalSpacing(float value, ValueSource source = ValueSource::Local);
		float GetHorizontalSpacing() const { return _horizontalSpacing; }

		static inline StyleableProperty OrientationProperty { &SetOrientation, &GetOrientation };
		static inline StyleableProperty VeritcalSpacingProperty { &SetVerticalSpacing, &GetVerticalSpacing };
		static inline StyleableProperty HorizontalSpacingProperty { &SetHorizontalSpacing, &GetHorizontalSpacing};
	protected:
		void OnPropertyChanged(PropertyBase& property) override;
		FSize MeasureContent(FSize availableSize) override;
		void ArrangeContent(FRect rect) override;
	private:
		float _verticalSpacing = 0;
		float _horizontalSpacing = 0;
		Orientation _orientation = Orientation::Horizontal;

		ValueSource _verticalSpacingSource {};
		ValueSource _horizontalSpacingSource {};
		ValueSource _orientationSource {};
	};
}