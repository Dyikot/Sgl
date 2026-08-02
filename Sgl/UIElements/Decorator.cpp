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
			RemoveChild(_child);
		}

		if(SetProperty(ChildProperty, _child, child, _childSource, source))
		{
			if(_child)
			{
				AddChild(_child);
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

	FSize Decorator::MeasureContent(FSize availableSize)
	{
		return MeasureChild(_child.Get(), availableSize, _padding);
	}

	void Decorator::ArrangeContent(FRect rect)
	{
		ArrangeChild(_child.Get(), rect, _padding);
	}

	Styleable& Decorator::Child::operator()(Styleable& element) const
	{
		return static_cast<Decorator&>(element).GetChild().GetValue();
	}
}

