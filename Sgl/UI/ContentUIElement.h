#pragma once

#include "UIElement.h"
#include "Template/DataTemplate.h"

namespace Sgl
{
	class ContentUIElement: public UIElement
	{
	protected:
		Shared<UIElement> _contentPresenter;
		Any _content;
	private:
		Shared<IDataTemplate> _contentTemplate;
		Thickness _padding;
		VerticalAlignment _verticalContentAlignment;
		HorizontalAlignment _horizontalContentAlignment;
	public:
		ContentUIElement();
		ContentUIElement(const ContentUIElement& other);
		ContentUIElement(ContentUIElement&& other) noexcept;
		
		template<typename T>
		void SetContent(T value)
		{
			_content = value;
			TryCreatePresenter();
		}

		template<std::derived_from<UIElement> T>
		void SetContent(const Shared<T>& value)
		{
			_content = Any::New<Shared<UIElement>>(value);
			_contentTemplate = NewShared<UIElementDataTemplate>();
			TryCreatePresenter();
		}

		Shared<UIElement> GetContentPresenter() const 
		{
			return _contentPresenter;
		}

		void SetContentTemplate(Shared<IDataTemplate> value)
		{
			SetProperty(ContentTemplateProperty, _contentTemplate, value);
			TryCreatePresenter();
		}

		Shared<IDataTemplate> GetContentTemplate() const
		{
			return _contentTemplate;
		}

		void SetPadding(Thickness value)
		{ 
			SetProperty(PaddingProperty, _padding, value);
			InvalidateMeasure();
		}

		Thickness GetPadding() const 
		{ 
			return _padding;
		}

		void SetVerticalContentAlignment(VerticalAlignment value)
		{ 
			SetProperty(VerticalContentAlignmentProperty, _verticalContentAlignment, value);
			InvalidateArrange();
		}

		VerticalAlignment GetVerticalContentAlignment() const 
		{ 
			return _verticalContentAlignment;
		}

		void SetHorizontalContentAlignment(HorizontalAlignment value) 
		{ 
			SetProperty(HorizontalContentAlignmentProperty, _horizontalContentAlignment, value);
			InvalidateArrange();
		}

		HorizontalAlignment GetHorizontalContentAlignment() const
		{ 
			return _horizontalContentAlignment;
		}

		void Render(RenderContext context) const override;
		void ApplyStyle() override;
	protected:
		void OnMouseMove(const MouseEventArgs& e) override;
		void OnMouseDown(const MouseButtonEventArgs& e) override;
		void OnMouseUp(const MouseButtonEventArgs& e) override;
		void OnMouseLeave(const MouseEventArgs& e) override;

		void TryCreatePresenter();
		FSize MeasureContent(FSize avaliableSize) override;
		void ArrangeContent(FRect rect) override;
	public:
		static inline BindableProperty<ContentUIElement, Shared<IDataTemplate>> ContentTemplateProperty =
			BindableProperty<ContentUIElement, Shared<IDataTemplate>>(&SetContentTemplate);

		static inline BindableProperty<ContentUIElement, Thickness> PaddingProperty =
			BindableProperty<ContentUIElement, Thickness>(&SetPadding);

		static inline BindableProperty<ContentUIElement, VerticalAlignment> VerticalContentAlignmentProperty =
			BindableProperty<ContentUIElement, VerticalAlignment>(&SetVerticalContentAlignment, VerticalAlignment::Top);

		static inline BindableProperty<ContentUIElement, HorizontalAlignment> HorizontalContentAlignmentProperty =
			BindableProperty<ContentUIElement, HorizontalAlignment>(&SetHorizontalContentAlignment, HorizontalAlignment::Left);
	};
}