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
		StyleProperties<TStyleTargetType> Properties;

		Style<TStyleTargetType> ClassStyle;
		Trigger<TStyleTargetType> OnHover;

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
		FPoint _position;
		UIElementsCollection _children;
	private:
		bool _isHover = false;
	public:
		UIElement():
			ClassStyle(Properties),
			OnHover(ClassStyle),
			_position(),
			_children(*this)
		{}

		virtual ~UIElement() = default;

		void OnRender(RenderContext renderContext) const override
		{
			for(IUIElement& child : _children)
			{
				child.OnRender(renderContext);
			}

			if(_isHover && Properties.Tooltip)
			{
				Properties.Tooltip->OnRender(renderContext);
			}
		}

		UIElementsCollection& Children() override { return _children; }

		void SetPosition(FPoint value) override { _position = value; }
		FPoint GetPosition() const override { return _position; }

		void SetWidth(float value) override { Properties.Width = value; }
		float GetWidth() const override { return Properties.Width; }

		void SetHeight(float value) override { Properties.Height = value; }
		float GetHeight() const override { return Properties.Height; }

		void SetMinWidth(float value) override { Properties.MinWidth = value; }
		float GetMinWidth() const override { return Properties.MinWidth; }

		void SetMinHeight(float value) override { Properties.MinHeight = value; }
		float GetMinHeight() const override { return Properties.MinHeight; }

		void SetMaxWidth(float value) override { Properties.MaxWidth = value; }
		float GetMaxWidth() const override { return Properties.MaxWidth; }

		void SetMaxHeight(float value) override { Properties.MaxHeight = value; }
		float GetMaxHeight() const override { return Properties.MaxHeight; }

		void SetZIndex(size_t value) override { Properties.ZIndex = value; }
		size_t GetZIndex() const override { return Properties.ZIndex; }

		void SetMargin(Thickness value) override { Properties.Margin = value; }
		Thickness GetMargin() const override { return Properties.Margin; }

		void SetPadding(Thickness value) override { Properties.Padding = value; }
		Thickness GetPadding() const override { return Properties.Padding; }

		void SetBorder(Border value) override { Properties.Border = value; }
		Border GetBorder() const override { return Properties.Border; }

		void SetFont(const Font& value) override { Properties.Font = value; }
		const Font& GetFont() const override { return Properties.Font; }

		void SetCursor(Cursor::Getter value) override { Properties.Cursor = value; }
		Cursor::Getter GetCursor() const override { return Properties.Cursor; }

		void SetBackgroundColor(Color value) override { Properties.BackgroundColor = value; }
		Color GetBackgroundColor() const override { return Properties.BackgroundColor; }

		void SetTooltip(IUIElement* value) override { Properties.Tooltip = value; }
		IUIElement* GetTooltip() const override { return Properties.Tooltip; }

		void SetBackgroundTexture(Texture* value) override { Properties.BackgroundTexture = value; }
		Texture* GetBackgroundTexture() const override { return Properties.BackgroundTexture; }

		void SetVisibility(Visibility value) override { Properties.Visibility = value; }
		Visibility GetVisibility() const override { return Properties.Visibility; }

		void SetVerticalAlignment(VerticalAlignment value) override { Properties.VerticalAlignment = value; }
		VerticalAlignment GetVerticalAlignment() const override { return Properties.VerticalAlignment; }

		void SetHorizontalAlignment(HorizontalAlignment value) override { Properties.HorizontalAlignment = value; }
		HorizontalAlignment GetHorizontalAlignment() const override { return Properties.HorizontalAlignment; }
	protected:
		virtual void OnMouseDown(const MouseButtonEventArgs& e) override
		{
			MouseDown.TryRaise(*this, e);
		}

		virtual void OnMouseUp(const MouseButtonEventArgs& e) override
		{
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