#include "ContentUIElement.h"
#include <algorithm>
#include "../Layout/LayoutHelper.h"

namespace Sgl
{
	ContentUIElement::ContentUIElement(const ContentUIElement& other):
		UIElement(other),
		_content(other._content),
		_presenter(other._presenter),
		_contentTemplate(other._contentTemplate),
		_padding(other._padding),
		_horizontalContentAlignment(other._horizontalContentAlignment),
		_verticalContentAlignment(other._verticalContentAlignment),
		_isContentPresenterValid(other._isContentPresenterValid)
	{}

	ContentUIElement::ContentUIElement(ContentUIElement&& other) noexcept:
		UIElement(std::move(other)),
		_content(std::move(other._content)),
		_contentTemplate(std::move(other._contentTemplate)),
		_padding(std::move(other._padding)),
		_horizontalContentAlignment(std::move(other._horizontalContentAlignment)),
		_verticalContentAlignment(std::move(other._verticalContentAlignment)),
		_presenter(std::move(other._presenter)),
		_isContentPresenterValid(other._isContentPresenterValid)
	{}

	ContentUIElement::~ContentUIElement()
	{
		if(_presenter)
		{
			OnContentPresenterDestroying(_presenter.GetValue());
		}
	}

	void ContentUIElement::SetContent(Ref<ObservableObject> content)
	{
		if(SetProperty(ContentProperty, _content, content))
		{
			if(!content.Is<UIElement>())
			{
				SetDataContext(content);
			}			
			
			InvalidateContentPresenter();
		}
	}

	void ContentUIElement::SetContentTemplate(Ref<IDataTemplate> value)
	{
		if(SetProperty(ContentTemplateProperty, _contentTemplate, value))
		{
			InvalidateContentPresenter();
		}
	}

	void ContentUIElement::SetPadding(Thickness value)
	{
		if(SetProperty(PaddingProperty, _padding, value))
		{
			InvalidateMeasure();
		}
	}

	void ContentUIElement::SetVerticalContentAlignment(VerticalAlignment value)
	{
		if(SetProperty(VerticalContentAlignmentProperty, _verticalContentAlignment, value))
		{
			InvalidateArrange();
		}
	}

	void ContentUIElement::SetHorizontalContentAlignment(HorizontalAlignment value)
	{
		if(SetProperty(HorizontalContentAlignmentProperty, _horizontalContentAlignment, value))
		{
			InvalidateArrange();
		}
	}
		
	void ContentUIElement::SetVisualRoot(IVisualRoot* value)
	{
		Renderable::SetVisualRoot(value);

		if(_presenter)
		{
			_presenter->SetVisualRoot(value);
		}
	}

	void ContentUIElement::Render(RenderContext context)
	{
		if(_presenter && _presenter->IsVisible())
		{
			_presenter->Render(context);
		}

		UIElement::Render(context);
	}

	void ContentUIElement::ApplyStyle()
	{
		StyleableElement::ApplyStyle();

		if(_presenter)
		{
			_presenter->ApplyStyle();
		}
	}

	void ContentUIElement::OnContentPresenterCreated(UIElement& presenter)
	{
		presenter.OnAttachedToLogicalTree(*this);		
	}

	void ContentUIElement::OnContentPresenterDestroying(UIElement& presenter)
	{
		presenter.OnDetachedFromLogicalTree();
	}

	void ContentUIElement::OnCursorChanged(const Cursor& cursor)
	{
		if(IsMouseOver() && !(_presenter && _presenter->IsMouseOver()))
		{
			Cursor::Set(cursor);
		}
	}

	void ContentUIElement::OnMouseMove(MouseMoveEventArgs e)
	{
		UIElement::OnMouseMove(e);

		if(_presenter)
		{
			bool wasMouseOver = _presenter->_isMouseOver;
			bool isMouseOver = LayoutHelper::IsPointInRect(e.X, e.Y, _presenter->_bounds);

			if(isMouseOver)
			{
				if(!wasMouseOver)
				{
					_presenter->OnMouseEnter(e);
				}

				_presenter->OnMouseMove(e);
			}
			else if(wasMouseOver)
			{
				_presenter->OnMouseLeave(e);
				Cursor::Set(GetCursor());
			}
		}
	}

	void ContentUIElement::OnMouseDown(MouseButtonEventArgs e)
	{
		UIElement::OnMouseDown(e);

		if(_presenter && _presenter->IsMouseOver() && _presenter->IsVisible())
		{
			_presenter->OnMouseDown(e);
		}
	}

	void ContentUIElement::OnMouseUp(MouseButtonEventArgs e)
	{
		UIElement::OnMouseUp(e);

		if(_presenter && _presenter->IsMouseOver() && _presenter->IsVisible())
		{
			_presenter->OnMouseUp(e);
		}
	}

	void ContentUIElement::OnMouseLeave(MouseMoveEventArgs e)
	{
		UIElement::OnMouseLeave(e);

		if(_presenter && _presenter->IsVisible())
		{
			_presenter->OnMouseLeave(e);
		}
	}

	void ContentUIElement::OnUpdate()
	{
		UIElement::OnUpdate();

		if(!_isContentPresenterValid)
		{
			UpdatePresenter();
		}
	}

	void ContentUIElement::InvalidateContentPresenter()
	{
		RequestUpdate();
		InvalidateMeasure();

		_isContentPresenterValid = false;
	}	

	FSize ContentUIElement::MeasureContent(FSize avaliableSize)
	{
		if(_presenter)
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

			 _presenter->Measure(contentAvaliableSize);
			 auto [width, height] = _presenter->GetDesiredSize();

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
		if(_presenter)
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
				_presenter->SetVerticalAlignment(_verticalContentAlignment);
			}

			if(_horizontalContentAlignment != HorizontalAlignment::Left)
			{
				_presenter->SetHorizontalAlignment(_horizontalContentAlignment);
			}

			_presenter->Arrange(finalRect);
		}
	}

	bool ContentUIElement::UpdatePresenter()
	{
		if(_contentTemplate && _contentTemplate->Match(_content))
		{
			if(_presenter)
			{
				OnContentPresenterDestroying(_presenter.GetValue());
			}

			_presenter = _contentTemplate->Build(_content);
			_isContentPresenterValid = true;

			if(_presenter)
			{
				OnContentPresenterCreated(_presenter.GetValue());
			}

			return true;
		}

		return false;
	}
}
