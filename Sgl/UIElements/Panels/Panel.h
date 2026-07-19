#pragma once

#include "../UIElement.h"
#include "../../Base/Collections/Collection.h"

namespace Sgl
{
	class Panel;

	class UIElementsCollection : public Collection<Ref<UIElement>>
	{
	private:
		using base = Collection<Ref<UIElement>>;
	public:
		explicit UIElementsCollection(Panel& panel);
		UIElementsCollection(const UIElementsCollection&) = delete;
		UIElementsCollection(UIElementsCollection&&) = delete;
		~UIElementsCollection();

		Ref<UIElement> FindByName(std::string_view name) const;

		template<typename T>
		Ref<T> FindOfType() const
		{
			for(auto& item : _items)
			{
				if(item.OfType<T>())
				{
					return item.As<T>();
				}
			}

			return nullptr;
		}
	protected:
		void ClearItems() override;
		void InsertItem(size_t index, const Ref<UIElement>& item) override;
		void SetItem(size_t index, const Ref<UIElement>& item) override;
		void RemoveItem(size_t index) override;
	private:
		Panel& _panel;
	};

	class Panel : public UIElement
	{
	public:
		Panel();
		Panel(const Panel&) = delete;
		Panel(Panel&& other) noexcept;

		UIElementsCollection Children;
	protected:
		void OnChildAdded(UIElement* child);
		void OnChildRemoving(UIElement* child);
		std::span<const Ref<UIElement>> GetChildren() const final;
		FSize MeasureContent(FSize availableSize) override;
		void ArrangeContent(FRect rect) override;

		friend class UIElementsCollection;
	};	

	struct Panel_FirstChild
	{
		StyleableElement& operator()(StyleableElement& target) const;
	};

	struct Panel_LastChild
	{
		StyleableElement& operator()(StyleableElement& target) const;
	};

	struct Panel_NthChild
	{
		explicit Panel_NthChild(size_t position);
		const size_t Index;
		StyleableElement& operator()(StyleableElement& target) const;	
	};
}