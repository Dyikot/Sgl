#pragma once

#include <set>
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

		void SetPosition(FPoint value) override { _position = value; }
		FPoint GetPosition() const override { return _position; }

		void SetWidth(float value) override { _properties.Width = value; }
		float GetWidth() const override { return _properties.Width; }

		void SetHeight(float value) override { _properties.Height = value; }
		float GetHeight() const override { return _properties.Height; }

		void SetMinWidth(float value) override { _properties.MinWidth = value; }
		float GetMinWidth() const override { return _properties.MinWidth; }

		void SetMinHeight(float value) override { _properties.MinHeight = value; }
		float GetMinHeight() const override { return _properties.MinHeight; }

		void SetMaxWidth(float value) override { _properties.MaxWidth = value; }
		float GetMaxWidth() const override { return _properties.MaxWidth; }

		void SetMaxHeight(float value) override { _properties.MaxHeight = value; }
		float GetMaxHeight() const override { return _properties.MaxHeight; }

		void SetZIndex(size_t value) override { _properties.ZIndex = value; }
		size_t GetZIndex() const override { return _properties.ZIndex; }

		void SetMargin(Thickness value) override { _properties.Margin = value; }
		Thickness GetMargin() const override { return _properties.Margin; }

		void SetPadding(Thickness value) override { _properties.Padding = value; }
		Thickness GetPadding() const override { return _properties.Padding; }

		void SetBorder(Border value) override { _properties.Border = value; }
		Border GetBorder() const override { return _properties.Border; }

		void SetFont(const Font& value) override { _properties.Font = value; }
		const Font& GetFont() const override { return _properties.Font; }

		void SetCursor(Cursor::Getter value) override { _properties.Cursor = value; }
		Cursor::Getter GetCursor() const override { return _properties.Cursor; }

		void SetBackgroundColor(Color value) override { _properties.BackgroundColor = value; }
		Color GetBackgroundColor() const override { return _properties.BackgroundColor; }

		void SetTooltip(IUIElement* value) override { _properties.Tooltip = value; }
		IUIElement* GetTooltip() const override { return _properties.Tooltip; }

		void SetBackgroundTexture(Texture* value) override { _properties.BackgroundTexture = value; }
		Texture* GetBackgroundTexture() const override { return _properties.BackgroundTexture; }

		void SetVisibility(Visibility value) override { _properties.Visibility = value; }
		Visibility GetVisibility() const override { return _properties.Visibility; }

		void SetVerticalAlignment(VerticalAlignment value) override { _properties.VerticalAlignment = value; }
		VerticalAlignment GetVerticalAlignment() const override { return _properties.VerticalAlignment; }

		void SetHorizontalAlignment(HorizontalAlignment value) override { _properties.HorizontalAlignment = value; }
		HorizontalAlignment GetHorizontalAlignment() const override { return _properties.HorizontalAlignment; }

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