#include "ContentUIElement.h"
#include "../UIElements/TextBlock.h"
#include "../Layout/LayoutHelper.h"

namespace Sgl
{
	void ContentUIElement::SetContent(const std::string& value)
	{
		if(_content.OfType<UIElements::TextBlock>())
		{
			auto textBlock = _content.GetAs<UIElements::TextBlock>();
			textBlock->SetText(value);
		}
		else
		{
			auto textBlock = New<UIElements::TextBlock>();
			textBlock->SetText(value);
			SetContent(textBlock);
		}
	}

	void ContentUIElement::SetContent(const Ref<ObservableObject>& value, ValueSource source)
	{
		SetProperty(ContentProperty, _content, value, _contentSource, source);
	}

	void ContentUIElement::SetContentTemplate(const Ref<IDataTemplate>& value, ValueSource source)
	{
		SetProperty(ContentTemplateProperty, _contentTemplate, value, _contentTemplateSource, source);
	}

	void ContentUIElement::SetPadding(Thickness value, ValueSource source)
	{
		SetProperty(PaddingProperty, _padding, value, _paddingSource, source);
	}

	void ContentUIElement::SetVerticalContentAlignment(VerticalAlignment value, ValueSource source)
	{
		SetProperty(VerticalContentAlignmentProperty, _verticalContentAlignment, value, _verticalContentAlignmentSource, source);
	}

	void ContentUIElement::SetHorizontalContentAlignment(HorizontalAlignment value, ValueSource source)
	{
		SetProperty(HorizontalContentAlignmentProperty, _horizontalContentAlignment, value, _horizontalContentAlignmentSource, source);
	}

	void ContentUIElement::OnPropertyChanged(PropertyBase& property)
	{
		UIElement::OnPropertyChanged(property);

		if(property == ContentProperty)
		{
			if(!_content.Is<UIElement>())
			{
				SetDataContext(_content);
			}

			UpdatePresenter();
		}
		else if(property == PaddingProperty)
		{
			InvalidateMeasure();
		}
		else if(property == ContentTemplateProperty)
		{
			UpdatePresenter();
		}
		else if(property == VerticalContentAlignmentProperty)
		{
			InvalidateArrange();

			if(_contentPresenter)
			{
				_contentPresenter->SetVerticalAlignment(_verticalContentAlignment, ValueSource::Inheritance);
			}
		}
		else if(property == HorizontalContentAlignmentProperty)
		{
			InvalidateArrange();

			if(_contentPresenter)
			{
				_contentPresenter->SetHorizontalAlignment(_horizontalContentAlignment, ValueSource::Inheritance);
			}
		}
	}

	FSize ContentUIElement::MeasureContent(FSize availableSize)
	{
		return MeasureChild(_contentPresenter.Get(), availableSize, _padding);
	}

	void ContentUIElement::ArrangeContent(FRect rect)
	{
		ArrangeChild(_contentPresenter.Get(), rect, _padding);
	}

	void ContentUIElement::UpdatePresenter()
	{
		if(_contentPresenter)
		{
			RemoveChild(_contentPresenter);
		}

		if(_contentTemplate && _contentTemplate->Match(_content))
		{
			_contentPresenter = _contentTemplate->Build(_content);

			if(_contentPresenter)
			{
				AddChild(_contentPresenter);
				_contentPresenter->SetVerticalAlignment(_verticalContentAlignment, ValueSource::Inheritance);
				_contentPresenter->SetHorizontalAlignment(_horizontalContentAlignment, ValueSource::Inheritance);
			}
		}
		else
		{
			_contentPresenter = nullptr;
		}
	}
}
