#pragma once

#include <functional>
#include "../Render/IVisual.h"
#include "../Events/Event.h"
#include "../Events/Delegates.h"
#include "../Appearance/Style/Style.h"
#include "../Appearance/Style/PropertyManagers.h"

namespace Sgl
{
	class UIElement;

	using UIEventHandler = EventHandler<UIElement, EventArgs>;
	using MouseEventHandler = EventHandler<UIElement, MouseButtonEventArgs>;
	using MouseWheelEventHandler = EventHandler<UIElement, MouseWheelEventArgs>;
	using KeyEventHandler = EventHandler<UIElement, KeyEventArgs>;

	class UIElement: public IVisual
	{
	public:
		static inline const EventId MouseDownEvent = EventManager::Register<MouseEventHandler>("MouseDown");
		static inline const EventId MouseUpEvent = EventManager::Register<MouseEventHandler>("MouseUp");
		static inline const EventId MouseMoveEvent = EventManager::Register<MouseEventHandler>("MouseMove");
		static inline const EventId MouseWheelEvent = EventManager::Register<MouseWheelEventHandler>("MouseWheel");
		static inline const EventId KeyDownEvent = EventManager::Register<KeyEventHandler>("KeyDown");
		static inline const EventId KeyUpEvent = EventManager::Register<KeyEventHandler>("KeyUp");

		std::unordered_map<std::string, Any> Resources;
	protected:
		static inline std::unordered_map<EventId, Action<Any&, const Any&>> _eventInitializersMap;
	public:
		UIElement() = default;
		virtual ~UIElement() = default;

		virtual void SetStyle(const Style& style);

		Event<MouseEventHandler> MouseDown;
		Event<MouseEventHandler> MouseUp;
		Event<MouseEventHandler> MouseMove;
		Event<MouseWheelEventHandler> MouseWheel;
		Event<KeyEventHandler> KeyDown;
		Event<KeyEventHandler> KeyUp;
	protected:
		virtual void OnMouseDown(const MouseButtonEventArgs& e);
		virtual void OnMouseUp(const MouseButtonEventArgs& e);
		virtual void OnMouseMove(const MouseButtonEventArgs& e);
		virtual void OnMouseWheel(const MouseWheelEventArgs& e);
		virtual void OnKeyDown(const KeyEventArgs& e);
		virtual void OnKeyUp(const KeyEventArgs& e);
	};
}