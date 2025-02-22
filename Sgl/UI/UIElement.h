#pragma once

#include <functional>
#include "../Render/IVisual.h"
#include "../Events/Event.h"
#include "../Appearance/Style.h"
#include "../Binding/BindableObject.h"

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
		Color BackgroundColor = Colors::Black;
		Texture* BackgroundTexture = nullptr;
		AnyMap<std::string> Resources;
	public:
		virtual ~UIElement() = default;

		Event<MouseEventHandler> MouseDown;
		Event<MouseEventHandler> MouseUp;
		Event<MouseEventHandler> MouseMove;
		Event<MouseWheelEventHandler> MouseWheel;
		Event<KeyEventHandler> KeyDown;
		Event<KeyEventHandler> KeyUp;
		Event<MouseEventHandler> MouseDoubleClick;

		virtual void SetStyle(const Style& style);
	protected:
		virtual void OnMouseDown(const MouseButtonEventArgs& e);
		virtual void OnMouseUp(const MouseButtonEventArgs& e);
		virtual void OnMouseMove(const MouseButtonEventArgs& e);
		virtual void OnMouseWheel(const MouseWheelEventArgs& e);
		virtual void OnKeyDown(const KeyEventArgs& e);
		virtual void OnKeyUp(const KeyEventArgs& e);
		virtual void OnMouseDoubleClick(const MouseButtonEventArgs& e);
	};
}