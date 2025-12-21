#pragma once

#include <cassert>

#include "UIElement.h"
#include "../Base/Collections/Collection.h"

namespace Sgl
{
	class Panel;

	class UIElementsCollection : public Collection<Ref<UIElement>>
	{
	private:
		using base = Collection<Ref<UIElement>>;
		Panel& _owner;
	public:
		UIElementsCollection(Panel& owner);
		UIElementsCollection(const UIElementsCollection&) = delete;
		UIElementsCollection(UIElementsCollection&& other) noexcept;
		~UIElementsCollection();
	protected:
		void ClearItems() override;
		void InsertItem(size_t index, const Ref<UIElement>& item) override;
		void SetItem(size_t index, const Ref<UIElement>& item) override;
		void RemoveItem(size_t index) override;		
	};
}