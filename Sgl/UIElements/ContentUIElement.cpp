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

	void ContentUIElement::SetContent(const Ref<ObservableObject>& value)
	{
		SetProperty(ContentProperty, _content, value, _contentSource);
	}

	void ContentUIElement::SetContentTemplate(const Ref<IDataTemplate>& value)
	{
		SetProperty(ContentTemplateProperty, _contentTemplate, value, _contentTemplateSource);
	}

	void ContentUIElement::SetPadding(Thickness value)
	{
		SetProperty(PaddingProperty, _padding, value, _paddingSource);
	}

	void ContentUIElement::SetVerticalContentAlignment(VerticalAlignment value)
	{
		SetProperty(VerticalContentAlignmentProperty, _verticalContentAlignment, value, _verticalContentAlignmentSource);
	}

	void ContentUIElement::SetHorizontalContentAlignment(HorizontalAlignment value)
	{
		SetProperty(HorizontalContentAlignmentProperty, _horizontalContentAlignment, value, _horizontalContentAlignmentSource);
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
				ValueSourceScope scope(_contentPresenter, ValueSource::Inheritance);
				_contentPresenter->SetVerticalAlignment(_verticalContentAlignment);
			}
		}
		else if(property == HorizontalContentAlignmentProperty)
		{
			InvalidateArrange();

			if(_contentPresenter)
			{
				ValueSourceScope scope(_contentPresenter, ValueSource::Inheritance);
				_contentPresenter->SetHorizontalAlignment(_horizontalContentAlignment);
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

				ValueSourceScope scope(_contentPresenter, ValueSource::Inheritance);
				_contentPresenter->SetVerticalAlignment(_verticalContentAlignment);
				_contentPresenter->SetHorizontalAlignment(_horizontalContentAlignment);
			}
		}
		else
		{
			_contentPresenter = nullptr;
		}
	}
}
