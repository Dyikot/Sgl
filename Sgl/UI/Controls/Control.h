#pragma once

#include "SDL/SDL_ttf.h"
#include "../Object.h"
#include "../../Appearance/Brushes.h"
#include "../../Appearance/ControlAppearance.h"

namespace Sgl
{
	class Control: public Object
	{
	public:
		static inline const PropertyId BackgroundProperty = PropertyManager::Register<Brush>("Background");
		static inline const PropertyId BorderProperty = PropertyManager::Register<ColorBrush>("Border");
		static inline const PropertyId BorderThiknessProperty = PropertyManager::Register<Thikness>("BorderThikness");
		static inline const PropertyId FontFamilyProperty = PropertyManager::Register<FontFamily>("FontFamily");
		static inline const PropertyId FontWeightProperty = PropertyManager::Register<FontWeight>("FontWeight");
		static inline const PropertyId FontColorProperty = PropertyManager::Register<ColorBrush>("FontColor");
		static inline const PropertyId FontSizeProperty = PropertyManager::Register<uint16_t>("FontSize");
		static inline const PropertyId MouseDoubleClickProperty = PropertyManager::Register<MouseEventHandler>("MouseDoubleClick");
	private:
		Brush _backgound = Colors::Transparent;
		ColorBrush _borderColor = Colors::Transparent;
		Thikness _borderThickness;
		FontFamily _fontFamily;
		FontWeight _fontWeight;
		ColorBrush _fontColor = Colors::Black;
		uint16_t _fontSize;
	public:
		Control() = default;
		Control(const Style& style) noexcept;

		virtual void SetBackgound(const Brush& value) { _backgound = value; }
		virtual void SetBorderColor(const ColorBrush& value) { _borderColor = value; }
		virtual void SetFontColor(const ColorBrush& value) { _fontColor = value; }
		virtual void SetBorderThikness(const Thikness& value) { _borderThickness = value; }
		virtual void SetFontFamily(const FontFamily& value) { _fontFamily = value; }
		virtual void SetFontSize(uint16_t value) { _fontSize = value; }
		virtual void SetFontWeight(FontWeight value) { _fontWeight = value; }
		
		virtual const Brush& GetBackgound() const { return _backgound; }
		virtual const ColorBrush& GetBorderColor() const { return _borderColor; }
		virtual const ColorBrush& GetFontColor() const { return _fontColor; }
		virtual const Thikness& GetBorderThikness() const { return _borderThickness; }
		virtual const FontFamily& GetFontFamily() const { return _fontFamily; }
		virtual const uint16_t GetFontSize() const { return _fontSize; }
		virtual const FontWeight GetFontWeight() const { return _fontWeight; }

		Event<MouseEventHandler> MouseDoubleClick;
	protected:
		virtual void OnMouseDoubleClick(const MouseButtonEventArgs& e);
	};
}
