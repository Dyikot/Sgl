#pragma once

#include <functional>
#include "../Graphic/IVisual.h"
#include "../Events/Event.h"
#include "../Style/Style.h"
#include "../Binding/BindableObject.h"
#include "../Graphic/Cursor.h"

namespace Sgl
{
	class UIElement;

	using UIEventHandler = EventHandler<UIElement, EventArgs>;
	using MouseEventHandler = EventHandler<UIElement, MouseButtonEventArgs>;
	using MouseWheelEventHandler = EventHandler<UIElement, MouseWheelEventArgs>;
	using KeyEventHandler = EventHandler<UIElement, KeyEventArgs>;

	class UIElement: public BindableObject, public IVisual
	{
	public:
		static inline const PropertyId CursorProperty = PropertyManager::Register<std::reference_wrapper<const Cursor>>("Cursor");

		Color BackgroundColor = Colors::Black;
		Texture* BackgroundTexture = nullptr;
		AnyMap<std::string> Resources;
	public:
		UIElement();
		virtual ~UIElement() = default;

		Event<MouseEventHandler> MouseDown;
		Event<MouseEventHandler> MouseUp;
		Event<MouseEventHandler> MouseMove;
		Event<MouseEventHandler> MouseDoubleClick;
		Event<MouseWheelEventHandler> MouseWheel;
		Event<KeyEventHandler> KeyDown;
		Event<KeyEventHandler> KeyUp;

		void SetCursor(const Sgl::Cursor& value) { SetProperty(CursorProperty, std::ref(value)); }

		const Cursor& GetCursor() const { return GetPropertyValue<std::reference_wrapper<const Cursor>>(CursorProperty); }

		virtual void AddStyle(const Style& style);
		virtual void SwitchCursorOn(const Cursor& cursor) = 0;
		virtual void SwitchCursorOnDefault() = 0;
	protected:
		virtual void OnMouseDown(const MouseButtonEventArgs& e);
		virtual void OnMouseUp(const MouseButtonEventArgs& e);
		virtual void OnMouseMove(const MouseButtonEventArgs& e);
		virtual void OnMouseDoubleClick(const MouseButtonEventArgs& e);
		virtual void OnMouseWheel(const MouseWheelEventArgs& e);
		virtual void OnKeyDown(const KeyEventArgs& e);
		virtual void OnKeyUp(const KeyEventArgs& e);
	};
}