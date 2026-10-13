#include "TemplatedUIElement.h"

namespace Sgl
{
	void TemplatedUIElement::SetTemplate(const Ref<UIElement>& value)
	{
		if(_template)
		{
			RemoveChild(_template);
		}

		_template = value;

		if(_template)
		{
			AddChild(_template);
		}

		InvalidateMeasure();
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

