#pragma once

#include "../Data/Object.h"
#include "../Style/Layout.h"
#include "../Style/StyleableElement.h"
#include "../Events/Event.h"
#include "../Events/MouseAndKeyArgs.h"
#include "../Render/Renderer.h"
#include "../Render/IRenderable.h"
#include "../Render/Cursor.h"
#include "ToolTip.h"

namespace Sgl
{
	class UIElement: public StyleableElement, public IRenderable
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
		Cursor::Getter _cursor;	
		Color _backgroundColor;
		shared_ptr<Texture> _backgroundTexture;
		float _width;
		float _height;
		float _minWidth;
		float _minHeight;
		float _maxWidth;
		float _maxHeight;
		bool _isVisible;
		size_t _zIndex;
		Thickness _margin;
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

		void SetCursor(Cursor::Getter value) { _cursor = value; }
		Cursor::Getter GetCursor() const { return _cursor; }

		void SetBackgroundColor(Color value) { _backgroundColor = value; }
		Color GetBackgroundColor() const { return _backgroundColor; }

		void SetBackgroundTexture(shared_ptr<Texture> value) { _backgroundTexture = value; }
		shared_ptr<Texture> GetBackgroundTexture() const { return _backgroundTexture; }

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

		void SetVisibility(bool value) { _isVisible = value; }
		bool IsVisible() const { return _isVisible; }

		void SetZIndex(size_t value) { _zIndex = value; }
		size_t GetZIndex() const { return _zIndex; }

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

		virtual void OnKeyUp(const KeyEventArgs& e) { KeyUp.TryRaise(*this, e); }
		virtual void OnKeyDown(const KeyEventArgs& e) { KeyDown.TryRaise(*this, e); }
		virtual void OnMouseMove(const MouseEventArgs& e) { MouseMove.TryRaise(*this, e); }
		virtual void OnMouseDown(const MouseButtonEventArgs& e) { MouseDown.TryRaise(*this, e); }
		virtual void OnMouseUp(const MouseButtonEventArgs& e) { MouseUp.TryRaise(*this, e); }
		virtual void OnMouseWheel(const MouseWheelEventArgs& e) { MouseWheel.TryRaise(*this, e); }
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