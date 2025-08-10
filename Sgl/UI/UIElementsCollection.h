#pragma once

#include "UIElement.h"
#include "../Base/Collections/SortedVector.h"

namespace Sgl
{
	class UIElementsCollection
	{
	private:
		SortedVector<Shared<UIElement>, UIElementComparer> _items;
		Layoutable& _owner;
	public:
		UIElementsCollection(Layoutable& layout):
			_owner(layout)
		{}

		UIElementsCollection(const UIElementsCollection& other):
			_items(other._items),
			_owner(other._owner)
		{}

		UIElementsCollection(UIElementsCollection&& other) noexcept:
			_items(std::move(other._items)),
			_owner(other._owner)
		{}

		auto begin() { return _items.begin(); }
		auto end() { return _items.end(); }

		auto begin() const { return _items.begin(); }
		auto end() const { return _items.end(); }

		void Add(Shared<UIElement> element)
		{
			if(element == nullptr)
			{
				throw std::invalid_argument("UIElement can not be null");
			}

			element->_stylingParent = &_owner;
			element->_layoutableParent = &_owner;
			_items.Add(std::move(element));
		}

		size_t Count() const noexcept
		{
			return _items.Count();
		}

		size_t Capacity() const noexcept
		{
			return _items.Capacity();
		}

		void Clear() noexcept
		{
			_items.Clear();
		}

		bool Contains(Shared<UIElement> element) const
		{
			return Contains(element);
		}

		Shared<UIElement> ElementAt(size_t index) const
		{
			return _items.ElementAt(index);
		}

		std::optional<size_t> Find(Shared<UIElement> element) const
		{
			return _items.Find(element);
		}

		void Remove(Shared<UIElement> element)
		{
			_items.Remove(element);
		}

		UIElementsCollection& operator=(std::initializer_list<Shared<UIElement>> items)
		{
			_items = items;
			return *this;
		}
	};
}