#pragma once

#include "UIElement.h"
#include "../Appearance/ControlAppearance.h"
#include "../Appearance/Cursor.h"


namespace Sgl
{
	class Object: public UIElement
	{
	public:
		using SizeChangedEventHandler = std::function<void(IVisual*, const SizeChangedEventArgs&)>;

		static inline const PropertyId WidthProperty = PropertyManager::Register<float>("Width");
		static inline const PropertyId HeightProperty = PropertyManager::Register<float>("Height");
		static inline const PropertyId MinWidthProperty = PropertyManager::Register<float>("MinWidth");
		static inline const PropertyId MinHeightProperty = PropertyManager::Register<float>("MinHeight");
		static inline const PropertyId MaxWidthProperty = PropertyManager::Register<float>("MaxWidth");
		static inline const PropertyId MaxHeightProperty = PropertyManager::Register<float>("MaxHeight");
		static inline const PropertyId MarginProperty = PropertyManager::Register<Thikness>("Margin");
		static inline const PropertyId CursorProperty = PropertyManager::Register<SDL_Cursor*>("Cursor");
		static inline const PropertyId ToolTipProperty = PropertyManager::Register<IVisual*>("ToolTip");
		static inline const PropertyId HorizontalAlignmentProperty = PropertyManager::Register<HorizontalAlignment>("HorizontalAlignment");
		static inline const PropertyId VerticalAligmentProperty = PropertyManager::Register<VerticalAligment>("VerticalAligment");
		static inline const PropertyId SizeChangedProperty = PropertyManager::Register<SizeChangedEventHandler>("SizeChanged");

		SDL_Cursor* SDLCursor = nullptr;
		IVisual* ToolTip = nullptr;
	protected:
		bool _isToolTipRenderRequired = false;
		float _width = 0;
		float _height = 0;
		float _minWidth = 0;
		float _minHeight = 0;
		float _maxWidth = 0;
		float _maxHeight = 0;
		SDL_FPoint _position = {};
		Thikness _margin = {};
		HorizontalAlignment _horizontalAlignment = HorizontalAlignment::Stretch;
		VerticalAligment _verticalAlignment = VerticalAligment::Stretch;
	public:
		Object() = default;
		Object(const SDL_FPoint& position) noexcept;
		Object(const SDL_FPoint& position, const Style& style) noexcept;
		Object(const Style& style) noexcept;
		virtual ~Object() = default;

		#pragma region Setters
		virtual void SetWidth(float value)
		{
			_width = value;
		}

		virtual void SetHeight(float value)
		{
			_height = value;
		}

		virtual void SetMinWidth(float value)
		{
			_minWidth = value;
		}

		virtual void SetMinHeight(float value)
		{
			_minHeight = value;
		}

		virtual void SetMaxWidth(float value)
		{
			_maxWidth = value;
		}

		virtual void SetMaxHeight(float value)
		{
			_maxHeight = value;
		}

		virtual void SetPosition(const SDL_FPoint& value)
		{
			_position = value;
		}

		virtual void SetMargin(const Thikness& value)
		{
			_margin = value;
		}

		virtual void SetHorizontalAlignment(HorizontalAlignment value)
		{
			_horizontalAlignment = value;
		}

		virtual void SetVerticalAlignment(VerticalAligment value)
		{
			_verticalAlignment = value;
		}
		#pragma endregion
		#pragma region Getters
		virtual float GetWidth() const
		{
			return _width;
		}

		virtual float GetHeight() const
		{
			return _height;
		}

		virtual float GetMinWidth() const
		{
			return _minWidth;
		}

		virtual float GetMinHeight() const
		{
			return _minHeight;
		}

		virtual float GetMaxWidth() const
		{
			return _maxWidth;
		}

		virtual float GetMaxHeight() const
		{
			return _maxHeight;
		}

		virtual const SDL_FPoint& GetPosition() const
		{
			return _position;
		}

		virtual const Thikness& GetMargin() const
		{
			return _margin;
		}

		virtual HorizontalAlignment GetHorizontalAlignment() const
		{
			return _horizontalAlignment;
		}

		virtual VerticalAligment GetVerticalAlignment() const
		{
			return _verticalAlignment;
		}
		#pragma endregion		

		Event<SizeChangedEventHandler> SizeChanged;

		void OnRender() const override;
		virtual bool IsMouseOver(const SDL_Point& mousePosition) const noexcept;
	protected:
		void OnMouseEnter(const MouseButtonEventArgs& e) override;
		void OnMouseLeave(const MouseButtonEventArgs& e) override;
		virtual void OnSizeChanged(IVisual* sender, const SizeChangedEventArgs& e);
	};
}