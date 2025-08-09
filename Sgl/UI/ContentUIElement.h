#pragma once

#include "Template/DataTemplate.h"
#include "UIElements/TextBlock.h"

namespace Sgl
{
	class ContentUIElement: public UIElement
	{
	private:
		Any _content;
		DataTemplate _contentTemplate;
		Shared<UIElement> _contentPresenter;
		Thickness _padding;
		VerticalAlignment _verticalContentAlignment = VerticalAlignment::Top;
		HorizontalAlignment _horizontalContentAlignment = HorizontalAlignment::Left;
		bool _isContentPresenterValid = false;
	public:
		ContentUIElement() = default;
		ContentUIElement(const ContentUIElement& other);
		ContentUIElement(ContentUIElement&& other) noexcept;
		~ContentUIElement() = default;

		template<typename T>
		void SetContent(T&& value)
		{
			using TContent = std::decay_t<T>;

			if constexpr(std::convertible_to<TContent, std::string>)
			{
				_content = Any::New<std::string>(std::forward<T>(value));
				_contentTemplate = StringDataTemplate();
			}
			else
			{
				_content = Any::New<TContent>(std::forward<T>(value));
			}

			InvalidateMeasure();
			InvalidateContentPresenter();
		}

		template<std::derived_from<UIElement> T>
		void SetContent(Shared<T> value)
		{
			_content = Any::New<Shared<UIElement>>(std::move(value));
			_contentTemplate = UIElementDataTemplate();
			InvalidateMeasure();
			InvalidateContentPresenter();
		}

		Shared<UIElement> GetContentPresenter() const 
		{
			return _contentPresenter;
		}

		void SetContentTemplate(DataTemplate value)
		{
			SetProperty(ContentTemplateProperty, _contentTemplate, value);
			InvalidateMeasure();
			InvalidateContentPresenter();
		}

		DataTemplate GetContentTemplate() const
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

		void Render(RenderContext context) override;
		void ApplyStyle() override;
	protected:
		void OnMouseMove(const MouseEventArgs& e) override;
		void OnMouseDown(const MouseButtonEventArgs& e) override;
		void OnMouseUp(const MouseButtonEventArgs& e) override;
		void OnMouseLeave(const MouseEventArgs& e) override;

		FSize MeasureContent(FSize avaliableSize) override;
		void ArrangeContent(FRect rect) override;
	private:
		bool TryCreatePresenter();
		void InvalidateContentPresenter() { _isContentPresenterValid = false; }
	public:
		static inline BindableProperty<ContentUIElement, DataTemplate> ContentTemplateProperty =
			BindableProperty<ContentUIElement, DataTemplate>(&SetContentTemplate);

		static inline BindableProperty<ContentUIElement, Thickness> PaddingProperty =
			BindableProperty<ContentUIElement, Thickness>(&SetPadding);

		static inline BindableProperty<ContentUIElement, VerticalAlignment> VerticalContentAlignmentProperty =
			BindableProperty<ContentUIElement, VerticalAlignment>(&SetVerticalContentAlignment);

		static inline BindableProperty<ContentUIElement, HorizontalAlignment> HorizontalContentAlignmentProperty =
			BindableProperty<ContentUIElement, HorizontalAlignment>(&SetHorizontalContentAlignment);
	};
}