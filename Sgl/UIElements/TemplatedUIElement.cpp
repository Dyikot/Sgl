#include "TemplatedUIElement.h"

namespace Sgl
{
	TemplatedUIElement::TemplatedUIElement(TemplatedUIElement&& other) noexcept:
		UIElement(std::move(other)),
		_template(std::move(_template))
	{}

	void TemplatedUIElement::SetTemplate(const Ref<UIElement>& atemplate)
	{
		if(_template)
		{
			RemoveLogicalChild(_template.Get());
		}

		_template = atemplate;

		if(_template)
		{
			AddLogicalChild(_template.Get());
		}

		InvalidateMeasure();
	}

	std::span<const Ref<UIElement>> TemplatedUIElement::GetChildren() const
	{
		if(!_template)
		{
			return {};
		}

		return std::span(&_template, 1);
	}

	FSize TemplatedUIElement::MeasureContent(FSize availableSize)
	{
		if(!_template)
		{
			return {};
		}

		_template->Measure(availableSize);
		return _template->GetDesiredSize();
	}

	void TemplatedUIElement::ArrangeContent(FRect rect)
	{
		if(_template)
		{
			_template->Arrange(rect);
		}
	}
}

