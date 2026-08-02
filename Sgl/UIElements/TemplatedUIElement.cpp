#include "TemplatedUIElement.h"

namespace Sgl
{
	TemplatedUIElement::TemplatedUIElement(TemplatedUIElement&& other) noexcept:
		UIElement(std::move(other)),
		_template(std::move(_template))
	{}

	void TemplatedUIElement::SetTemplate(const Ref<UIElement>& template_)
	{
		if(_template)
		{
			RemoveChild(_template);
		}

		_template = template_;

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

	Styleable& TemplatedUIElement::Template::operator()(Styleable& element) const
	{
		return static_cast<TemplatedUIElement&>(element).GetTemplate().GetValue();
	}
}

