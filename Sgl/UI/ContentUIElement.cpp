#include "ContentUIElement.h"
#include <algorithm>
#include "../Base/Math.h"

namespace Sgl
{
	ContentUIElement::ContentUIElement():
		UIElement(),
		ContentTemplate(*this),
		Padding(*this),
		HorizontalContentAlignment(*this, HorizontalAlignment::Left),
		VerticalContentAlignment(*this, VerticalAlignment::Top)
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

	ContentUIElement::ContentUIElement(ContentUIElement&& other) noexcept:
		UIElement(std::move(other)),
		ContentTemplate(std::move(other.ContentTemplate)),
		Padding(std::move(other.Padding)),
		HorizontalContentAlignment(std::move(other.HorizontalContentAlignment)),
		VerticalContentAlignment(std::move(other.VerticalContentAlignment)),
		_content(std::exchange(other._content, nullptr)),
		_contentPresenter(std::move(other._contentPresenter))
	{}

	void ContentUIElement::Render(RenderContext context) const
	{
		if(_contentPresenter && _contentPresenter->IsVisible)
		{
			_contentPresenter->Render(context);
		}

		UIElement::Render(context);
	}

	void ContentUIElement::ApplyStyle()
	{
		StyleableElement::ApplyStyle();

		if(_contentPresenter && _contentPresenter->IsVisible)
		{
			_contentPresenter->ApplyStyle();
		}
	}

	void ContentUIElement::OnMouseMove(const MouseEventArgs& e)
	{
		UIElement::OnMouseMove(e);

		if(_contentPresenter)
		{
			auto& content = *_contentPresenter;
			bool wasMouseOver = content._isMouseOver;
			bool isMouseOver = Math::IsPointInRect(e.Position, content._bounds);

			if(isMouseOver)
			{
				if(!wasMouseOver)
				{
					content.OnMouseEnter(e);
				}

				content.OnMouseMove(e);
			}
			else
			{
				content.OnMouseLeave(e);
				Cursor::Set(Cursor);
			}
		}
	}

	void ContentUIElement::OnMouseDown(const MouseButtonEventArgs& e)
	{
		UIElement::OnMouseDown(e);

		if(_contentPresenter && _contentPresenter->IsMouseOver() && _contentPresenter->IsVisible)
		{
			_contentPresenter->OnMouseDown(e);
		}
	}

	void ContentUIElement::OnMouseUp(const MouseButtonEventArgs& e)
	{
		UIElement::OnMouseUp(e);

		if(_contentPresenter && _contentPresenter->IsMouseOver() && _contentPresenter->IsVisible)
		{
			_contentPresenter->OnMouseUp(e);
		}
	}

	void ContentUIElement::OnMouseLeave(const MouseEventArgs& e)
	{
		UIElement::OnMouseLeave(e);

		if(_contentPresenter && _contentPresenter->IsVisible)
		{
			_contentPresenter->OnMouseLeave(e);
		}
	}

	void ContentUIElement::TryCreatePresenter()
	{
		auto contentTemplate = ContentTemplate.Get();

		if(contentTemplate != nullptr && _content.HasValue())
		{
			_contentPresenter = contentTemplate->Build(_content);
			_contentPresenter->_stylingParent = this;
			_contentPresenter->_layoutableParent = this;
			InvalidateMeasure();
		}
	}

	FSize ContentUIElement::MeasureContent(FSize avaliableSize)
	{
		if(_contentPresenter)
		{
			Thickness padding = Padding;

			FSize contentAvaliableSize =
			{
				.Width = std::clamp<float>(avaliableSize.Width - padding.Left - padding.Right,
					MinWidth,
					MaxWidth),
				.Height = std::clamp<float>(avaliableSize.Height - padding.Top - padding.Bottom,
					MinHeight,
					MaxHeight)
			};

			 _contentPresenter->Measure(contentAvaliableSize);
			 auto contentSize = _contentPresenter->GetDesiredSize();
			 contentSize.Width += padding.Left + padding.Right;
			 contentSize.Height += padding.Top + padding.Bottom;

			 return contentSize;
		}

		return FSize();
	}

	void ContentUIElement::ArrangeContent(FRect rect)
	{
		if(_contentPresenter)
		{
			auto padding = Padding.Get();
			auto availableWidth = rect.w;
			auto availableHeight = rect.h;
			auto desireSize = _contentPresenter->GetDesiredSize();
			auto x = rect.x;
			auto y = rect.y;

			FRect finalRect =
			{
				.x = x + padding.Left,
				.y = y + padding.Top,
				.w = availableWidth - padding.Left - padding.Right,
				.h = availableHeight - padding.Top - padding.Bottom
			};

			if(finalRect.w < 0)
			{
				finalRect.w = 0;
			}

			if(finalRect.h < 0)
			{
				finalRect.h = 0;
			}

			auto verticalContentAlignment = VerticalContentAlignment.Get();
			auto horizontalContentAlignment = HorizontalContentAlignment.Get();

			if(verticalContentAlignment != VerticalAlignment::Top)
			{
				_contentPresenter->VerticalAlignment = verticalContentAlignment;
			}

			if(horizontalContentAlignment != HorizontalAlignment::Left)
			{
				_contentPresenter->HorizontalAlignment = horizontalContentAlignment;		
			}

			_contentPresenter->Arrange(finalRect);
		}
	}
}
