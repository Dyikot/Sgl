#pragma once

#include <functional>
#include "../Render/IVisual.h"
#include "../Events/EventArgs.h"
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

		static inline const PropertyId MouseDownProperty = PropertyManager::Register<MouseEventHandler>("MouseDown");
		static inline const PropertyId MouseUpProperty = PropertyManager::Register<MouseEventHandler>("MouseUp");
		static inline const PropertyId MouseMoveProperty = PropertyManager::Register<MouseEventHandler>("MouseMove");
		static inline const PropertyId MouseEnterProperty = PropertyManager::Register<MouseEventHandler>("MouseEnter");
		static inline const PropertyId MouseLeaveProperty = PropertyManager::Register<MouseEventHandler>("MouseLeave");
		static inline const PropertyId KeyDownProperty = PropertyManager::Register<KeyEventHandler>("KeyDown");
		static inline const PropertyId KeyUpProperty = PropertyManager::Register<KeyEventHandler>("KeyUp");

		std::unordered_map<std::string, Style> StyleMap;
	public:
		UIElement() = default;
		UIElement(const Style& style) noexcept;
		virtual ~UIElement() = default;

		Event<MouseEventHandler> MouseDown;
		Event<MouseEventHandler> MouseUp;
		Event<MouseEventHandler> MouseMove;
		Event<MouseEventHandler> MouseEnter;
		Event<MouseEventHandler> MouseLeave;
		Event<MouseWheelEventHandler> MouseWheel;
		Event<KeyEventHandler> KeyDown;
		Event<KeyEventHandler> KeyUp;
	protected:
		virtual void OnMouseDown(const MouseButtonEventArgs& e);
		virtual void OnMouseUp(const MouseButtonEventArgs& e);
		virtual void OnMouseMove(const MouseButtonEventArgs& e);
		virtual void OnMouseEnter(const MouseButtonEventArgs& e);
		virtual void OnMouseLeave(const MouseButtonEventArgs& e);
		virtual void OnMouseWheel(const MouseWheelEventArgs& e);
		virtual void OnKeyDown(const KeyEventArgs& e);
		virtual void OnKeyUp(const KeyEventArgs& e);
	};
}