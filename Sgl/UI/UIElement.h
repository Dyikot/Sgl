#pragma once

#include "../Base/Object.h"
#include "../Base/Event.h"
#include "../Base/Collections/ResourcesMap.h"
#include "../Input/Cursor.h"
#include "../Input/MouseAndKeyEventArgs.h"
#include "../Render/IRenderable.h"
#include "Layoutable.h"

namespace Sgl
{
	class Scene;

	class UIElement: public Layoutable, public IRenderable
	{
	private:
		using ColorProperty = StylyableProperty<Color>;
		using CursorProperty = StylyableProperty<std::reference_wrapper<const Cursor>, const Cursor&>;
		using TextureProperty = StylyableProperty<shared_ptr<Texture>>;
		using TagProperty = StylyableProperty<object, const object&>;
		using ToolTipProperty = StylyableProperty<shared_ptr<UIElement>>;
		using ZIndexProperty = StylyableProperty<size_t>;

		using KeyEventHandler = EventHandler<UIElement, KeyEventArgs>;
		using MouseEventHandler = EventHandler<UIElement, MouseEventArgs>;
		using MouseButtonEventHandler = EventHandler<UIElement, MouseButtonEventArgs>;
		using MouseWheelEventHandler = EventHandler<UIElement, MouseWheelEventArgs>;
	public:		
		Event<KeyEventHandler> KeyUp;
		Event<KeyEventHandler> KeyDown;
		Event<MouseEventHandler> MouseMove;
		Event<MouseEventHandler> MouseEnter;
		Event<MouseEventHandler> MouseLeave;
		Event<MouseButtonEventHandler> MouseUp;
		Event<MouseButtonEventHandler> MouseDown;
		Event<MouseWheelEventHandler> MouseWheel;
		
		CursorProperty Cursor;
		TextureProperty BackgroundTexture;
		ColorProperty BackgroundColor;
		TagProperty Tag;
		ToolTipProperty ToolTip;
		ZIndexProperty ZIndex;

		ResourcesMap Resources;
	protected:
		bool _isMouseOver;
	public:
		UIElement();
		UIElement(const UIElement& other);
		UIElement(UIElement&& other) noexcept;
		virtual ~UIElement() = default;
		
		Scene& GetScene();
		bool IsMouseOver() const { return _isMouseOver; }

		void OnRender(RenderContext context) const override;
	protected:
		virtual void OnKeyUp(const KeyEventArgs& e)
		{
			KeyUp.TryInvoke(*this, e);
		}

		virtual void OnKeyDown(const KeyEventArgs& e)
		{
			KeyDown.TryInvoke(*this, e);
		}

		virtual void OnMouseMove(const MouseEventArgs& e)
		{
			MouseMove.TryInvoke(*this, e);
		}

		virtual void OnMouseDown(const MouseButtonEventArgs& e)
		{
			MouseDown.TryInvoke(*this, e);
		}

		virtual void OnMouseUp(const MouseButtonEventArgs& e)
		{ 
			MouseUp.TryInvoke(*this, e);
		}

		virtual void OnMouseWheelChanged(const MouseWheelEventArgs& e)
		{ 
			MouseWheel.TryInvoke(*this, e);
		}

		virtual void OnMouseEnter(const MouseEventArgs& e)
		{
			_isMouseOver = true;
			Cursor::Set(Cursor);
			MouseEnter.TryInvoke(*this, e);
		}

		virtual void OnMouseLeave(const MouseEventArgs& e)
		{
			MouseLeave.TryInvoke(*this, e);
			_isMouseOver = false;
		}
		
		friend class Scene;
		friend class Panel;
		friend class ContentUIElement;
		friend class UIElementsCollection;
	};

	struct UIElementComparer
	{
		bool operator()(const shared_ptr<UIElement>& left, const shared_ptr<UIElement>& right) const
		{
			return left->ZIndex < right->ZIndex;
		}
	};
}