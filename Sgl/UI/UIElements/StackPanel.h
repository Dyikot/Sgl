#pragma once

#include "../Panel.h"

namespace Sgl
{
	class StackPanel : public Panel
	{
	private:
		float _spacing = 0;
		Orientation _orientation = Orientation::Vertical;
	public:
		StackPanel() = default;
		StackPanel(const StackPanel& other);
		StackPanel(StackPanel&& other) noexcept;
		~StackPanel() = default;

		void SetSpacing(float value);
		float GetSpacing() const { return _spacing; }

		void SetOrientation(Orientation value);
		Orientation GetOrientation() const { return _orientation; }
	protected:
		FSize MeasureContent(FSize avaliableSize) override;
		void ArrangeContent(FRect rect) override;
	public:
		static inline ObservableProperty SpacingProperty { &SetSpacing, &GetSpacing };
		static inline ObservableProperty OrientationProperty { &SetOrientation, &GetOrientation };
	};
}