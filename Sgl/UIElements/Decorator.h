#pragma once

#include "UIElement.h"

namespace Sgl
{
	class Decorator : public UIElement
	{
	public:
		struct Child;
	public:
		Decorator() = default;
		Decorator(Decorator&& other) noexcept;

		void SetChild(const Ref<UIElement>& child, ValueSource source = ValueSource::Default);
		const Ref<UIElement>& GetChild() const { return _child; }
		
		void SetPadding(Thickness value, ValueSource source = ValueSource::Local);
		Thickness GetPadding() const { return _padding; }

		static inline StyleableProperty ChildProperty { &SetChild, &GetChild };
		static inline StyleableProperty PaddingProperty { &SetPadding, &GetPadding };
	protected:
		std::span<const Ref<UIElement>> GetVisualChildren() const final;
		FSize MeasureContent(FSize availableSize) override;
		void ArrangeContent(FRect rect) override;
	private:
		Ref<UIElement> _child;
		Thickness _padding;

		ValueSource _childSource {};
		ValueSource _paddingSource {};
	};

	struct Decorator::Child
	{
		StyleableElement& operator()(StyleableElement& element) const;
	};
}