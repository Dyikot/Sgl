#include "Decorator.h"
#include "../Layout/LayoutHelper.h"

namespace Sgl
{
	Decorator::Decorator(Decorator&& other) noexcept:
		UIElement(std::move(other)),
		_child(std::move(_child)),
		_padding(other._padding)
	{}

	void Decorator::SetChild(const Ref<UIElement>& child, ValueSource source)
	{
		if(_childSource > source)
		{
			return;
		}

		if(_child)
		{
			RemoveLogicalChild(_child.Get());
		}

		if(SetProperty(ChildProperty, _child, child, _childSource, source))
		{
			if(_child)
			{
				AddLogicalChild(_child.Get());
			}

			InvalidateMeasure();
		}
	}

	void Decorator::SetPadding(Thickness value, ValueSource source)
	{
		if(SetProperty(PaddingProperty, _padding, value, _paddingSource, source))
		{
			InvalidateMeasure();
		}
	}

	std::span<const Ref<UIElement>> Decorator::GetChildren() const
	{
		if(!_child)
		{
			return {};
		}

		return std::span(&_child, 1);
	}

	FSize Decorator::MeasureContent(FSize availableSize)
	{
		return MeasureChild(_child.Get(), availableSize, _padding);
	}

	void Decorator::ArrangeContent(FRect rect)
	{
		ArrangeChild(_child.Get(), rect, _padding);
	}

	StyleableElement& Decorator_Child::operator()(StyleableElement& element) const
	{
		return static_cast<Decorator&>(element).GetChild().GetValue();
	}
}

