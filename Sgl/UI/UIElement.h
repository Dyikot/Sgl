#pragma once

#include "../Style/Layout.h"
#include "../Style/Style.h"
#include "../Events/Event.h"
#include "../Events/MouseAndKeyEvents.h"
#include "../Visual/VisualElement.h"
#include "../Data/Object.h"
#include "../Visual/Renderer.h"
#include "ToolTip.h"

namespace Sgl::UI
{
	class UIElement: public VisualElement, public IMouseEventsListener, public IKeyEventsListener
	{
	public:
		using KeyEventHandler = EventHandler<UIElement, KeyEventArgs>;
		using MouseEventHandler = EventHandler<UIElement, MouseEventArgs>;
		using MouseButtonEventHandler = EventHandler<UIElement, MouseButtonEventArgs>;
		using MouseWheelEventHandler = EventHandler<UIElement, MouseWheelEventArgs>;
		using SizeChangedEventHandler = EventHandler<UIElement, EventArgs>;

		Event<KeyEventHandler> KeyUp;
		Event<KeyEventHandler> KeyDown;
		Event<MouseEventHandler> MouseMove;
		Event<MouseEventHandler> MouseEnter;
		Event<MouseEventHandler> MouseLeave;
		Event<MouseButtonEventHandler> MouseUp;
		Event<MouseButtonEventHandler> MouseDown;
		Event<MouseWheelEventHandler> MouseWheel;
		Event<SizeChangedEventHandler> SizeChanged;
	private:
		float _width;
		float _height;
		float _minWidth;
		float _minHeight;
		float _maxWidth;
		float _maxHeight;
		size_t _zIndex;
		Thickness _margin;
		Visibility _visibility;
		VerticalAlignment _verticalAlignment;
		HorizontalAlignment _horizontalAlignment;
		float _actualWidth;
		float _actualHeight;
		FPoint _position;
		FPoint _actualPosition;
	public:
		UIElement();
		UIElement(const UIElement& other);
		UIElement(UIElement&& other) noexcept;
		virtual ~UIElement() = default;

		void SetWidth(float value)
		{
			_width = value; 
			_actualWidth = value - _margin.Right;
			OnSizeChanged();
		}
		float GetWidth() const { return _width; }

		void SetHeight(float value)
		{
			_height = value;
			_actualHeight = value - _margin.Bottom;
			OnSizeChanged();
		}
		float GetHeight() const { return _height; }

		void SetMinWidth(float value) { _minWidth = value; }
		float GetMinWidth() const { return _minWidth; }

		void SetMinHeight(float value) { _minHeight = value; }
		float GetMinHeight() const { return _minHeight; }

		void SetMaxWidth(float value) { _maxWidth = value; }
		float GetMaxWidth() const { return _maxWidth; }

		void SetMaxHeight(float value) { _maxHeight = value; }
		float GetMaxHeight() const { return _maxHeight; }

		void SetZIndex(size_t value) { _zIndex = value; }
		size_t GetZIndex() const { return _zIndex; }

		void SetVisibility(Visibility value) { _visibility = value; }
		Visibility GetVisibility() const { return _visibility; }

		void SetVerticalAlignment(VerticalAlignment value) { _verticalAlignment = value; }
		VerticalAlignment GetVerticalAlignment() const { return _verticalAlignment; }

		void SetHorizontalAlignment(HorizontalAlignment value) { _horizontalAlignment = value; }
		HorizontalAlignment GetHorizontalAlignment() const { return _horizontalAlignment; }

		void SetMargin(const Thickness& value) { _margin = value; }
		const Thickness& GetMargin() const { return _margin; }

		float GetActualWidth() const { return _actualWidth; }
		float GetActualHeight() const { return _actualHeight; }

		void SetTooltip(const ToolTip& tooltip);

		void OnRender(RenderContext rc) const override;
	protected:
		void SetPosition(FPoint value)
		{
			_position = value;
			_actualPosition = FPoint
			{
				.x = static_cast<float>(value.x + _margin.Left),
				.y = static_cast<float>(value.y + _margin.Top)
			};
		}
		FPoint GetPosition() const { return _position; }
		FPoint GetActualPosition() const { return _actualPosition; }

		void OnKeyUp(const KeyEventArgs& e) { KeyUp.TryRaise(*this, e); }
		void OnKeyDown(const KeyEventArgs& e) { KeyDown.TryRaise(*this, e); }
		void OnMouseMove(const MouseEventArgs& e) { MouseMove.TryRaise(*this, e); }
		void OnMouseDown(const MouseButtonEventArgs& e) { MouseDown.TryRaise(*this, e); }
		void OnMouseUp(const MouseButtonEventArgs& e) { MouseUp.TryRaise(*this, e); }
		void OnMouseWheel(const MouseWheelEventArgs& e) { MouseWheel.TryRaise(*this, e); }
		virtual void OnMouseEnter(const MouseEventArgs& e) { MouseEnter.TryRaise(*this, e); }
		virtual void OnMouseLeave(const MouseEventArgs& e) { MouseLeave.TryRaise(*this, e); }
		virtual void OnSizeChanged() { SizeChanged.TryRaise(*this, EmptyEventArgs); }

		friend class Layout;
	};

	struct UIElementComparer
	{
		bool operator()(const shared_ptr<UIElement>& left, const shared_ptr<UIElement>& right) const
		{
			return left->GetZIndex() < right->GetZIndex();
		}
	};
}