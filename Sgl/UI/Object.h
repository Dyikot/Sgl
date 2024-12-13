#pragma once

#include "UIElement.h"
#include "../Appearance/UIAppearance.h"
#include "../Appearance/Cursor.h"

namespace Sgl
{
	class Object: public UIElement
	{
	public:
		using SizeChangedEventHandler = EventHandler<IVisual, SizeChangedEventArgs>;

		static inline const PropertyId WidthProperty = PropertyManager::Register<float>("Width");
		static inline const PropertyId HeightProperty = PropertyManager::Register<float>("Height");
		static inline const PropertyId MinWidthProperty = PropertyManager::Register<float>("MinWidth");
		static inline const PropertyId MinHeightProperty = PropertyManager::Register<float>("MinHeight");
		static inline const PropertyId MaxWidthProperty = PropertyManager::Register<float>("MaxWidth");
		static inline const PropertyId MaxHeightProperty = PropertyManager::Register<float>("MaxHeight");
		static inline const PropertyId MarginProperty = PropertyManager::Register<Thikness>("Margin");
		static inline const PropertyId CursorProperty = PropertyManager::Register<Cursor*>("Cursor");
		static inline const PropertyId ToolTipProperty = PropertyManager::Register<IVisual*>("ToolTip");
		static inline const PropertyId HorizontalAlignmentProperty = PropertyManager::Register<HorizontalAlignment>("HorizontalAlignment");
		static inline const PropertyId VerticalAligmentProperty = PropertyManager::Register<VerticalAligment>("VerticalAligment");
		static inline const PropertyId MouseEnterProperty = PropertyManager::Register<MouseEventHandler>("MouseEnter");
		static inline const PropertyId MouseLeaveProperty = PropertyManager::Register<MouseEventHandler>("MouseLeave");
		static inline const PropertyId SizeChangedProperty = PropertyManager::Register<SizeChangedEventHandler>("SizeChanged");

		Cursor* ObjectCursor = nullptr;
		IVisual* ToolTip = nullptr;
	protected:
		float _width = 0;
		float _height = 0;
		float _minWidth = 0;
		float _minHeight = 0;
		float _maxWidth = 0;
		float _maxHeight = 0;
		Point _position = { 0, 0 };
		Thikness _margin;
		HorizontalAlignment _horizontalAlignment = HorizontalAlignment::Stretch;
		VerticalAligment _verticalAlignment = VerticalAligment::Stretch;
		Visibility _visibility = Visibility::Visible;
	private:
		bool _isMouseOver = false;
	public:
		Object() = default;
		Object(SDL_FPoint position) noexcept;
		Object(SDL_FPoint position, const Style& style) noexcept;
		Object(const Style& style) noexcept;
		virtual ~Object() = default;

		void SetWidth(float value) { _width = value; }
		void SetHeight(float value) { _height = value; }
		void SetMinWidth(float value) { _minWidth = value; }
		void SetMinHeight(float value) { _minHeight = value; }
		void SetMaxWidth(float value) { _maxWidth = value; }
		void SetMaxHeight(float value) { _maxHeight = value; }
		void SetPosition(SDL_FPoint value) { _position = value; }
		void SetMargin(const Thikness& value) { _margin = value; }
		void SetHorizontalAlignment(HorizontalAlignment value) { _horizontalAlignment = value; }
		void SetVerticalAlignment(VerticalAligment value) { _verticalAlignment = value; }
		void SetVisibility(Visibility value) { _visibility = value; }

		float GetWidth() const { return _width; }
		float GetHeight() const { return _height; }
		float GetMinWidth() const { return _minWidth; }
		float GetMinHeight() const { return _minHeight; }
		float GetMaxWidth() const { return _maxWidth; }
		float GetMaxHeight() const { return _maxHeight; }
		SDL_FPoint GetPosition() const { return _position; }
		const Thikness& GetMargin() const { return _margin; }
		HorizontalAlignment GetHorizontalAlignment() const { return _horizontalAlignment; }
		VerticalAligment GetVerticalAlignment() const { return _verticalAlignment; }
		Visibility GetVisibility() const { return _visibility; }

		Event<MouseEventHandler> MouseEnter;
		Event<MouseEventHandler> MouseLeave;
		Event<SizeChangedEventHandler> SizeChanged;

		void OnRender(RenderContext& renderContext) const override;
		bool IsMouseOver() const noexcept { return _isMouseOver; }
	protected:
		virtual void OnMouseEnter(const MouseButtonEventArgs& e);
		virtual void OnMouseLeave(const MouseButtonEventArgs& e);
		virtual void OnSizeChanged(IVisual* sender, const SizeChangedEventArgs& e);
	};
}