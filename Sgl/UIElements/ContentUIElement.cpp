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

		if(property == PaddingProperty)
		{
			InvalidateMeasure();
		}
		else if(property == ContentProperty || property == ContentTemplateProperty)
		{
			UpdateChild();
		}
		else if(property == VerticalContentAlignmentProperty)
		{
			InvalidateArrange();

			if(_child)
			{
				ValueSourceScope scope(_child, ValueSource::Inheritance);
				_child->SetVerticalAlignment(_verticalContentAlignment);
			}
		}
		else if(property == HorizontalContentAlignmentProperty)
		{
			InvalidateArrange();

			if(_child)
			{
				ValueSourceScope scope(_child, ValueSource::Inheritance);
				_child->SetHorizontalAlignment(_horizontalContentAlignment);
			}
		}
	}

	FSize ContentUIElement::MeasureContent(FSize availableSize)
	{
		return MeasureChild(_child.Get(), availableSize, _padding);
	}

	void ContentUIElement::ArrangeContent(FRect rect)
	{
		ArrangeChild(_child.Get(), rect, _padding);
	}

	void ContentUIElement::UpdateChild()
	{
		if(_child)
		{
			RemoveChild(_child);
		}

		if(!_content)
		{
			_child = nullptr;
			return;
		}
		else if(auto element = _content.As<UIElement>())
		{
			_child = element;
		}
		else
		{
			_child = CreateChild();
		}

		if(_child)
		{
			AddChild(_child);

			ValueSourceScope scope(_child, ValueSource::Inheritance);
			_child->SetVerticalAlignment(_verticalContentAlignment);
			_child->SetHorizontalAlignment(_horizontalContentAlignment);
		}
	}

	Ref<UIElement> ContentUIElement::CreateChild() const
	{
		if(_contentTemplate && _contentTemplate->Match(_content))
		{
			auto element = _contentTemplate->Build(_content);
			element->SetDataContext(_content);
			return element;
		}

		return nullptr;
	}
}
