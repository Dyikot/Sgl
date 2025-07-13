#pragma once

#include "UIElement.h"
#include "Template/DataTemplate.h"

namespace Sgl
{
	class ContentUIElement: public UIElement
	{
	public:
		class DataTemplateProperty: public StylyableProperty<shared_ptr<IDataTemplate>>
		{
		private:
			ContentUIElement& _owner;
		public:
			DataTemplateProperty(ContentUIElement& owner):
				StylyableProperty<shared_ptr<IDataTemplate>>(),
				_owner(owner)
			{}

			void OnPropertyChanged(shared_ptr<IDataTemplate> value) override
			{
				_owner.TryCreatePresenter();
			}

			using StylyableProperty<shared_ptr<IDataTemplate>>::operator=;
		};

		DataTemplateProperty ContentTemplate;
		ThicknessProperty Padding;
		HorizontalAlignmentProperty HorizontalContentAlignment;
		VerticalAlignmentProperty VerticalContentAlignment;
	protected:
		ConstVoidPtr _content;
		shared_ptr<UIElement> _contentPresenter;
	public:
		ContentUIElement();
		ContentUIElement(const ContentUIElement& other);
		ContentUIElement(ContentUIElement&& other) noexcept;

		template<typename T>
		void SetContent(const T& value)
		{
			_content = value;
			TryCreatePresenter();
		}

		template<std::derived_from<UIElement> T>
		void SetContent(const shared_ptr<T>& value)
		{
			_content = value;
			ContentTemplate = UIElementDataTemplate::New();
		}

		ConstVoidPtr GetContent() const { return _content; }

		shared_ptr<UIElement> GetPresenter() const { return _contentPresenter; }

		void OnRender(RenderContext context) const override;
	protected:
		void TryCreatePresenter();
	};
}