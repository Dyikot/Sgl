#include "ContentUIElement.h"
#include <algorithm>
#include "../Base/Math.h"

namespace Sgl
{
	ContentUIElement::ContentUIElement():
		UIElement(),
		_contentTemplate(),
		_padding(),
		_horizontalContentAlignment(HorizontalAlignmentProperty.DefaultValue),
		_verticalContentAlignment(VerticalContentAlignmentProperty.DefaultValue)
	{}

	ContentUIElement::ContentUIElement(const ContentUIElement& other):
		UIElement(other),
		_contentTemplate(other._contentTemplate),
		_padding(other._padding),
		_horizontalContentAlignment(other._horizontalContentAlignment),
		_verticalContentAlignment(other._verticalContentAlignment),
		_content(other._content),
		_contentPresenter(other._contentPresenter)
	{}

	ContentUIElement::ContentUIElement(ContentUIElement&& other) noexcept:
		UIElement(std::move(other)),
		_contentTemplate(std::move(other._contentTemplate)),
		_padding(std::move(other._padding)),
		_horizontalContentAlignment(std::move(other._horizontalContentAlignment)),
		_verticalContentAlignment(std::move(other._verticalContentAlignment)),
		_content(std::move(other._content)),
		_contentPresenter(std::move(other._contentPresenter))
	{}

	void ContentUIElement::Render(RenderContext context) const
	{
		if(_contentPresenter && _contentPresenter->IsVisible())
		{
			_contentPresenter->Render(context);
		}

		UIElement::Render(context);
	}

	void ContentUIElement::ApplyStyle()
	{
		StyleableElement::ApplyStyle();

		if(_contentPresenter && _contentPresenter->IsVisible())
		{
			_contentPresenter->ApplyStyle();
		}
	}

	void ContentUIElement::OnMouseMove(const MouseEventArgs& e)
	{
		UIElement::OnMouseMove(e);

		if(_contentPresenter)
		{
			auto& _content = *_contentPresenter;
			bool wasMouseOver = _content._isMouseOver;
			bool isMouseOver = Math::IsPointInRect(e.Position, _content._bounds);

			if(isMouseOver)
			{
				if(!wasMouseOver)
				{
					_content.OnMouseEnter(e);
				}

				_content.OnMouseMove(e);
			}
			else if(wasMouseOver)
			{
				_content.OnMouseLeave(e);
				Cursor::Set(GetCursor());
			}
		}
	}

	void ContentUIElement::OnMouseDown(const MouseButtonEventArgs& e)
	{
		UIElement::OnMouseDown(e);

		if(_contentPresenter && _contentPresenter->IsMouseOver() && _contentPresenter->IsVisible())
		{
			_contentPresenter->OnMouseDown(e);
		}
	}

	void ContentUIElement::OnMouseUp(const MouseButtonEventArgs& e)
	{
		UIElement::OnMouseUp(e);

		if(_contentPresenter && _contentPresenter->IsMouseOver() && _contentPresenter->IsVisible())
		{
			_contentPresenter->OnMouseUp(e);
		}
	}

	void ContentUIElement::OnMouseLeave(const MouseEventArgs& e)
	{
		UIElement::OnMouseLeave(e);

		if(_contentPresenter && _contentPresenter->IsVisible())
		{
			_contentPresenter->OnMouseLeave(e);
		}
	}

	void ContentUIElement::TryCreatePresenter()
	{
		if(_contentTemplate && _content.HasValue())
		{
			_contentPresenter = _contentTemplate->Build(_content);
			_contentPresenter->_stylingParent = this;
			_contentPresenter->_layoutableParent = this;
			InvalidateMeasure();
		}
	}

	FSize ContentUIElement::MeasureContent(FSize avaliableSize)
	{
		if(_contentPresenter)
		{
			FSize contentAvaliableSize =
			{
				.Width = std::clamp<float>(avaliableSize.Width - _padding.Left - _padding.Right,
					GetMinWidth(),
					GetMaxWidth()),
				.Height = std::clamp<float>(avaliableSize.Height - _padding.Top - _padding.Bottom,
					GetMinHeight(),
					GetMaxHeight())
			};

			 _contentPresenter->Measure(contentAvaliableSize);
			 auto contentSize = _contentPresenter->GetDesiredSize();
			 contentSize.Width += _padding.Left + _padding.Right;
			 contentSize.Height += _padding.Top + _padding.Bottom;

			 return contentSize;
		}

		return FSize();
	}

	void ContentUIElement::ArrangeContent(FRect rect)
	{
		if(_contentPresenter)
		{
			auto availableWidth = rect.w;
			auto availableHeight = rect.h;
			auto desireSize = _contentPresenter->GetDesiredSize();
			auto x = rect.x;
			auto y = rect.y;

			FRect finalRect =
			{
				.x = x + _padding.Left,
				.y = y + _padding.Top,
				.w = availableWidth - _padding.Left - _padding.Right,
				.h = availableHeight - _padding.Top - _padding.Bottom
			};

			if(finalRect.w < 0)
			{
				finalRect.w = 0;
			}

			if(finalRect.h < 0)
			{
				finalRect.h = 0;
			}

			if(_verticalContentAlignment != VerticalAlignment::Top)
			{
				_contentPresenter->SetVerticalAlignment(_verticalContentAlignment);
			}

			if(_horizontalContentAlignment != HorizontalAlignment::Left)
			{
				_contentPresenter->SetHorizontalAlignment(_horizontalContentAlignment);
			}

			_contentPresenter->Arrange(finalRect);
		}
	}
}
