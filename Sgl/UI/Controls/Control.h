#pragma once

#include "SDL/SDL_ttf.h"
#include "../Panels/Panel.h"
#include "../../Appearance/Color.h"

namespace Sgl
{
	class Control: public Object
	{
	public:
		static inline const PropertyId BackgroundProperty = PropertyManager::Register<Paint>("Background");
		static inline const PropertyId BorderProperty = PropertyManager::Register<Color>("Border");
		static inline const PropertyId BorderThiknessProperty = PropertyManager::Register<Thikness>("BorderThikness");
		static inline const PropertyId FontFamilyProperty = PropertyManager::Register<FontFamily>("FontFamily");
		static inline const PropertyId FontWeightProperty = PropertyManager::Register<FontWeight>("FontWeight");
		static inline const PropertyId FontColorProperty = PropertyManager::Register<Color>("FontColor");
		static inline const PropertyId FontSizeProperty = PropertyManager::Register<uint16_t>("FontSize");
		static inline const PropertyId MouseDoubleClickProperty = PropertyManager::Register<MouseEventHandler>("MouseDoubleClick");

		Panel* ControlPanel = nullptr;
	private:
		Paint _backgound = &Colors::Transparent;
		Color _borderColor = Colors::Transparent;
		Thikness _borderThickness;
		FontFamily _fontFamily;
		FontWeight _fontWeight;
		Color _fontColor = Colors::Black;
		uint16_t _fontSize;
	public:
		Control() = default;
		Control(const Style& style) noexcept;

		virtual void SetBackgound(const Paint& value) { _backgound = value; }
		virtual void SetBorderColor(Color value) { _borderColor = value; }
		virtual void SetFontColor(Color value) { _fontColor = value; }
		virtual void SetBorderThikness(const Thikness& value) { _borderThickness = value; }
		virtual void SetFontFamily(const FontFamily& value) { _fontFamily = value; }
		virtual void SetFontSize(uint16_t value) { _fontSize = value; }
		virtual void SetFontWeight(FontWeight value) { _fontWeight = value; }
		
		virtual const Paint& GetBackgound() const { return _backgound; }
		virtual Color GetBorderColor() const { return _borderColor; }
		virtual Color GetFontColor() const { return _fontColor; }
		virtual const Thikness& GetBorderThikness() const { return _borderThickness; }
		virtual const FontFamily& GetFontFamily() const { return _fontFamily; }
		virtual uint16_t GetFontSize() const { return _fontSize; }
		virtual FontWeight GetFontWeight() const { return _fontWeight; }

		Event<MouseEventHandler> MouseDoubleClick;
	protected:
		virtual void OnMouseDoubleClick(const MouseButtonEventArgs& e);
	private:
		friend class Panel;
	};
}
