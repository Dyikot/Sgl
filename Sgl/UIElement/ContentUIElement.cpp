#include "ContentUIElement.h"
#include <algorithm>
#include "../Layout/LayoutHelper.h"

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
		_content(std::move(other._content)),
		_contentTemplate(std::move(other._contentTemplate)),
		_padding(std::move(other._padding)),
		_horizontalContentAlignment(std::move(other._horizontalContentAlignment)),
		_verticalContentAlignment(std::move(other._verticalContentAlignment)),
		_contentPresenter(std::move(other._contentPresenter)),
		_isContentPresenterValid(other._isContentPresenterValid)
	{}

	ContentUIElement::~ContentUIElement()
	{
		if(_contentPresenter)
		{
			OnContentPresenterDestroying(_contentPresenter.GetValue());
		}
	}

	void ContentUIElement::SetContent(const Ref<INotifyPropertyChanged>& value)
	{
		SetContent(value, ValueSource::Local);
	}

	void ContentUIElement::SetContent(const Ref<INotifyPropertyChanged>& value, ValueSource source)
	{
		if(SetProperty(ContentProperty, _content, value, _contentSource, source))
		{
			if(!value.Is<UIElement>())
			{
				SetDataContext(value);
			}

			InvalidateContentPresenter();
		}
	}

	void ContentUIElement::SetContentTemplate(const Ref<IDataTemplate>& value)
	{
		SetContentTemplate(value, ValueSource::Local);
	}

	void ContentUIElement::SetContentTemplate(const Ref<IDataTemplate>& value, ValueSource source)
	{
		if(SetProperty(ContentTemplateProperty, _contentTemplate, value, _contentTemplateSource, source))
		{
			InvalidateContentPresenter();
		}
	}

	void ContentUIElement::SetPadding(Thickness value)
	{
		SetPadding(value, ValueSource::Local);
	}

	void ContentUIElement::SetPadding(Thickness value, ValueSource source)
	{
		if(SetProperty(PaddingProperty, _padding, value, _paddingSource, source))
		{
			InvalidateMeasure();
		}
	}

	void ContentUIElement::SetVerticalContentAlignment(VerticalAlignment value)
	{
		SetVerticalContentAlignment(value, ValueSource::Local);
	}

	void ContentUIElement::SetVerticalContentAlignment(VerticalAlignment value, ValueSource source)
	{
		if(SetProperty(VerticalContentAlignmentProperty, _verticalContentAlignment, value, _verticalContentAlignmentSource, source))
		{
			InvalidateArrange();
		}
	}

	void ContentUIElement::SetHorizontalContentAlignment(HorizontalAlignment value)
	{
		SetHorizontalContentAlignment(value, ValueSource::Local);
	}

	void ContentUIElement::SetHorizontalContentAlignment(HorizontalAlignment value, ValueSource source)
	{
		if(SetProperty(HorizontalContentAlignmentProperty, _horizontalContentAlignment, value, _horizontalContentAlignmentSource, source))
		{
			InvalidateArrange();
		}
	}
		
	void ContentUIElement::SetVisualRoot(IVisualRoot* value)
	{
		Renderable::SetVisualRoot(value);

		if(_contentPresenter)
		{
			_contentPresenter->SetVisualRoot(value);
		}
	}

	void ContentUIElement::SetStylingRoot(IStyleHost* value)
	{
		Renderable::SetStylingRoot(value);

		if(_contentPresenter)
		{
			_contentPresenter->SetStylingRoot(value);
		}
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

		if(_contentPresenter)
		{
			_contentPresenter->ApplyStyle();
		}
	}

	void ContentUIElement::OnContentPresenterCreated(UIElement& presenter)
	{
		presenter.SetParent(this);

		if(IsAttachedToLogicalTree())
		{
			presenter.OnAttachedToLogicalTree();		
		}
	}

	void ContentUIElement::OnContentPresenterDestroying(UIElement& presenter)
	{
		if(IsAttachedToLogicalTree())
		{
			presenter.OnDetachedFromLogicalTree();
		}

		SetParent(nullptr);
	}

	void ContentUIElement::OnAttachedToLogicalTree()
	{
		UIElement::OnAttachedToLogicalTree();

		if(_contentPresenter)
		{
			_contentPresenter->OnAttachedToLogicalTree();
		}
	}

	void ContentUIElement::OnDetachedFromLogicalTree()
	{
		UIElement::OnDetachedFromLogicalTree();

		if(_contentPresenter)
		{
			_contentPresenter->OnDetachedFromLogicalTree();
		}
	}

	void ContentUIElement::OnCursorChanged(const Cursor& cursor)
	{
		if(IsMouseOver() && !(_contentPresenter && _contentPresenter->IsMouseOver()))
		{
			Cursor::Set(cursor);
		}
	}

	void ContentUIElement::OnMouseMove(MouseMoveEventArgs e)
	{
		UIElement::OnMouseMove(e);

		if(_contentPresenter)
		{
			bool visible = _contentPresenter->IsVisible();
			bool wasMouseOver = _contentPresenter->_isMouseOver;
			bool isMouseOver = visible && LayoutHelper::IsPointInRect(e.X, e.Y, _contentPresenter->_bounds);

			if(isMouseOver)
			{
				if(!wasMouseOver)
				{
					_contentPresenter->OnMouseEnter(e);
				}

				_contentPresenter->OnMouseMove(e);
			}
			else if(wasMouseOver)
			{
				_contentPresenter->OnMouseLeave(e);
				Cursor::Set(GetCursor());
			}
		}
	}

	void ContentUIElement::OnMouseDown(MouseButtonEventArgs e)
	{
		UIElement::OnMouseDown(e);

		if(_contentPresenter && _contentPresenter->IsVisible() && _contentPresenter->IsMouseOver())
		{
			_contentPresenter->OnMouseDown(e);
		}
	}

	void ContentUIElement::OnMouseUp(MouseButtonEventArgs e)
	{
		UIElement::OnMouseUp(e);

		if(_contentPresenter && _contentPresenter->IsVisible() && _contentPresenter->IsMouseOver())
		{
			_contentPresenter->OnMouseUp(e);
		}
	}

	void ContentUIElement::OnMouseLeave(MouseMoveEventArgs e)
	{
		UIElement::OnMouseLeave(e);

		if(_contentPresenter && _contentPresenter->IsVisible())
		{
			_contentPresenter->OnMouseLeave(e);
		}
	}

	void ContentUIElement::InvalidateContentPresenter()
	{
		InvalidateMeasure();

		_isContentPresenterValid = false;
	}	

	FSize ContentUIElement::MeasureContent(FSize avaliableSize)
	{
		if(!_isContentPresenterValid)
		{
			UpdatePresenter();
		}

		if(_contentPresenter)
		{
			bool visible = _contentPresenter->IsVisible();
			int horizontalPadding = visible ? _padding.Left + _padding.Right : 0;
			int verticalPadding = visible ? _padding.Top + _padding.Bottom : 0;

			FSize contentAvaliableSize =
			{
				.Width = std::clamp<float>(avaliableSize.Width - verticalPadding,
					GetMinWidth(),
					GetMaxWidth()),
				.Height = std::clamp<float>(avaliableSize.Height - horizontalPadding,
					GetMinHeight(),
					GetMaxHeight())
			};

			 _contentPresenter->Measure(contentAvaliableSize);
			 auto [width, height] = _contentPresenter->GetDesiredSize();

			 return FSize 
			 {
				 .Width = width + horizontalPadding,
				 .Height = height + verticalPadding
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

	bool ContentUIElement::UpdatePresenter()
	{
		if(_contentTemplate && _contentTemplate->Match(_content))
		{
			if(_contentPresenter)
			{
				OnContentPresenterDestroying(_contentPresenter.GetValue());
			}

			_contentPresenter = _contentTemplate->Build(_content);
			_isContentPresenterValid = true;

			if(_contentPresenter)
			{
				OnContentPresenterCreated(_contentPresenter.GetValue());
			}

			return true;
		}

		return false;
	}
}
