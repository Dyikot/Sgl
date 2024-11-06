#pragma once

#include <functional>
#include "../Render/IVisual.h"
#include "../Events/Event.h"
#include "../Appearance/Style/Style.h"

namespace Sgl
{
	using EventHandler = std::function<void(IVisual*, const EventArgs&)>;
	using MouseEventHandler = std::function<void(IVisual*, const MouseButtonEventArgs&)>;
	using KeyEventHandler = std::function<void(IVisual*, const KeyEventArgs&)>;

	class UIElement: public IVisual
	{
	public:
		static inline const PropertyId MouseDownProperty = PropertyManager::Register<MouseEventHandler>("MouseDown");
		static inline const PropertyId MouseUpProperty = PropertyManager::Register<MouseEventHandler>("MouseUp");
		static inline const PropertyId MouseMoveProperty = PropertyManager::Register<MouseEventHandler>("MouseMove");
		static inline const PropertyId MouseEnterProperty = PropertyManager::Register<MouseEventHandler>("MouseEnter");
		static inline const PropertyId MouseLeaveProperty = PropertyManager::Register<MouseEventHandler>("MouseLeave");
		static inline const PropertyId KeyDownProperty = PropertyManager::Register<KeyEventHandler>("KeyDown");
		static inline const PropertyId KeyUpProperty = PropertyManager::Register<KeyEventHandler>("KeyUp");
	public:
		virtual ~UIElement() = default;

		virtual void SetStyle(const Style& style);

		Event<MouseEventHandler> MouseDown;
		Event<MouseEventHandler> MouseUp;
		Event<MouseEventHandler> MouseMove;
		Event<MouseEventHandler> MouseEnter;
		Event<MouseEventHandler> MouseLeave;
		Event<KeyEventHandler> KeyDown;
		Event<KeyEventHandler> KeyUp;
	protected:
		virtual void OnMouseDown(const MouseButtonEventArgs& e);
		virtual void OnMouseUp(const MouseButtonEventArgs& e);
		virtual void OnMouseMove(const MouseButtonEventArgs& e);
		virtual void OnMouseEnter(const MouseButtonEventArgs& e);
		virtual void OnMouseLeave(const MouseButtonEventArgs& e);
		virtual void OnKeyDown(const KeyEventArgs& e);
		virtual void OnKeyUp(const KeyEventArgs& e);
	};
}