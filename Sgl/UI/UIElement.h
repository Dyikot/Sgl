#pragma once

#include "../Style/Layout.h"
#include "../Style/Trigger.h"
#include "../Events/Event.h"
#include "../Events/MouseAndKeyEvents.h"
#include "../Visual/VisualElement.h"
#include "../Data/Object.h"
#include "../Visual/Renderer.h"

namespace Sgl::UI
{
	class UIElement: 
		public VisualElement,
		public IMouseEventsListener,
		public IKeyEventsListener
	{
	public:
		Trigger OnHover;
		Trigger OnMousePressed;

		Event<UIElement&> SizeChanged;
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

		RenderFragment _tooltip;
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

		void SetTooltip(RenderFragment value) { _tooltip = std::move(value); }
		// TODO: Create tooltip for text
		void SetTooltip(const std::string& text);

		void OnRender(RenderContext rc) const override;
		void ApplyDefaultStyle() override;
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

		void OnKeyUp(const KeyEventArgs& e) {}
		void OnKeyDown(const KeyEventArgs& e) {}
		void OnMouseMove(const MouseButtonEventArgs& e) {}
		void OnMouseWheel(const MouseWheelEventArgs& e) {}
		void OnMouseDown(const MouseButtonEventArgs& e);
		void OnMouseUp(const MouseButtonEventArgs& e);
		virtual void OnMouseEnter(FPoint position);
		virtual void OnMouseLeave(FPoint position);
		virtual void OnSizeChanged();

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