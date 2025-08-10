#pragma once

#include <type_traits>
#include "../Base/Any.h"
#include "../Base/Media/Brush.h"
#include "../Base/Observable/Event.h"
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
	protected:
		bool _isMouseOver = false;
	private:		
		std::reference_wrapper<const Cursor> _cursor = Cursors::Arrow;
		Brush _background = Colors::Transparent;
		Any _tag;
		Shared<UIElement> _toolTip;
		size_t _zIndex {};
	public:
		UIElement() = default;
		UIElement(const UIElement& other);
		UIElement(UIElement&& other) noexcept;
		virtual ~UIElement() = default;
		
		void SetCursor(const Cursor& value)
		{
			SetProperty(CursorProperty, _cursor, value);
		}

		const Cursor& GetCursor() const 
		{
			return _cursor;
		}

		void SetBackground(Brush value) 
		{ 
			SetProperty(BackgroundProperty, _background, value);
		}

		Brush GetBackground() const
		{ 
			return _background;
		}

		void SetTag(const Any& value) 
		{ 
			SetProperty(TagProperty, _tag, value);
		}

		const Any& GetTag() const
		{ 
			return _tag;
		}

		void SetToolTip(Shared<UIElement> value) 
		{ 
			SetProperty(ToolTipProperty, _toolTip, value);
		}

		Shared<UIElement> GetToolTip() const 
		{ 
			return _toolTip;
		}

		void SetZIndex(size_t value) 
		{ 
			SetProperty(ZIndexProperty, _zIndex, value);
		}

		size_t GetZIndex() const 
		{ 
			return _zIndex;
		}

		bool IsMouseOver() const 
		{
			return _isMouseOver;
		}

		void Render(RenderContext context) override;
	protected:
		void RenderBackground(RenderContext context) const;

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
			Cursor::Set(_cursor);
			MouseEnter.TryInvoke(*this, e);
		}

		virtual void OnMouseLeave(const MouseEventArgs& e)
		{
			MouseLeave.TryInvoke(*this, e);
			_isMouseOver = false;
		}
	public:
		static inline BindableProperty<UIElement, const Cursor&> CursorProperty =
			BindableProperty<UIElement, const Cursor&>(&SetCursor);

		static inline BindableProperty<UIElement, Brush> BackgroundProperty =
			BindableProperty<UIElement, Brush>(&SetBackground);

		static inline BindableProperty<UIElement, const Any&> TagProperty =
			BindableProperty<UIElement, const Any&>(&SetTag);

		static inline BindableProperty<UIElement, Shared<UIElement>> ToolTipProperty =
			BindableProperty<UIElement, Shared<UIElement>>(&SetToolTip);

		static inline BindableProperty<UIElement, size_t> ZIndexProperty =
			BindableProperty<UIElement, size_t>(&SetZIndex);

		friend class Scene;
		friend class Panel;
		friend class ContentUIElement;
		friend class UIElementsCollection;
	};

	struct UIElementComparer
	{
		bool operator()(const Shared<UIElement>& left, const Shared<UIElement>& right) const
		{
			return left->GetZIndex() < right->GetZIndex();
		}
	};

	struct UIElementDataTemplate
	{
		Shared<UIElement> operator()(const Any& data) const
		{
			return data.As<Shared<UIElement>>();
		}
	};
}