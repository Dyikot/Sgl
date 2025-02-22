#pragma once

#include "SDL/SDL_ttf.h"
#include "../Component.h"
#include "../../Appearance/Color.h"

namespace Sgl
{
	class Panel;

	class Control: public Component
	{
	public:
		static constexpr uint16_t DefaultFontSize = 14;
		
		static inline const PropertyId BackgroundProperty = PropertyManager::Register<Fill>("Background");
		static inline const PropertyId BorderColorProperty = PropertyManager::Register<Color>("BorderColor");
		static inline const PropertyId BorderThiknessProperty = PropertyManager::Register<Thikness>("BorderThikness");
		static inline const PropertyId FontFamilyProperty = PropertyManager::Register<FontFamily>("FontFamily");
		static inline const PropertyId FontWeightProperty = PropertyManager::Register<FontWeight>("FontWeight");
		static inline const PropertyId FontColorProperty = PropertyManager::Register<Color>("FontColor");
		static inline const PropertyId FontSizeProperty = PropertyManager::Register<uint16_t>("FontSize");

		Panel* Panel;
	private:
		bool _isEventsInitialized = false;
	public:
		explicit Control(SDL_FPoint position = {}) noexcept;

		void SetBackgound(const Fill& value) { SetProperty(BackgroundProperty, value); }
		void SetBorderColor(Color value) { SetProperty(BorderColorProperty, value); }
		void SetFontColor(Color value) { SetProperty(FontColorProperty, value); }
		void SetBorderThikness(const Thikness& value) { SetProperty(BorderThiknessProperty, value); }
		void SetFontFamily(const FontFamily& value) { SetProperty(FontFamilyProperty, value); }
		void SetFontSize(uint16_t value) { SetProperty(FontSizeProperty, value); }
		void SetFontWeight(FontWeight value) { SetProperty(FontWeightProperty, value); }
		
		const Fill& GetBackgound() const { return GetPropertyValue<Fill>(BackgroundProperty); }
		Color GetBorderColor() const { return GetPropertyValue<Color>(BorderColorProperty); }
		Color GetFontColor() const { return GetPropertyValue<Color>(FontColorProperty); }
		const Thikness& GetBorderThikness() const { return GetPropertyValue<Thikness>(BorderThiknessProperty); }
		const FontFamily& GetFontFamily() const { return GetPropertyValue<FontFamily>(FontFamilyProperty); }
		uint16_t GetFontSize() const { return GetPropertyValue<uint16_t>(FontSizeProperty); }
		FontWeight GetFontWeight() const { return GetPropertyValue<FontWeight>(FontWeightProperty); }
	private:
		friend class Panel;
	};
}
