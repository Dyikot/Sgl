#pragma once

#include <set>
#include "../Graphic/IVisual.h"
#include "../Graphic/Color.h"
#include "../Graphic/Cursor.h"
#include "../Graphic/Texture.h"
#include "../Events/Event.h"
#include "../Events/MouseAndKeyEvents.h"
#include "../Appearance/Properties.h"
#include "../Appearance/Style.h"

namespace Sgl
{
	class UIElement;
	
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
		UIElement* Parent = nullptr;
		Func<Cursor::Getter> DefaultCursorGetter;
	public:
		UIElementsCollection(UIElement* parent);
		UIElementsCollection(Func<Cursor::Getter> defaultCursorGetter);
		void OnRender(RenderContext renderContext) const override;
	protected:
		void OnMouseMove(const MouseButtonEventArgs& e) override; 
		void OnMouseDown(const MouseButtonEventArgs& e) override;
		void OnMouseUp(const MouseButtonEventArgs& e) override;
		void OnMouseDoubleClick(const MouseButtonEventArgs& e) override;
		void OnMouseWheel(const MouseWheelEventArgs& e) override;
		void OnKeyDown(const KeyEventArgs& e) override;
		void OnKeyUp(const KeyEventArgs& e) override;

		friend class Application;
	};

	class UIElement: 
		public IVisual,
		public IMouseEventsListener, 
		public IKeyEventsListener
	{
	public:
		struct Properties
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
			UIElement* Tooltip = nullptr;
			Texture* BackgroundTexture = nullptr;
			Visibility Visibility = Visibility::Visible;
			VerticalAlignment VerticalAlignment = VerticalAlignment::Top;
			HorizontalAlignment HorizontalAlignment = HorizontalAlignment::Left;
		};

		using PropertiesFactory = std::unique_ptr<Properties>(*)();

		FPoint Position;
		Style<Properties> BaseStyle;
		Style<Properties> HoverStyle;
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
	protected:
		std::unique_ptr<Properties> _properties;
	private:
		bool _isHover = false;
	public:
		UIElement();
		UIElement(PropertiesFactory propertiesFactory);
		virtual ~UIElement() = default;

		void OnRender(RenderContext renderContext) const override;

		void SetWidth(float value) { _properties->Width = value; }
		float GetWidth() const { return _properties->Width; }

		void SetHeight(float value) { _properties->Height = value; }
		float GetHeight() const { return _properties->Height; }

		void SetMinWidth(float value) { _properties->MinWidth = value; }
		float GetMinWidth() const { return _properties->MinWidth; }

		void SetMinHeight(float value) { _properties->MinHeight = value; }
		float GetMinHeight() const { return _properties->MinHeight; }

		void SetMaxWidth(float value) { _properties->MaxWidth = value; }
		float GetMaxWidth() const { return _properties->MaxWidth; }

		void SetMaxHeight(float value) { _properties->MaxHeight = value; }
		float GetMaxHeight() const { return _properties->MaxHeight; }

		void SetZIndex(size_t value) { _properties->ZIndex = value; }
		size_t GetZIndex() const { return _properties->ZIndex; }

		void SetMargin(Thickness value) { _properties->Margin = value; }
		Thickness GetMargin() const { return _properties->Margin; }

		void SetPadding(Thickness value) { _properties->Padding = value; }
		Thickness GetPadding() const { return _properties->Padding; }

		void SetBorder(Border value) { _properties->Border = value; }
		Border GetBorder() const { return _properties->Border; }

		void SetFont(const Font& value) { _properties->Font = value; }
		const Font& GetFont() const { return _properties->Font; }

		void SetCursor(Cursor::Getter value) { _properties->Cursor = value; }
		Cursor::Getter GetCursor() const { return _properties->Cursor; }

		void SetBackgroundColor(Color value) { _properties->BackgroundColor = value; }
		Color GetBackgroundColor() const { return _properties->BackgroundColor; }

		void SetTooltip(UIElement* value) { _properties->Tooltip = value; }
		UIElement* GetTooltip() const { return _properties->Tooltip; }

		void SetBackgroundTexture(Texture* value) { _properties->BackgroundTexture = value; }
		Texture* GetBackgroundTexture() const { return _properties->BackgroundTexture; }

		void SetVisibility(Visibility value) { _properties->Visibility = value; }
		Visibility GetVisibility() const { return _properties->Visibility; }

		void SetVerticalAlignment(VerticalAlignment value) { _properties->VerticalAlignment = value; }
		VerticalAlignment GetVerticalAlignment() const { return _properties->VerticalAlignment; }

		void SetHorizontalAlignment(HorizontalAlignment value) { _properties->HorizontalAlignment = value; }
		HorizontalAlignment GetHorizontalAlignment() const { return _properties->HorizontalAlignment; }
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
			HoverStyle.Apply();
			MouseEnter.TryRaise(*this, e);
		}

		virtual void OnMouseLeave(const MouseButtonEventArgs& e)
		{
			_isHover = false;
			BaseStyle.Apply();
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