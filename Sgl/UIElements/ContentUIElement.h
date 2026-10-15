#pragma once

#include "UIElement.h"
#include "../Data/IDataTemplate.h"

namespace Sgl
{
	class ContentUIElement : public UIElement
	{
	public:
		ContentUIElement() = default;
		
		void SetContent(const std::string& value);
		void SetContent(const Ref<ObservableObject>& value);
		const Ref<ObservableObject>& GetContent() const { return _content; }

		void SetContentTemplate(const Ref<IDataTemplate>& value);
		const Ref<IDataTemplate>& GetContentTemplate() const { return _contentTemplate; }

		void SetPadding(Thickness value);
		Thickness GetPadding() const { return _padding; }

		void SetVerticalContentAlignment(VerticalAlignment value);
		VerticalAlignment GetVerticalContentAlignment() const { return _verticalContentAlignment; }

		void SetHorizontalContentAlignment(HorizontalAlignment value);
		HorizontalAlignment GetHorizontalContentAlignment() const { return _horizontalContentAlignment; }

		const Ref<UIElement>& GetChild() const { return _child; }

		static inline StyleableProperty ContentProperty { &SetContent, &GetContent };
		static inline StyleableProperty ContentTemplateProperty { &SetContentTemplate, &GetContentTemplate };
		static inline StyleableProperty PaddingProperty { &SetPadding, &GetPadding };
		static inline StyleableProperty VerticalContentAlignmentProperty { &SetVerticalContentAlignment, &GetVerticalContentAlignment };
		static inline StyleableProperty HorizontalContentAlignmentProperty { &SetHorizontalContentAlignment, &GetHorizontalContentAlignment };
	protected:		
		void OnPropertyChanged(PropertyBase& property) override;
		FSize MeasureContent(FSize availableSize) override;
		void ArrangeContent(FRect rect) override;
	private:
		void UpdateChild();
		Ref<UIElement> CreateChild() const;
	private:
		Ref<ObservableObject> _content;
		Ref<IDataTemplate> _contentTemplate;
		Ref<UIElement> _child;
		Thickness _padding;
		VerticalAlignment _verticalContentAlignment = VerticalAlignment::Stretch;
		HorizontalAlignment _horizontalContentAlignment = HorizontalAlignment::Stretch;

		ValueSource _contentSource {};
		ValueSource _contentTemplateSource {};
		ValueSource _paddingSource {};
		ValueSource _verticalContentAlignmentSource {};
		ValueSource _horizontalContentAlignmentSource {};
	};
}