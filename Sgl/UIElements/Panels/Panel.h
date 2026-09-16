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
		struct FirstChild;
		struct LastChild;
		struct NthChild;
		struct ChildWithName;
		template<typename T>
		struct ChildOfType;
	public:
		Panel();
		Panel(Panel&& other) noexcept;

		UIElementsCollection Children;
	protected:
		FSize MeasureContent(FSize availableSize) override;
		void ArrangeContent(FRect rect) override;

		friend class UIElementsCollection;
	};	

	struct Panel::FirstChild
	{
		Styleable& operator()(Styleable& element) const
		{
			return static_cast<Panel&>(element).Children.Front().GetValue();
		}
	};

	struct Panel::LastChild
	{
		Styleable& operator()(Styleable& element) const
		{
			return static_cast<Panel&>(element).Children.Back().GetValue();
		}
	};

	struct Panel::NthChild
	{
		explicit Panel::NthChild(size_t position):
			Index(std::max(1ull, position) - 1ull)
		{}

		const size_t Index;

		Styleable& operator()(Styleable& element) const
		{
			return static_cast<Panel&>(element).Children.GetElementAt(Index).GetValue();
		}
	};

	struct Panel::ChildWithName
	{
		std::string Name;

		Styleable& operator()(Styleable& element) const
		{
			return static_cast<Panel&>(element).Children.FindByName(Name).GetValue();
		}
	};

	template<typename T>
	struct Panel::ChildOfType
	{
		Styleable& operator()(Styleable& element) const
		{
			return static_cast<Panel&>(element).Children.FindOfType<T>().GetValue();
		}
	};
}