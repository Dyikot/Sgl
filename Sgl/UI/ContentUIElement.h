#pragma once

#include "UIElements/TextBlock.h"

namespace Sgl
{
	class ContentUIElement : public UIElement
	{
	private:
		Ref<IData> _content;
		DataTemplate _contentTemplate;
		Ref<UIElement> _contentPresenter;
		Thickness _padding;
		VerticalAlignment _verticalContentAlignment = VerticalAlignment::Top;
		HorizontalAlignment _horizontalContentAlignment = HorizontalAlignment::Left;
		bool _isContentPresenterValid = false;
	public:
		ContentUIElement() = default;
		ContentUIElement(const ContentUIElement& other);
		ContentUIElement(ContentUIElement&& other) noexcept;
		~ContentUIElement() = default;
		
		const Ref<UIElement>& GetContentPresenter() const { return _contentPresenter; }

		void SetContent(Ref<IData> content);
		Ref<IData> GetContent() const { return _content; }

		void SetContentTemplate(DataTemplate value);
		DataTemplate GetContentTemplate() const { return _contentTemplate; }

		void SetPadding(Thickness value);
		Thickness GetPadding() const { return _padding; }

		void SetVerticalContentAlignment(VerticalAlignment value);
		VerticalAlignment GetVerticalContentAlignment() const { return _verticalContentAlignment; }

		void SetHorizontalContentAlignment(HorizontalAlignment value);
		HorizontalAlignment GetHorizontalContentAlignment() const { return _horizontalContentAlignment; }

		void Render(RenderContext context) override;
		void ApplyStyle() override;
	protected:
		void OnCursorChanged(const Cursor& cursor) override;
		void OnMouseMove(MouseEventArgs& e) override;
		void OnMouseDown(MouseButtonEventArgs& e) override;
		void OnMouseUp(MouseButtonEventArgs& e) override;
		void OnMouseLeave(MouseEventArgs& e) override;

		FSize MeasureContent(FSize avaliableSize) override;
		void ArrangeContent(FRect rect) override;
	private:
		bool TryCreatePresenter();
		void InvalidateContentPresenter() { _isContentPresenterValid = false; }
	public:
		static inline ObservableProperty<ContentUIElement, Ref<IData>> ContentProperty =
			ObservableProperty<ContentUIElement, Ref<IData>>(&SetContent, &GetContent);

		static inline ObservableProperty<ContentUIElement, DataTemplate> ContentTemplateProperty =
			ObservableProperty<ContentUIElement, DataTemplate>(&SetContentTemplate, &GetContentTemplate);

		static inline ObservableProperty<ContentUIElement, Thickness> PaddingProperty =
			ObservableProperty<ContentUIElement, Thickness>(&SetPadding, &GetPadding);

		static inline ObservableProperty<ContentUIElement, VerticalAlignment> VerticalContentAlignmentProperty =
			ObservableProperty<ContentUIElement, VerticalAlignment>(&SetVerticalContentAlignment, &GetVerticalContentAlignment);

		static inline ObservableProperty<ContentUIElement, HorizontalAlignment> HorizontalContentAlignmentProperty =
			ObservableProperty<ContentUIElement, HorizontalAlignment>(&SetHorizontalContentAlignment, &GetHorizontalContentAlignment);
	};
}