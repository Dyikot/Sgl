#pragma once

#include "UIElement.h"

namespace Sgl
{
	class Decorator : public UIElement
	{
	public:
		Decorator() = default;

		void SetChild(const Ref<UIElement>& value, ValueSource source = ValueSource::Local);
		const Ref<UIElement>& GetChild() const { return _child; }
		
		void SetPadding(Thickness value, ValueSource source = ValueSource::Local);
		Thickness GetPadding() const { return _padding; }

		static inline StyleableProperty ChildProperty { &SetChild, &GetChild };
		static inline StyleableProperty PaddingProperty { &SetPadding, &GetPadding };
	protected:
		void OnPropertyChanged(PropertyBase& property) override;
		FSize MeasureContent(FSize availableSize) override;
		void ArrangeContent(FRect rect) override;
	private:
		Ref<UIElement> _child;
		Thickness _padding;

		ValueSource _childSource {};
		ValueSource _paddingSource {};
	};
}