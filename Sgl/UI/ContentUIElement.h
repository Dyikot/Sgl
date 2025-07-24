#pragma once

#include "UIElement.h"
#include "Template/DataTemplate.h"

namespace Sgl
{
	class ContentUIElement: public UIElement
	{
	private:
		class DataTemplateProperty: public ObservableProperty<Shared<IDataTemplate>>
		{
		private:
			using base = ObservableProperty<Shared<IDataTemplate>>;
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
		using VerticalAlignmentProperty = ArrangedProperty<Sgl::VerticalAlignment>;
		using HorizontalAlignmentProperty = ArrangedProperty<Sgl::HorizontalAlignment>;
	public:
		DataTemplateProperty ContentTemplate;
		MeasuredProperty<Thickness> Padding;
		VerticalAlignmentProperty VerticalContentAlignment;
		HorizontalAlignmentProperty HorizontalContentAlignment;
	protected:
		Any _content;
		Shared<UIElement> _contentPresenter;
	public:
		ContentUIElement();
		ContentUIElement(const ContentUIElement& other);
		ContentUIElement(ContentUIElement&& other) noexcept;

		template<std::copyable T>
		void SetContent(const T& value)
		{
			_content = value;
			TryCreatePresenter();
		}

		template<std::derived_from<UIElement> T>
		void SetContent(const Shared<T>& value)
		{
			_content = Any::New<Shared<UIElement>>(value);
			ContentTemplate = CreateShared<UIElementDataTemplate>();
		}

		const Any& GetContent() const { return _content; }
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