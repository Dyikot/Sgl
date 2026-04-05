#pragma once

#include "Panel.h"

namespace Sgl::UIElements
{
	class StackPanel : public Panel
	{
	public:
		StackPanel() = default;
		StackPanel(const StackPanel&) = delete;
		StackPanel(StackPanel&& other) noexcept;

		void SetSpacing(float value, ValueSource source = ValueSource::Local);
		float GetSpacing() const { return _spacing; }

		void SetOrientation(Orientation value, ValueSource source = ValueSource::Local);
		Orientation GetOrientation() const { return _orientation; }

		static inline StyleableProperty SpacingProperty { &SetSpacing, &GetSpacing };
		static inline StyleableProperty OrientationProperty { &SetOrientation, &GetOrientation };
	protected:
		FSize MeasureContent(FSize avaliableSize) override;
		void ArrangeContent(FRect rect) override;
	private:
		float _spacing = 0;
		Orientation _orientation = Orientation::Vertical;

		ValueSource _spacingSource {};
		ValueSource _orientationSource {};
	};
}