#include "ContentUIElement.h"
#include <algorithm>

namespace Sgl
{
	ContentUIElement::ContentUIElement():
		UIElement(),
		ContentTemplate(*this),
		Padding(*this),
		HorizontalContentAlignment(*this, HorizontalAlignment::Center),
		VerticalContentAlignment(*this, VerticalAlignment::Center)
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
		if(auto content = _contentPresenter; content != nullptr)
		{
			content->OnRender(context);
		}

		UIElement::OnRender(context);
	}

	void ContentUIElement::TryCreatePresenter()
	{
		auto contentTemplate = ContentTemplate.Get();

		if(contentTemplate != nullptr && !_content.IsEmpty())
		{
			_contentPresenter = contentTemplate->Build(_content);
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
			auto horizontalContentAlignment = HorizontalContentAlignment.Get();
			auto verticalContentAlignment = VerticalContentAlignment.Get();
			auto availableWidth = rect.w;
			auto availableHeight = rect.h;
			auto x = rect.x;
			auto y = rect.y;

			if(horizontalContentAlignment != HorizontalAlignment::Stretch)
			{
				availableWidth = std::fmin(availableWidth, _desiredSize.Width);
			}

			if(verticalContentAlignment != VerticalAlignment::Stretch)
			{
				availableHeight = std::min(availableHeight, _desiredSize.Height);
			}

 			switch(horizontalContentAlignment)
			{
				case HorizontalAlignment::Right:
					x += availableWidth - rect.w;
					break;

				case HorizontalAlignment::Center:
					x += (availableWidth - rect.w) / 2.f;
					break;

				default:
					break;
			}

			switch(verticalContentAlignment)
			{
				case VerticalAlignment::Bottom:
					y += availableHeight - rect.h;
					break;

				case VerticalAlignment::Center:
					y += (availableHeight - rect.h) / 2.f;
					break;

				default:
					break;
			}

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

			_contentPresenter->Arrange(finalRect);
		}
	}
}
