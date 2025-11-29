#pragma once

#include "UIElement.h"

namespace Sgl
{
	class ContentUIElement : public UIElement
	{
	private:
		Ref<ObservableObject> _content;
		Ref<IDataTemplate> _contentTemplate = New<UIElementDataTemplate>();
		Ref<UIElement> _presenter;
		Thickness _padding;
		VerticalAlignment _verticalContentAlignment = VerticalAlignment::Top;
		HorizontalAlignment _horizontalContentAlignment = HorizontalAlignment::Left;
		bool _isContentPresenterValid = false;
	public:
		ContentUIElement() = default;
		ContentUIElement(const ContentUIElement& other);
		ContentUIElement(ContentUIElement&& other) noexcept;
		~ContentUIElement();
		
		const Ref<UIElement>& GetContentPresenter() const { return _presenter; }

		void SetContent(Ref<ObservableObject> content);
		Ref<ObservableObject> GetContent() const { return _content; }

		void SetContentTemplate(Ref<IDataTemplate> value);
		Ref<IDataTemplate> GetContentTemplate() const { return _contentTemplate; }

		void SetPadding(Thickness value);
		Thickness GetPadding() const { return _padding; }

		void SetVerticalContentAlignment(VerticalAlignment value);
		VerticalAlignment GetVerticalContentAlignment() const { return _verticalContentAlignment; }

		void SetHorizontalContentAlignment(HorizontalAlignment value);
		HorizontalAlignment GetHorizontalContentAlignment() const { return _horizontalContentAlignment; }

		void SetVisualRoot(IVisualRoot* value) override;
		void Render(RenderContext context) override;
		void ApplyStyle() override;
	protected:
		virtual void OnContentPresenterCreated(UIElement& presenter);
		virtual void OnContentPresenterDestroying(UIElement& presenter);
		void OnCursorChanged(const Cursor& cursor) override;
		void OnMouseMove(MouseMoveEventArgs e) override;
		void OnMouseDown(MouseButtonEventArgs e) override;
		void OnMouseUp(MouseButtonEventArgs e) override;
		void OnMouseLeave(MouseMoveEventArgs e) override;
		
		FSize MeasureContent(FSize avaliableSize) override;
		void ArrangeContent(FRect rect) override;
	protected:
		void InvalidateContentPresenter();
	private:
		bool UpdatePresenter();
	public:
		static inline ObservableProperty ContentProperty { &SetContent, &GetContent };
		static inline ObservableProperty ContentTemplateProperty { &SetContentTemplate, &GetContentTemplate };
		static inline ObservableProperty PaddingProperty { &SetPadding, &GetPadding };
		static inline ObservableProperty VerticalContentAlignmentProperty { &SetVerticalContentAlignment, &GetVerticalContentAlignment };
		static inline ObservableProperty HorizontalContentAlignmentProperty { &SetHorizontalContentAlignment, &GetHorizontalContentAlignment };
	};
}