#pragma once

#include "UIElement.h"
#include "Template/DataTemplate.h"
#include "../Base/ContentPropertyBase.h"

namespace Sgl
{
	class ContentUIElement: public UIElement
	{
	private:
		class DataTemplateProperty: public BindableProperty<Shared<IDataTemplate>>
		{
		private:
			using base = BindableProperty<Shared<IDataTemplate>>;
			ContentUIElement& _owner;
		public:
			DataTemplateProperty(ContentUIElement& owner):
				_owner(owner)
			{}

			void OnChanged() override
			{
				_owner.TryCreatePresenter();
			}

			using base::operator=;
		};

		class ContentProperty: public ContentPropertyBase
		{
		private:
			ContentUIElement& _owner;
		public:
			ContentProperty(ContentUIElement& owner):
				_owner(owner)
			{}

			template<std::copyable T>
			ContentProperty& operator=(T value)
			{
				_value = value;
				_owner.TryCreatePresenter();
				return *this;
			}

			template<std::derived_from<UIElement> T>
			ContentProperty& operator=(const Shared<T>& value)
			{
				_value = Any::New<Shared<UIElement>>(value);
				_owner.ContentTemplate = NewShared<UIElementDataTemplate>();
				return *this;
			}
		};

		using VerticalAlignmentProperty = ArrangedProperty<Sgl::VerticalAlignment>;
		using HorizontalAlignmentProperty = ArrangedProperty<Sgl::HorizontalAlignment>;
	public:
		DataTemplateProperty ContentTemplate;
		MeasuredProperty<Thickness> Padding;
		VerticalAlignmentProperty VerticalContentAlignment;
		HorizontalAlignmentProperty HorizontalContentAlignment;
		ContentProperty Content;
	protected:
		Shared<UIElement> _contentPresenter;
	public:
		ContentUIElement();
		ContentUIElement(const ContentUIElement& other);
		ContentUIElement(ContentUIElement&& other) noexcept;

		Shared<UIElement> GetContentPresenter() const { return _contentPresenter; }

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
	};
}