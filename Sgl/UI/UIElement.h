#pragma once

#include <functional>
#include "../Render/IVisual.h"
#include "../Events/EventArgs.h"
#include "../Events/Delegates.h"
#include "../Appearance/Style/Style.h"

namespace Sgl
{	
	class UIElement: public IVisual
	{
	public:
		using UIEventHandler = EventHandler<UIElement, EventArgs>;
		using MouseEventHandler = EventHandler<UIElement, MouseButtonEventArgs>;
		using MouseWheelEventHandler = EventHandler<UIElement, MouseWheelEventArgs>;
		using KeyEventHandler = EventHandler<UIElement, KeyEventArgs>;

		static inline const EventId MouseDownEvent = EventManager::Register<MouseEventHandler>("MouseDown");
		static inline const EventId MouseUpEvent = EventManager::Register<MouseEventHandler>("MouseUp");
		static inline const EventId MouseMoveEvent = EventManager::Register<MouseEventHandler>("MouseMove");
		static inline const EventId MouseWheelEvent = EventManager::Register<MouseWheelEventHandler>("MouseWheel");
		static inline const EventId KeyDownEvent = EventManager::Register<KeyEventHandler>("KeyDown");
		static inline const EventId KeyUpEvent = EventManager::Register<KeyEventHandler>("KeyUp");

		std::unordered_map<std::string, Any> Resources;
	protected:
		static inline std::unordered_map<EventId, Action<Any&, const Any&>> _eventInitializersMap;

		SetterMap<PropertyId> _properties;
		SetterMap<EventId> _events = CreateEvents();
	private:
		static inline bool _isStaticInitialised = false;
	public:
		UIElement();
		UIElement(const Style& style) noexcept;
		virtual ~UIElement() = default;

		Event<MouseEventHandler>& MouseDown = _events[MouseDownEvent].As<Event<MouseEventHandler>>();
		Event<MouseEventHandler>& MouseUp = _events[MouseUpEvent].As<Event<MouseEventHandler>>();
		Event<MouseEventHandler>& MouseMove = _events[MouseMoveEvent].As<Event<MouseEventHandler>>();
		Event<MouseWheelEventHandler>& MouseWheel = _events[MouseWheelEvent].As<Event<MouseWheelEventHandler>>();
		Event<KeyEventHandler>& KeyDown = _events[KeyDownEvent].As<Event<KeyEventHandler>>();
		Event<KeyEventHandler>& KeyUp = _events[KeyDownEvent].As<Event<KeyEventHandler>>();
	protected:
		virtual void OnMouseDown(const MouseButtonEventArgs& e);
		virtual void OnMouseUp(const MouseButtonEventArgs& e);
		virtual void OnMouseMove(const MouseButtonEventArgs& e);
		virtual void OnMouseWheel(const MouseWheelEventArgs& e);
		virtual void OnKeyDown(const KeyEventArgs& e);
		virtual void OnKeyUp(const KeyEventArgs& e);

		void InitEvents(const Style& style);

		template<typename THanlder>
		static void InitEvent(EventId id, Any& event, const Any& handler)
		{
			event.As<Event<THanlder>>() += handler.As<THanlder>();
		}
	private:
		SetterMap<EventId> CreateEvents();
	};
}