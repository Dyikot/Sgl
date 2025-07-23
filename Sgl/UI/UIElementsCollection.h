#pragma once

#include "UIElement.h"
#include "../Base/Collections/SortedVector.h"

namespace Sgl
{
	class UIElementsCollection
	{
	private:
		SortedVector<std::shared_ptr<UIElement>, UIElementComparer> _items;
		Layoutable& _owner;
	public:
		UIElementsCollection(Layoutable& layout):
			_items(),
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

		void Add(const std::shared_ptr<UIElement>& element)
		{
			_items.Add(element);
			element->_stylingParent = &_owner;
			element->_layoutableParent = &_owner;
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

		bool Contains(const std::shared_ptr<UIElement>& element) const
		{
			return Contains(element);
		}

		const std::shared_ptr<UIElement>& ElementAt(size_t index) const
		{
			return _items.ElementAt(index);
		}

		std::optional<size_t> Find(const std::shared_ptr<UIElement>& element) const
		{
			return _items.Find(element);
		}

		void Remove(const std::shared_ptr<UIElement>& element)
		{
			_items.Remove(element);
		}
	};
}