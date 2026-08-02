#pragma once

#include "UIElement.h"

namespace Sgl
{
	class ContentUIElement : public UIElement
	{
	public:
		struct ContentPresenter;
	public:
		ContentUIElement() = default;
		ContentUIElement(ContentUIElement&& other) noexcept;
		~ContentUIElement();
		
		const Ref<UIElement>& GetContentPresenter() const { return _contentPresenter; }

		void SetContent(const std::string& value);
		void SetContent(const Ref<INotifyPropertyChanged>& value, ValueSource source = ValueSource::Local);
		const Ref<INotifyPropertyChanged>& GetContent() const { return _content; }

		void SetContentTemplate(const Ref<IDataTemplate>& value, ValueSource source = ValueSource::Local);
		const Ref<IDataTemplate>& GetContentTemplate() const { return _contentTemplate; }

		void SetPadding(Thickness value, ValueSource source = ValueSource::Local);
		Thickness GetPadding() const { return _padding; }

		void SetVerticalContentAlignment(VerticalAlignment value, ValueSource source = ValueSource::Local);
		VerticalAlignment GetVerticalContentAlignment() const { return _verticalContentAlignment; }

		void SetHorizontalContentAlignment(HorizontalAlignment value, ValueSource source = ValueSource::Local);
		HorizontalAlignment GetHorizontalContentAlignment() const { return _horizontalContentAlignment; }

		static inline StyleableProperty ContentProperty { &SetContent, &GetContent };
		static inline StyleableProperty ContentTemplateProperty { &SetContentTemplate, &GetContentTemplate };
		static inline StyleableProperty PaddingProperty { &SetPadding, &GetPadding };
		static inline StyleableProperty VerticalContentAlignmentProperty { &SetVerticalContentAlignment, &GetVerticalContentAlignment };
		static inline StyleableProperty HorizontalContentAlignmentProperty { &SetHorizontalContentAlignment, &GetHorizontalContentAlignment };
	protected:
		void OnAttachedToLogicalTree() override;
		void InvalidateContentPresenter();
		
		FSize MeasureContent(FSize availableSize) override;
		void ArrangeContent(FRect rect) override;
	private:
		void UpdatePresenter();
	private:
		Ref<INotifyPropertyChanged> _content;
		Ref<IDataTemplate> _contentTemplate = New<UIElementDataTemplate>();
		Ref<UIElement> _contentPresenter;
		Thickness _padding;
		VerticalAlignment _verticalContentAlignment = VerticalAlignment::Stretch;
		HorizontalAlignment _horizontalContentAlignment = HorizontalAlignment::Stretch;
		bool _isContentPresenterValid = false;

		ValueSource _contentSource {};
		ValueSource _contentTemplateSource {};
		ValueSource _paddingSource {};
		ValueSource _verticalContentAlignmentSource {};
		ValueSource _horizontalContentAlignmentSource {};
	};

	struct ContentUIElement::ContentPresenter
	{
		Styleable& operator()(Styleable& element) const;
	};
}