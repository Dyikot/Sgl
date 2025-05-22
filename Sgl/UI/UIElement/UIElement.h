#pragma once

#include <set>
#include "../IVisual.h"
#include "UIElementsCollection.h"
#include "../../Events/Event.h"
#include "../../Style/Trigger.h"

namespace Sgl
{
	template<std::derived_from<IUIElement> TStyleTargetType = IUIElement>
	class UIElement: public IUIElement
	{
	public:
		Style<TStyleTargetType> ClassStyle;

		Trigger<TStyleTargetType> OnHover;
		Trigger<TStyleTargetType> OnMousePressed;

		Event<KeyEventHandler> KeyUp;
		Event<KeyEventHandler> KeyDown;
		Event<MouseEventHandler> MouseDown;
		Event<MouseEventHandler> MouseUp;
		Event<MouseEventHandler> MouseMove;
		Event<MouseEventHandler> MouseEnter;
		Event<MouseEventHandler> MouseLeave;
		Event<MouseEventHandler> MouseDoubleClick;
		Event<MouseWheelEventHandler> MouseWheel;
	protected:
		StyleProperties<TStyleTargetType> _properties;
		FPoint _position;
		UIElementsCollection _children;
	private:
		bool _isHover = false;
	public:
		UIElement():
			ClassStyle(_properties),
			OnHover(ClassStyle),
			OnMousePressed(ClassStyle),
			_position(),
			_children(*this)
		{}

		virtual ~UIElement() = default;

		UIElementsCollection& Children() override { return _children; }

		void SetPosition(FPoint value) final { _position = value; }
		FPoint GetPosition() const final { return _position; }

		void SetWidth(float value) final { _properties.Width = value; }
		float GetWidth() const final { return _properties.Width; }

		void SetHeight(float value) final { _properties.Height = value; }
		float GetHeight() const final { return _properties.Height; }

		void SetMinWidth(float value) final { _properties.MinWidth = value; }
		float GetMinWidth() const final { return _properties.MinWidth; }

		void SetMinHeight(float value) final { _properties.MinHeight = value; }
		float GetMinHeight() const final { return _properties.MinHeight; }

		void SetMaxWidth(float value) final { _properties.MaxWidth = value; }
		float GetMaxWidth() const final { return _properties.MaxWidth; }

		void SetMaxHeight(float value) final { _properties.MaxHeight = value; }
		float GetMaxHeight() const final { return _properties.MaxHeight; }

		void SetZIndex(size_t value) final { _properties.ZIndex = value; }
		size_t GetZIndex() const final { return _properties.ZIndex; }

		void SetMargin(Thickness value) final { _properties.Margin = value; }
		Thickness GetMargin() const final { return _properties.Margin; }

		void SetPadding(Thickness value) final { _properties.Padding = value; }
		Thickness GetPadding() const final { return _properties.Padding; }

		void SetBorder(Border value) final { _properties.Border = value; }
		Border GetBorder() const final { return _properties.Border; }

		void SetFont(const Font& value) final { _properties.Font = value; }
		const Font& GetFont() const final { return _properties.Font; }

		void SetCursor(Cursor::Getter value) final { _properties.Cursor = value; }
		Cursor::Getter GetCursor() const final { return _properties.Cursor; }

		void SetBackgroundColor(Color value) final { _properties.BackgroundColor = value; }
		Color GetBackgroundColor() const final { return _properties.BackgroundColor; }

		void SetTooltip(IUIElement* value) final { _properties.Tooltip = value; }
		IUIElement* GetTooltip() const final { return _properties.Tooltip; }

		void SetBackgroundTexture(Texture* value) final { _properties.BackgroundTexture = value; }
		Texture* GetBackgroundTexture() const final { return _properties.BackgroundTexture; }

		void SetVisibility(Visibility value) final { _properties.Visibility = value; }
		Visibility GetVisibility() const final { return _properties.Visibility; }

		void SetVerticalAlignment(VerticalAlignment value) final { _properties.VerticalAlignment = value; }
		VerticalAlignment GetVerticalAlignment() const final { return _properties.VerticalAlignment; }

		void SetHorizontalAlignment(HorizontalAlignment value) final { _properties.HorizontalAlignment = value; }
		HorizontalAlignment GetHorizontalAlignment() const final { return _properties.HorizontalAlignment; }

		void OnRender(RenderContext renderContext) const override
		{
			for(IUIElement& child : _children)
			{
				child.OnRender(renderContext);
			}

			if(_isHover && _properties.Tooltip)
			{
				_properties.Tooltip->OnRender(renderContext);
			}
		}
	protected:
		virtual void OnMouseDown(const MouseButtonEventArgs& e) override
		{
			OnMousePressed.Activate();
			MouseDown.TryRaise(*this, e);
		}

		virtual void OnMouseUp(const MouseButtonEventArgs& e) override
		{
			OnMousePressed.Deactivate();
			MouseUp.TryRaise(*this, e);
		}

		virtual void OnMouseMove(const MouseButtonEventArgs& e) override
		{
			MouseMove.TryRaise(*this, e);
		}

		void OnMouseEnter(const MouseButtonEventArgs& e) override
		{
			_isHover = true;
			OnHover.Activate();
			MouseEnter.TryRaise(*this, e);
		}

		void OnMouseLeave(const MouseButtonEventArgs& e) override
		{
			_isHover = false;
			OnHover.Deactivate();
			MouseLeave.TryRaise(*this, e);
		}

		virtual void OnMouseDoubleClick(const MouseButtonEventArgs& e) override
		{
			MouseDoubleClick.TryRaise(*this, e);
		}

		virtual void OnMouseWheel(const MouseWheelEventArgs& e) override
		{
			MouseWheel.TryRaise(*this, e);
		}

		virtual void OnKeyDown(const KeyEventArgs& e) override
		{
			KeyDown.TryRaise(*this, e);
		}

		virtual void OnKeyUp(const KeyEventArgs& e) override
		{
			KeyUp.TryRaise(*this, e);
		}
	};
}