#pragma once

#include "SDL/SDL_ttf.h"
#include "../Object.h"
#include "../../Appearance/Color.h"

namespace Sgl
{
	class Panel;

	class Control: public Object
	{
	public:
		static constexpr size_t DefaultFontSize = 14;
		
		static inline const PropertyId BackgroundProperty = PropertyManager::Register<Fill>("Background");
		static inline const PropertyId BorderColorProperty = PropertyManager::Register<Color>("BorderColor");
		static inline const PropertyId BorderThiknessProperty = PropertyManager::Register<Thikness>("BorderThikness");
		static inline const PropertyId FontFamilyProperty = PropertyManager::Register<FontFamily>("FontFamily");
		static inline const PropertyId FontWeightProperty = PropertyManager::Register<FontWeight>("FontWeight");
		static inline const PropertyId FontColorProperty = PropertyManager::Register<Color>("FontColor");
		static inline const PropertyId FontSizeProperty = PropertyManager::Register<uint16_t>("FontSize");
		static inline const EventId MouseDoubleClickEvent = EventManager::Register<MouseEventHandler>("MouseDoubleClick");

		Panel* Panel = nullptr;
	protected:
		Fill _backgound = Colors::Transparent;
		Color _borderColor = Colors::Transparent;
		Thikness _borderThickness;
		FontFamily _fontFamily;
		FontWeight _fontWeight = FontWeight::Normal;
		Color _fontColor = Colors::Black;
		size_t _fontSize = DefaultFontSize;
	public:
		Control();
		explicit Control(SDL_FPoint position) noexcept;

		void SetBackgound(const Fill& value) { _backgound = value; }
		void SetBorderColor(Color value) { _borderColor = value; }
		void SetFontColor(Color value) { _fontColor = value; }
		void SetBorderThikness(const Thikness& value) { _borderThickness = value; }
		void SetFontFamily(const FontFamily& value) { _fontFamily = value; }
		void SetFontSize(uint16_t value) { _fontSize = value; }
		void SetFontWeight(FontWeight value) { _fontWeight = value; }
		void SetStyle(const Style& style) override;
		
		const Fill& GetBackgound() const { return _backgound; }
		Color GetBorderColor() const { return _borderColor; }
		Color GetFontColor() const { return _fontColor; }
		const Thikness& GetBorderThikness() const { return _borderThickness; }
		const FontFamily& GetFontFamily() const { return _fontFamily; }
		uint16_t GetFontSize() const { return _fontSize; }
		FontWeight GetFontWeight() const { return _fontWeight; }

		Event<MouseEventHandler> MouseDoubleClick;
	protected:
		virtual void OnMouseDoubleClick(const MouseButtonEventArgs& e);
	private:
		friend class Panel;
	};
}
