#pragma once

#include <set>
#include "../Graphic/IVisual.h"
#include "../Graphic/Color.h"
#include "../Graphic/Texture.h"
#include "../Graphic/Cursor.h"
#include "../Events/Event.h"
#include "../Events/MouseAndKeyEvents.h"
#include "../Appearance/Properties.h"
#include "../Appearance/Style.h"

namespace Sgl
{
	class UIElement;
	class Scene;

	struct ZIndexComparer
	{
		bool operator()(const UIElement& left, const UIElement& right) const;
	};

	class UIElementsCollection:
		public std::multiset<std::reference_wrapper<UIElement>, ZIndexComparer>,
		public IVisual,
		public IMouseEventsListener,
		public IKeyEventsListener
	{
	public:
		UIElement* HoverElement = nullptr;
	public:
		UIElementsCollection() = default;
		void OnRender(RenderContext renderContext) const override;
	protected:
		// TODO: Add Parent cursor
		void OnMouseMove(const MouseButtonEventArgs& e) override; 
		void OnMouseDown(const MouseButtonEventArgs& e) override;
		void OnMouseUp(const MouseButtonEventArgs& e) override;
		void OnMouseDoubleClick(const MouseButtonEventArgs& e) override;
		void OnMouseWheel(const MouseWheelEventArgs& e) override;
		void OnKeyDown(const KeyEventArgs& e) override;
		void OnKeyUp(const KeyEventArgs& e) override;

		friend class Application;
	};

	class UIElement: public IVisual, public IMouseEventsListener, public IKeyEventsListener
	{
	public:
		struct StyleableProperties
		{
			float Width = 0;
			float Height = 0;
			float MinWidth = 0;
			float MinHeight = 0;
			float MaxWidth = std::numeric_limits<float>::max();
			float MaxHeight = std::numeric_limits<float>::max();
			size_t ZIndex = 1;
			Thickness Margin;
			Thickness Padding;
			Border Border =
			{
				.Thickness = Thickness(1),
				.Color = Colors::Black
			};
			Font Font =
			{
				.Size = 14,
				.Family = {},
				.Style = FontStyle::Normal,
				.Weight = FontWeight::Normal,
				.Color = Colors::Black
			};
			Cursor::Getter Cursor = Cursors::Arrow;
			Color BackgroundColor = Colors::Black;
			const UIElement* Tooltip = nullptr;
			const Texture* BackgroundTexture = nullptr;
			Visibility Visibility = Visibility::Visible;
			VerticalAlignment VerticalAlignment = VerticalAlignment::Top;
			HorizontalAlignment HorizontalAlignment = HorizontalAlignment::Left;
		};

		Point Position;
		StyleableProperties Properties;
		Style<StyleableProperties> Style;
		UIElementsCollection Children;

		Event<KeyEventHandler> KeyUp;
		Event<KeyEventHandler> KeyDown;
		Event<MouseEventHandler> MouseDown;
		Event<MouseEventHandler> MouseUp;
		Event<MouseEventHandler> MouseMove;
		Event<MouseEventHandler> MouseEnter;
		Event<MouseEventHandler> MouseLeave;
		Event<MouseEventHandler> MouseDoubleClick;
		Event<MouseWheelEventHandler> MouseWheel;
	private:
		bool _isHover = false;
	public:
		UIElement();
		virtual ~UIElement() = default;

		void OnRender(RenderContext renderContext) const override;
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

		virtual void OnMouseEnter(const MouseButtonEventArgs& e)
		{
			_isHover = true;
			Style.ApplyStyleOn(StyleState::Hover);
			MouseEnter.TryRaise(*this, e);
		}

		virtual void OnMouseLeave(const MouseButtonEventArgs& e)
		{
			_isHover = false;
			Style.ApplyStyleOn(StyleState::Normal);
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

		friend class UIElementsCollection;
	};
}