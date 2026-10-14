#pragma once

#include "UIElement.h"

namespace Sgl::UIElements
{
	class Border : public UIElement
	{
	public:
		Border();

		void SetChild(const Ref<UIElement>& value);
		const Ref<UIElement>& GetChild() const { return _child; }

		void SetPadding(Thickness value);
		Thickness GetPadding() const { return _padding; }

		void SetBorderWidth(uint32_t value);
		uint32_t GetBorderWidth() const;

		void SetBorderColor(Color value);
		Color GetBorderColor() const;

		static inline StyleableProperty ChildProperty { &SetChild, &GetChild };
		static inline StyleableProperty PaddingProperty { &SetPadding, &GetPadding };
		static inline StyleableProperty BorderWidthProperty { &SetBorderWidth, &GetBorderWidth };
		static inline StyleableProperty BorderColorProperty { &SetBorderColor, &GetBorderColor };
	protected:
		void OnRender(RenderContext& context) override;
		void OnPropertyChanged(PropertyBase& property) override;
		FSize MeasureContent(FSize availableSize) override;
		void ArrangeContent(FRect rect) override;
	private:
		Ref<UIElement> _child;
		Thickness _padding;
		uint32_t _borderWidth = 1;
		Color _borderColor = Colors::Black;

		ValueSource _childSource {};
		ValueSource _paddingSource {};
		ValueSource _borderWidthSource {};
		ValueSource _borderColorSource {};
	};
}