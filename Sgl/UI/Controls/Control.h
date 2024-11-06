#pragma once

#include "SDL/SDL_ttf.h"
#include "../Object.h"
#include "../../Appearance/Color.h"
#include "../../Appearance/ControlAppearance.h"

namespace Sgl
{
	class Control: public Object
	{
	public:
		static inline const PropertyId BackgroundProperty = PropertyManager::Register<ColorBrush>("Background");
	private:
		ColorBrush _backgound = Colors::Transparent;
		ColorBrush _borderColor = Colors::Transparent;
		Thikness _borderThickness;
		FontFamily _fontFamily;
		FontWeight _fontWeight;
		ColorBrush _fontColor = Colors::Black;
		uint16_t _fontSize;
	public:
		virtual void SetStyle(const Style& style) override;
		virtual void SetBackgound(const ColorBrush& value) 
		{
			_backgound = value;
		}
		virtual void SetBorderColor(const ColorBrush& value)
		{
			_borderColor = value;
		}
		virtual void SetFontColor(const ColorBrush& value) 
		{
			_fontColor = value; 
		}
		virtual void SetBorderThikness(const Thikness& value) 
		{
			_borderThickness = value;
		}
		virtual void SetFontFamily(const FontFamily& value) 
		{
			_fontFamily = value;
		}
		virtual void SetFontSize(uint16_t value) 
		{ 
			_fontSize = value;
		}
		virtual void SetFontWeight(FontWeight value) 
		{
			_fontWeight = value; 
		}
		
		virtual const ColorBrush& GetBackgound() const 
		{
			return _backgound; 
		}
		virtual const ColorBrush& GetBorderColor() const 
		{
			return _borderColor; 
		}
		virtual const ColorBrush& GetFontColor() const 
		{
			return _fontColor; 
		}
		virtual const Thikness& GetBorderThikness() const 
		{
			return _borderThickness;
		}
		virtual const FontFamily& GetFontFamily() const
		{
			return _fontFamily;
		}
		virtual const uint16_t GetFontSize() const 
		{ 
			return _fontSize; 
		}
		virtual const FontWeight GetFontWeight() const 
		{
			return _fontWeight; 
		}


		Event<MouseEventHandler> MouseDoubleClick;
	protected:
		virtual void OnMouseDoubleClick(const MouseButtonEventArgs& e);
	};
}
