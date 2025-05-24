#pragma once

#include <set>
#include "../../Events/MouseAndKeyEvents.h"
#include "../VisualElement.h"

namespace Sgl
{
	class UIElement;

	class UIElementsCollection: public IMouseEventsListener, public IKeyEventsListener
	{
	public:
		struct UIElementContext
		{
			FPoint Position;
			UIElement& Element;
		};

		struct ZIndexComparer
		{
			bool operator()(const UIElementContext& left, const UIElementContext& right) const;
		};

		using iterator = std::multiset<UIElementContext, ZIndexComparer>::iterator;
		using const_iterator = std::multiset<UIElementContext, ZIndexComparer>::const_iterator;

		const UIElementContext* HoverElementContext = nullptr;
		VisualElement& Parent;
	private:
		std::multiset<UIElementContext, ZIndexComparer> _items;
	public:
		UIElementsCollection(VisualElement& parent);
		
		iterator begin() noexcept
		{
			return _items.begin();
		}

		const_iterator begin() const noexcept
		{
			return _items.begin();
		}

		iterator end() noexcept
		{
			return _items.end();
		}

		const_iterator end() const noexcept
		{
			return _items.end();
		}
	protected:
		void OnMouseMove(const MouseButtonEventArgs& e) override;
		void OnMouseDown(const MouseButtonEventArgs& e) override;
		void OnMouseUp(const MouseButtonEventArgs& e) override;
		void OnMouseDoubleClick(const MouseButtonEventArgs& e) override;
		void OnMouseWheel(const MouseWheelEventArgs& e) override;
		void OnKeyDown(const KeyEventArgs& e) override;
		void OnKeyUp(const KeyEventArgs& e) override;

		friend class Application;
	};
}