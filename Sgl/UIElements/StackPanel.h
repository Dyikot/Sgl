#pragma once

#include "../UIElement/Panel.h"

namespace Sgl
{
	class StackPanel : public Panel
	{
	private:
		float _spacing = 0;
		Orientation _orientation = Orientation::Vertical;

		ValueSource _spacingSource {};
		ValueSource _orientationSource {};
	public:
		StackPanel() = default;
		StackPanel(const StackPanel& other);
		StackPanel(StackPanel&& other) noexcept;
		~StackPanel() = default;

		void SetSpacing(float value, ValueSource source = ValueSource::Local);
		float GetSpacing() const { return _spacing; }

		void SetOrientation(Orientation value, ValueSource source = ValueSource::Local);
		Orientation GetOrientation() const { return _orientation; }
	protected:
		FSize MeasureContent(FSize avaliableSize) override;
		void ArrangeContent(FRect rect) override;
	public:
		static inline StyleableProperty SpacingProperty { &SetSpacing, &GetSpacing };
		static inline StyleableProperty OrientationProperty { &SetOrientation, &GetOrientation };
	};
}