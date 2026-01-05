#pragma once

#include <type_traits>

#include "../Base/Any.h"
#include "../Base/Event.h"
#include "../Data/ObservableObject.h"
#include "../Data/IDataTemplate.h"
#include "../Input/MouseAndKeyEventArgs.h"
#include "../Layout/Layoutable.h"

namespace Sgl
{
	class UIElement : public Layoutable
	{
	private:
		using UIElementEventHandler = EventHandler<UIElement>;
		using KeyEventHandler = EventHandler<UIElement, KeyEventArgs>;
		using MouseMoveEventHandler = EventHandler<UIElement, MouseMoveEventArgs>;
		using MouseButtonEventHandler = EventHandler<UIElement, MouseButtonEventArgs>;
		using MouseWheelEventHandler = EventHandler<UIElement, MouseWheelEventArgs&>;
	public:
		Event<KeyEventHandler> KeyUp;
		Event<KeyEventHandler> KeyDown;
		Event<MouseMoveEventHandler> MouseMove;
		Event<MouseMoveEventHandler> MouseEnter;
		Event<MouseMoveEventHandler> MouseLeave;
		Event<MouseButtonEventHandler> MouseUp;
		Event<MouseButtonEventHandler> MouseDown;
		Event<MouseWheelEventHandler> MouseWheel;		
	private:		
		Any _tag;		
		Ref<UIElement> _tooltip;		

		bool _isMouseOver = false;

		ValueSource _tagSource {};
		ValueSource _tooltipSource {};
	public:
		UIElement() = default;
		UIElement(const UIElement& other);
		UIElement(UIElement&& other) noexcept;
		virtual ~UIElement() = default;
		
		void SetTag(const Any& value);
		void SetTag(const Any& value, ValueSource source);
		const Any& GetTag() const { return _tag; }		

		void SetToolTip(const Ref<UIElement>& value);
		void SetToolTip(const Ref<UIElement>& value, ValueSource source);
		const Ref<UIElement>& GetToolTip() const { return _tooltip; }		

		bool IsMouseOver() const { return _isMouseOver; }

		void Render(RenderContext context) override;		
	protected:
		void RenderBackground(RenderContext context);
		void OnCursorChanged(const Cursor& cursor) override;
		virtual void OnKeyUp(KeyEventArgs e);
		virtual void OnKeyDown(KeyEventArgs e);
		virtual void OnMouseMove(MouseMoveEventArgs e);
		virtual void OnMouseDown(MouseButtonEventArgs e);
		virtual void OnMouseUp(MouseButtonEventArgs e);
		virtual void OnMouseWheelChanged(MouseWheelEventArgs& e);
		virtual void OnMouseEnter(MouseMoveEventArgs e);
		virtual void OnMouseLeave(MouseMoveEventArgs e);
		void OnAttachedToLogicalTree() override;
		void OnDetachedFromLogicalTree() override;
	public:
		static inline StyleableProperty TagProperty { &SetTag, &GetTag };
		static inline StyleableProperty ToolTipProperty { &SetToolTip, &GetToolTip };

		friend class Panel;
		friend class Window;
		friend class ContentUIElement;
	};

	class UIElementDataTemplate : public IDataTemplate
	{
	public:
		Ref<UIElement> Build(const Ref<INotifyPropertyChanged>& data) override;
		bool Match(const Ref<INotifyPropertyChanged>& data) const override;
	};
}