#include "ContentUIElement.h"

#include <algorithm>
#include "../UIElements/TextBlock.h"
#include "../Layout/LayoutHelper.h"

namespace Sgl
{
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
			RemoveLogicalChild(_contentPresenter.Get());
		}
	}

	void ContentUIElement::SetContent(const std::string& value)
	{
		if(_content.OfType<UIElements::TextBlock>())
		{
			auto& textBlock = _content.GetValueAs<UIElements::TextBlock>();
			textBlock.SetText(value);
		}
		else
		{
			auto textBlock = New<UIElements::TextBlock>();
			textBlock->SetText(value);
			SetContent(textBlock);
		}
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

	void ContentUIElement::SetContentTemplate(const Ref<IDataTemplate>& value, ValueSource source)
	{
		if(SetProperty(ContentTemplateProperty, _contentTemplate, value, _contentTemplateSource, source))
		{
			InvalidateContentPresenter();
		}
	}

	void ContentUIElement::SetPadding(Thickness value, ValueSource source)
	{
		if(SetProperty(PaddingProperty, _padding, value, _paddingSource, source))
		{
			InvalidateMeasure();
		}
	}

	void ContentUIElement::SetVerticalContentAlignment(VerticalAlignment value, ValueSource source)
	{
		if(SetProperty(VerticalContentAlignmentProperty, _verticalContentAlignment, value, _verticalContentAlignmentSource, source))
		{
			InvalidateArrange();

			if(_contentPresenter)
			{
				_contentPresenter->SetVerticalAlignment(value, ValueSource::Inheritance);
			}
		}
	}

	void ContentUIElement::SetHorizontalContentAlignment(HorizontalAlignment value, ValueSource source)
	{
		if(SetProperty(HorizontalContentAlignmentProperty, _horizontalContentAlignment, value, _horizontalContentAlignmentSource, source))
		{
			InvalidateArrange();

			if(_contentPresenter)
			{
				_contentPresenter->SetHorizontalAlignment(value, ValueSource::Inheritance);
			}
		}
	}
	
	void ContentUIElement::OnAttachedToLogicalTree()
	{
		UIElement::OnAttachedToLogicalTree();
		UpdatePresenter();
	}

	void ContentUIElement::InvalidateContentPresenter()
	{
		InvalidateMeasure();
		_isContentPresenterValid = false;
	}	

	std::span<const Ref<UIElement>> ContentUIElement::GetChildren() const
	{
		if(!_contentPresenter)
		{
			return {};
		}

		return std::span(&_contentPresenter, 1);
	}

	FSize ContentUIElement::MeasureContent(FSize availableSize)
	{
		UpdatePresenter();
		return MeasureChild(_contentPresenter.Get(), availableSize, _padding);
	}

	void ContentUIElement::ArrangeContent(FRect rect)
	{
		ArrangeChild(_contentPresenter.Get(), rect, _padding);
	}

	void ContentUIElement::UpdatePresenter()
	{
		if(_isContentPresenterValid)
		{
			return;
		}

		if(_contentTemplate && _contentTemplate->Match(_content))
		{
			if(_contentPresenter)
			{
				RemoveLogicalChild(_contentPresenter.Get());
			}

			_contentPresenter = _contentTemplate->Build(_content);
			_isContentPresenterValid = true;

			if(_contentPresenter)
			{
				AddLogicalChild(_contentPresenter.Get());
				_contentPresenter->SetVerticalAlignment(_verticalContentAlignment, ValueSource::Inheritance);
				_contentPresenter->SetHorizontalAlignment(_horizontalContentAlignment, ValueSource::Inheritance);
			}
		}
	}

	Styleable& ContentUIElement::ContentPresenter::operator()(Styleable& element) const
	{
		return static_cast<ContentUIElement&>(element).GetContentPresenter().GetValue();
	}
}
