#include "Decorator.h"
#include "../Layout/LayoutHelper.h"

namespace Sgl
{
	void Decorator::SetChild(const Ref<UIElement>& value)
	{
		auto source = GetValueSource();

		if(_childSource > source)
		{
			return;
		}

		if(_child == value)
		{
			_childSource = source;
			return;
		}

		if(_child)
		{
			RemoveChild(_child);
		}

		_child = value;
		_childSource = source;

		if(_child)
		{
			AddChild(_child);
		}

		OnPropertyChanged(ChildProperty);
	}

	void Decorator::SetPadding(Thickness value)
	{
		SetProperty(PaddingProperty, _padding, value, _paddingSource);
	}

	void Decorator::OnPropertyChanged(PropertyBase& property)
	{
		UIElement::OnPropertyChanged(property);

		if(property == ChildProperty || property == PaddingProperty)
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
}

