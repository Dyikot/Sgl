#include "ContentUIElement.h"
#include <algorithm>
#include "../Base/Math.h"

namespace Sgl
{
	ContentUIElement::ContentUIElement(const ContentUIElement& other):
		UIElement(other),
		_content(other._content),
		_contentPresenter(other._contentPresenter),
		_contentTemplate(other._contentTemplate),
		_padding(other._padding),
		_horizontalContentAlignment(other._horizontalContentAlignment),
		_verticalContentAlignment(other._verticalContentAlignment),
		_isContentPresenterValid(other._isContentPresenterValid)
	{}

	ContentUIElement::ContentUIElement(ContentUIElement&& other) noexcept:
		UIElement(std::move(other)),
		_contentTemplate(std::move(other._contentTemplate)),
		_padding(std::move(other._padding)),
		_horizontalContentAlignment(std::move(other._horizontalContentAlignment)),
		_verticalContentAlignment(std::move(other._verticalContentAlignment)),
		_content(std::move(other._content)),
		_contentPresenter(std::move(other._contentPresenter)),
		_isContentPresenterValid(other._isContentPresenterValid)
	{}

	void ContentUIElement::SetContentTemplate(DataTemplate value)
	{
		SetProperty(ContentTemplateProperty, _contentTemplate, value);
		InvalidateMeasure();
		InvalidateContentPresenter();
	}

	void ContentUIElement::SetPadding(Thickness value)
	{
		SetProperty(PaddingProperty, _padding, value);
		InvalidateMeasure();
	}

	void ContentUIElement::SetVerticalContentAlignment(VerticalAlignment value)
	{
		SetProperty(VerticalContentAlignmentProperty, _verticalContentAlignment, value);
		InvalidateArrange();
	}

	void ContentUIElement::SetHorizontalContentAlignment(HorizontalAlignment value)
	{
		SetProperty(HorizontalContentAlignmentProperty, _horizontalContentAlignment, value);
		InvalidateArrange();
	}
		
	void ContentUIElement::Render(RenderContext context)
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

		if(!_isContentPresenterValid && TryCreatePresenter())
		{
			_isContentPresenterValid = true;
		}

		if(_contentPresenter && _contentPresenter->IsVisible())
		{
			_contentPresenter->ApplyStyle();
		}
	}

	void ContentUIElement::OnCursorChanged(const Cursor& cursor)
	{
		if(IsMouseOver() && !(_contentPresenter && _contentPresenter->IsMouseOver()))
		{
			Cursor::Set(cursor);
		}
	}

	void ContentUIElement::OnMouseMove(MouseEventArgs& e)
	{
		UIElement::OnMouseMove(e);

		if(_contentPresenter)
		{
			auto& _content = _contentPresenter.GetValue();
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

	void ContentUIElement::OnMouseDown(MouseButtonEventArgs& e)
	{
		UIElement::OnMouseDown(e);

		if(_contentPresenter && _contentPresenter->IsMouseOver() && _contentPresenter->IsVisible())
		{
			_contentPresenter->OnMouseDown(e);
		}
	}

	void ContentUIElement::OnMouseUp(MouseButtonEventArgs& e)
	{
		UIElement::OnMouseUp(e);

		if(_contentPresenter && _contentPresenter->IsMouseOver() && _contentPresenter->IsVisible())
		{
			_contentPresenter->OnMouseUp(e);
		}
	}

	void ContentUIElement::OnMouseLeave(MouseEventArgs& e)
	{
		UIElement::OnMouseLeave(e);

		if(_contentPresenter && _contentPresenter->IsVisible())
		{
			_contentPresenter->OnMouseLeave(e);
		}
	}

	bool ContentUIElement::TryCreatePresenter()
	{
		if(_contentTemplate.HasTarget() && _content.HasValue())
		{
			_contentPresenter = _contentTemplate(_content);
			_contentPresenter->_parent = this;
			return true;
		}

		return false;
	}

	FSize ContentUIElement::MeasureContent(FSize avaliableSize)
	{
		if(!_isContentPresenterValid && TryCreatePresenter())
		{			
			_isContentPresenterValid = true;
		}

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
			 auto [width, height] = _contentPresenter->GetDesiredSize();

			 return FSize 
			 {
				 .Width = width + _padding.Left + _padding.Right,
				 .Height = height + _padding.Top + _padding.Bottom
			 };
		}

		return FSize();
	}

	void ContentUIElement::ArrangeContent(FRect rect)
	{
		if(_contentPresenter)
		{
			FRect finalRect =
			{
				.x = rect.x + _padding.Left,
				.y = rect.y + _padding.Top,
				.w = rect.w - _padding.Left - _padding.Right,
				.h = rect.h - _padding.Top - _padding.Bottom
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
