#pragma once

#include <cassert>

#include "UIElement.h"
#include "../Base/Collections/Collection.h"

namespace Sgl
{
	class UIElementsCollection : public Collection<Ref<UIElement>>
	{
	private:
		using base = Collection<Ref<UIElement>>;
		UIElement& _owner;
	public:
		UIElementsCollection(UIElement& owner);
		UIElementsCollection(const UIElementsCollection& other);
		UIElementsCollection(UIElementsCollection&& other) noexcept;
		~UIElementsCollection();
	protected:
		void ClearItems() override;
		void InsertItem(size_t index, const Ref<UIElement>& item) override;
		void SetItem(size_t index, const Ref<UIElement>& item) override;
		void RemoveItem(size_t index) override;
	private:
		void OnChildAdded(UIElement& child);
		void OnChildRemoving(UIElement& child);
	};
}