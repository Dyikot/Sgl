#pragma once

#include "../Graphic/IVisual.h"
#include "../Graphic/Cursor.h"
#include "../Events/Event.h"
#include "../Style/Style.h"
#include "../Style/StyleableObject.h"

namespace Sgl
{
	class UIElement;

	using UIElementEventHandler = EventHandler<UIElement, EventArgs>;
	using MouseEventHandler = EventHandler<UIElement, MouseButtonEventArgs>;
	using MouseWheelEventHandler = EventHandler<UIElement, MouseWheelEventArgs>;
	using KeyEventHandler = EventHandler<UIElement, KeyEventArgs>;

	class UIElement: public StyleableObject, public IVisual
	{
	public:
		static inline const PropertyId& CursorProperty = PropertyManager::Register<std::reference_wrapper<const Cursor>>("Cursor");

		Color backgroundColor = Colors::Black;
		Nullable<Texture> backgroundTexture;
		Event<KeyEventHandler> onKeyUp;
		Event<KeyEventHandler> onKeyDown;
		Event<MouseEventHandler> onMouseDown;
		Event<MouseEventHandler> onMouseUp;
		Event<MouseEventHandler> onMouseMove;
		Event<MouseEventHandler> onMouseDoubleClick;
		Event<MouseWheelEventHandler> onMouseWheel;
	public:
		UIElement();
		virtual ~UIElement() = default;

		void SetCursor(const Sgl::Cursor& value) { SetProperty(CursorProperty, std::ref(value)); }
		const Cursor& GetCursor() const { return GetProperty<std::reference_wrapper<const Cursor>>(CursorProperty); }

		virtual void AddStyle(const Style& style);
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