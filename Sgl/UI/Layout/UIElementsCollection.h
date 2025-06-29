#pragma once

#include "../UIElement.h"
#include "../../Data/Ranges/SortedVector.h"
#include "ILayout.h"

namespace Sgl
{
	class UIElementsCollection
	{
	public:
		struct OnItemSizeChanged
		{
			ILayout& Layout;

			void operator()(UIElement& element, const EventArgs& e) const
			{
				Layout.QueryArrange();
			}
		};
	private:
		SortedVector<shared_ptr<UIElement>, UIElementComparer> _items;
		ILayout& _layout;
	public:
		UIElementsCollection(ILayout& layout):
			_layout(layout)
		{}

		auto begin() { return _items.begin(); }
		auto end() { return _items.end(); }

		auto begin() const { return _items.begin(); }
		auto end() const { return _items.end(); }

		void Add(const shared_ptr<UIElement>& element)
		{
			_items.Add(element);
			element->SizeChanged += OnItemSizeChanged(_layout);
			_layout.QueryArrange();
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

		bool Contains(const shared_ptr<UIElement>& element) const
		{
			return Contains(element);
		}

		const std::shared_ptr<UIElement>& ElementAt(size_t index) const
		{
			return _items.ElementAt(index);
		}

		std::optional<size_t> Find(const shared_ptr<UIElement>& element) const
		{
			return _items.Find(element);
		}

		void Remove(const shared_ptr<UIElement>& element)
		{
			_items.Remove(element);
			element->SizeChanged -= OnItemSizeChanged(_layout);
			_layout.QueryArrange();
		}
	};
}