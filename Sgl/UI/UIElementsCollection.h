#pragma once

#include "UIElement.h"
#include "../Base/Collections/Collection.h"
#include <cassert>

namespace Sgl
{
	class UIElementsCollection : public Collection<Ref<UIElement>>
	{
	private:
		using base = Collection<Ref<UIElement>>;
		Layoutable& _owner;
	public:
		UIElementsCollection(Layoutable& layout):
			base(),
			_owner(layout)
		{}

		UIElementsCollection(const UIElementsCollection& other) :
			base(other),
			_owner(other._owner)
		{}

		UIElementsCollection(UIElementsCollection&& other) noexcept :
			base(std::move(other)),
			_owner(other._owner)
		{}

		~UIElementsCollection()
		{
			ClearItems();
		}
	protected:
		void ClearItems() override
		{
			auto& items = Items();
			for(auto& item : items)
			{
				item->_parent = nullptr;
				item->SetVisualRoot(_owner.GetVisualRoot());
				item->OnDetached();
			}

			_owner.InvalidateMeasure();
		}

		void InsertItem(size_t index, const Ref<UIElement>& item) override
		{
			assert(item != nullptr);
			
			item->_parent = &_owner;
			item->SetVisualRoot(_owner.GetVisualRoot());
			item->OnAttached();
			base::InsertItem(index, item);
			_owner.InvalidateMeasure();
		}

		void SetItem(size_t index, const Ref<UIElement>& item) override
		{
			assert(item != nullptr);

			item->_parent = &_owner;
			item->SetVisualRoot(_owner.GetVisualRoot());
			item->OnAttached();
			base::SetItem(index, item);
			_owner.InvalidateMeasure();
		}

		void RemoveItem(size_t index) override
		{
			auto& item = ElementAt(index);
			item->_parent = nullptr;
			item->SetVisualRoot(_owner.GetVisualRoot());
			item->OnDetached();

			base::RemoveItem(index);
			_owner.InvalidateMeasure();
		}
	};
}