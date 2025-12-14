#include "UIElementsCollection.h"

namespace Sgl
{
	Sgl::UIElementsCollection::UIElementsCollection(UIElement& owner):
		_owner(owner)
	{}

	UIElementsCollection::UIElementsCollection(UIElementsCollection && other) noexcept:
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
			OnChildRemoving(item.GetValue());
		}

		base::ClearItems();
	}

	void UIElementsCollection::InsertItem(size_t index, const Ref<UIElement>& item)
	{
		assert(item != nullptr);

		OnChildAdded(item.GetValue());
		base::InsertItem(index, item);
	}

	void UIElementsCollection::SetItem(size_t index, const Ref<UIElement>& item)
	{
		assert(item != nullptr);

		OnChildAdded(item.GetValue());
		base::SetItem(index, item);
	}

	void UIElementsCollection::RemoveItem(size_t index)
	{
		auto& item = GetElementAt(index);
		OnChildRemoving(item.GetValue());

		base::RemoveItem(index);
	}

	void UIElementsCollection::OnChildAdded(UIElement& child)
	{
		child.SetParent(&_owner);

		if(child.IsAttachedToLogicalTree())
		{
			child.OnAttachedToLogicalTree();
		}
	}

	void UIElementsCollection::OnChildRemoving(UIElement& child)
	{
		if(child.IsAttachedToLogicalTree())
		{
			child.OnDetachedFromLogicalTree();
		}

		child.SetParent(nullptr);
	}
}

