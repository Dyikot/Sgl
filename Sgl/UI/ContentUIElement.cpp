#include "ContentUIElement.h"

namespace Sgl
{
	ContentUIElement::ContentUIElement():
		UIElement(),
		ContentTemplate(*this)
	{}

	ContentUIElement::ContentUIElement(const ContentUIElement& other):
		UIElement(other),
		ContentTemplate(other.ContentTemplate),
		_content(other._content),
		_contentPresenter(other._contentPresenter)
	{}

	ContentUIElement::ContentUIElement(ContentUIElement && other) noexcept:
		UIElement(std::move(other)),
		ContentTemplate(std::move(other.ContentTemplate)),
		_content(std::move(other._content)),
		_contentPresenter(std::move(other._contentPresenter))
	{}

	void ContentUIElement::TryCreatePresenter()
	{
		auto contentTemplate = ContentTemplate.Get();

		if(contentTemplate != nullptr && !_content.IsEmpty())
		{
			_contentPresenter = contentTemplate->Build(_content);
		}
	}
}
