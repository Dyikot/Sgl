#include "UIElementsCollection.h"
#include "Panel.h"

namespace Sgl
{
	UIElementsCollection::UIElementsCollection(Panel& owner):
		_owner(owner)
	{}

	UIElementsCollection::UIElementsCollection(UIElementsCollection&& other) noexcept:
		base(std::move(other)),
		_owner(other._owner)
	{}

	UIElementsCollection::~UIElementsCollection()
	{
		ClearItems();
	}

	void UIElementsCollection::ClearItems()
	{
		for(auto& item : _items)
		{
			_owner.OnChildRemoving(item.GetValue());
		}

		base::ClearItems();
	}

	void UIElementsCollection::InsertItem(size_t index, const Ref<UIElement>& item)
	{
		assert(item != nullptr);

		_owner.OnChildAdded(item.GetValue());
		base::InsertItem(index, item);
	}

	void UIElementsCollection::SetItem(size_t index, const Ref<UIElement>& item)
	{
		assert(item != nullptr);

		_owner.OnChildAdded(item.GetValue());
		base::SetItem(index, item);
	}

	void UIElementsCollection::RemoveItem(size_t index)
	{
		auto& item = GetElementAt(index);
		_owner.OnChildRemoving(item.GetValue());

		base::RemoveItem(index);
	}	
}

