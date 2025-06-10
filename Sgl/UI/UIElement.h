#pragma once

#include "../Style/Trigger.h"
#include "../Style/Layout.h"
#include "../Events/Event.h"
#include "../Events/MouseAndKeyEvents.h"
#include "../Visual/VisualElement.h"
#include "../Data/Object.h"

namespace Sgl::UI
{
	class UIElement: 
		public VisualElement,
		public IMouseEventsListener,
		public IKeyEventsListener
	{
	public:
		Style Class;

		Trigger OnHover;
		Trigger OnMousePressed;

		Event<UIElement&> SizeChanged;
	private:
		using base = VisualElement;

		float _width;
		float _height;
		float _minWidth;
		float _minHeight;
		float _maxWidth;
		float _maxHeight;
		size_t _zIndex;
		VisualElement* _tooltip;
		Visibility _visibility;
		VerticalAlignment _verticalAlignment;
		HorizontalAlignment _horizontalAlignment;
		Thickness _margin;

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

		void SetTooltip(VisualElement* value) { _tooltip = value; }
		VisualElement* GetTooltip() const { return _tooltip; }

		void SetVisibility(Visibility value) { _visibility = value; }
		Visibility GetVisibility() const { return _visibility; }

		void SetVerticalAlignment(VerticalAlignment value) { _verticalAlignment = value; }
		VerticalAlignment GetVerticalAlignment() const { return _verticalAlignment; }

		void SetHorizontalAlignment(HorizontalAlignment value) { _horizontalAlignment = value; }
		HorizontalAlignment GetHorizontalAlignment() const { return _horizontalAlignment; }

		void SetMargin(const Thickness& value) { _margin = value; }
		Thickness GetMargin() const { return _margin; }

		float GetActualWidth() const { return _actualWidth; }
		float GetActualHeight() const { return _actualHeight; }

		void OnRender(RenderContext rc) const override;
		void ResetStyle() override;
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

		void OnKeyUp(const KeyEventArgs& e) override {}
		void OnKeyDown(const KeyEventArgs& e) override {}
		void OnMouseMove(const MouseButtonEventArgs& e) override {}
		void OnMouseWheel(const MouseWheelEventArgs& e) override {}
		void OnMouseDown(const MouseButtonEventArgs& e) override;
		void OnMouseUp(const MouseButtonEventArgs& e) override;
		virtual void OnMouseEnter(const MouseButtonEventArgs& e);
		virtual void OnMouseLeave(const MouseButtonEventArgs& e);
		virtual void OnSizeChanged();

		friend class Layout;
	};

	struct UIElementComparer
	{
		bool operator()(const std::shared_ptr<UIElement>& left,
						const std::shared_ptr<UIElement>& right) const
		{
			return left->GetZIndex() < right->GetZIndex();
		}
	};
}