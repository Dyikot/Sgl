#pragma once

#include "UIElement.h"

namespace Sgl
{
	class ContentUIElement : public UIElement
	{
	private:
		Ref<INotifyPropertyChanged> _content;
		Ref<IDataTemplate> _contentTemplate = New<UIElementDataTemplate>();
		Ref<UIElement> _contentPresenter;
		Thickness _padding;
		VerticalAlignment _verticalContentAlignment = VerticalAlignment::Top;
		HorizontalAlignment _horizontalContentAlignment = HorizontalAlignment::Left;
		bool _isContentPresenterValid = false;

		ValueSource _contentSource {};
		ValueSource _contentTemplateSource {};
		ValueSource _paddingSource {};
		ValueSource _verticalContentAlignmentSource {};
		ValueSource _horizontalContentAlignmentSource {};
	public:
		ContentUIElement() = default;
		ContentUIElement(const ContentUIElement& other);
		ContentUIElement(ContentUIElement&& other) noexcept;
		~ContentUIElement();
		
		const Ref<UIElement>& GetContentPresenter() const { return _contentPresenter; }

		void SetContent(const Ref<INotifyPropertyChanged>& value);
		void SetContent(const Ref<INotifyPropertyChanged>& value, ValueSource source);
		const Ref<INotifyPropertyChanged>& GetContent() const { return _content; }

		void SetContentTemplate(const Ref<IDataTemplate>& value);
		void SetContentTemplate(const Ref<IDataTemplate>& value, ValueSource source);
		const Ref<IDataTemplate>& GetContentTemplate() const { return _contentTemplate; }

		void SetPadding(Thickness value);
		void SetPadding(Thickness value, ValueSource source);
		Thickness GetPadding() const { return _padding; }

		void SetVerticalContentAlignment(VerticalAlignment value);
		void SetVerticalContentAlignment(VerticalAlignment value, ValueSource source);
		VerticalAlignment GetVerticalContentAlignment() const { return _verticalContentAlignment; }

		void SetHorizontalContentAlignment(HorizontalAlignment value);
		void SetHorizontalContentAlignment(HorizontalAlignment value, ValueSource source);
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
		static inline StyleableProperty ContentProperty { &SetContent, &GetContent };
		static inline StyleableProperty ContentTemplateProperty { &SetContentTemplate, &GetContentTemplate };
		static inline StyleableProperty PaddingProperty { &SetPadding, &GetPadding };
		static inline StyleableProperty VerticalContentAlignmentProperty { &SetVerticalContentAlignment, &GetVerticalContentAlignment };
		static inline StyleableProperty HorizontalContentAlignmentProperty { &SetHorizontalContentAlignment, &GetHorizontalContentAlignment };
	};
}