#pragma once

#include "UIElement.h"

namespace Sgl
{
	class ContentUIElement : public UIElement
	{
	private:
		Ref<INotityPropertyChanged> _content;
		Ref<IDataTemplate> _contentTemplate = New<UIElementDataTemplate>();
		Ref<UIElement> _contentPresenter;
		Thickness _padding;
		VerticalAlignment _verticalContentAlignment = VerticalAlignment::Top;
		HorizontalAlignment _horizontalContentAlignment = HorizontalAlignment::Left;
		bool _isContentPresenterValid = false;
	public:
		ContentUIElement() = default;
		ContentUIElement(const ContentUIElement& other);
		ContentUIElement(ContentUIElement&& other) noexcept;
		~ContentUIElement();
		
		const Ref<UIElement>& GetContentPresenter() const { return _contentPresenter; }

		void SetContent(Ref<INotityPropertyChanged> content);
		Ref<INotityPropertyChanged> GetContent() const { return _content; }

		void SetContentTemplate(Ref<IDataTemplate> value);
		Ref<IDataTemplate> GetContentTemplate() const { return _contentTemplate; }

		void SetPadding(Thickness value);
		Thickness GetPadding() const { return _padding; }

		void SetVerticalContentAlignment(VerticalAlignment value);
		VerticalAlignment GetVerticalContentAlignment() const { return _verticalContentAlignment; }

		void SetHorizontalContentAlignment(HorizontalAlignment value);
		HorizontalAlignment GetHorizontalContentAlignment() const { return _horizontalContentAlignment; }

		void SetVisualRoot(IVisualRoot* value) final;
		void SetStylingRoot(IStyleHost* value) final;

		void Render(RenderContext context) override;
		void ApplyStyle() override;
	protected:
		virtual void OnContentPresenterCreated(UIElement& presenter);
		virtual void OnContentPresenterDestroying(UIElement& presenter);
		void OnAttachedToLogicalTree() override;
		void OnDetachedFromLogicalTree() override;
		void OnCursorChanged(const Cursor& cursor) override;
		void OnMouseMove(MouseMoveEventArgs e) override;
		void OnMouseDown(MouseButtonEventArgs e) override;
		void OnMouseUp(MouseButtonEventArgs e) override;
		void OnMouseLeave(MouseMoveEventArgs e) override;
		void InvalidateContentPresenter();
		
		FSize MeasureContent(FSize avaliableSize) override;
		void ArrangeContent(FRect rect) override;
	private:
		bool UpdatePresenter();
	public:
		static inline SglProperty ContentProperty { &SetContent, &GetContent };
		static inline SglProperty ContentTemplateProperty { &SetContentTemplate, &GetContentTemplate };
		static inline SglProperty PaddingProperty { &SetPadding, &GetPadding };
		static inline SglProperty VerticalContentAlignmentProperty { &SetVerticalContentAlignment, &GetVerticalContentAlignment };
		static inline SglProperty HorizontalContentAlignmentProperty { &SetHorizontalContentAlignment, &GetHorizontalContentAlignment };
	};
}