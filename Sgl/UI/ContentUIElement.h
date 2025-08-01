#pragma once

#include "UIElement.h"
#include "Template/DataTemplate.h"

namespace Sgl
{
	class ContentUIElement: public UIElement
	{
	private:
		class DataTemplateProperty: public StyleableProperty<Shared<IDataTemplate>>
		{
		private:
			using base = StyleableProperty<Shared<IDataTemplate>>;
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
	public:
		DataTemplateProperty ContentTemplate;
		MeasuredProperty<Thickness> Padding;
		ArrangedProperty<Sgl::VerticalAlignment> VerticalContentAlignment;
		ArrangedProperty<Sgl::HorizontalAlignment> HorizontalContentAlignment;
	protected:
		Shared<UIElement> _contentPresenter;
		Any _content;
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
			ContentTemplate = NewShared<UIElementDataTemplate>();
		}

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