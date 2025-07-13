#include "ContentUIElement.h"

namespace Sgl
{
	ContentUIElement::ContentUIElement():
		UIElement(),
		ContentTemplate(*this),
		Padding(),
		HorizontalContentAlignment(HorizontalAlignment::Center),
		VerticalContentAlignment(VerticalAlignment::Center)
	{}

	ContentUIElement::ContentUIElement(const ContentUIElement& other):
		UIElement(other),
		ContentTemplate(other.ContentTemplate),
		Padding(other.Padding),
		HorizontalContentAlignment(other.HorizontalContentAlignment),
		VerticalContentAlignment(other.VerticalContentAlignment),
		_content(other._content),
		_contentPresenter(other._contentPresenter)
	{}

	ContentUIElement::ContentUIElement(ContentUIElement && other) noexcept:
		UIElement(std::move(other)),
		ContentTemplate(std::move(other.ContentTemplate)),
		Padding(std::move(other.Padding)),
		HorizontalContentAlignment(std::move(other.HorizontalContentAlignment)),
		VerticalContentAlignment(std::move(other.VerticalContentAlignment)),
		_content(std::move(other._content)),
		_contentPresenter(std::move(other._contentPresenter))
	{}

	void ContentUIElement::OnRender(RenderContext context) const
	{
		UIElement::OnRender(context);

		if(auto content = _contentPresenter; content != nullptr)
		{
			content->OnRender(context);
		}
	}

	void ContentUIElement::TryCreatePresenter()
	{
		auto contentTemplate = ContentTemplate.Get();

		if(contentTemplate != nullptr && !_content.IsEmpty())
		{
			_contentPresenter = contentTemplate->Build(_content);
		}
	}
}
