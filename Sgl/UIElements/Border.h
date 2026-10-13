#pragma once

#include "Decorator.h"

namespace Sgl::UIElements
{
	class Border : public Decorator
	{
	public:
		Border();

		void SetBorderWidth(uint32_t value);
		uint32_t GetBorderWidth() const;

		void SetBorderColor(Color value);
		Color GetBorderColor() const;

		static inline StyleableProperty BorderWidthProperty { &SetBorderWidth, &GetBorderWidth };
		static inline StyleableProperty BorderColorProperty { &SetBorderColor, &GetBorderColor };
	protected:
		void OnRender(RenderContext& context) override;
		void OnPropertyChanged(PropertyBase& property) override;
		FSize MeasureContent(FSize availableSize) override;
		void ArrangeContent(FRect rect) override;
	private:
		uint32_t _borderWidth = 1;
		Color _borderColor = Colors::Black;

		ValueSource _borderWidthSource {};
		ValueSource _borderColorSource {};
	};
}