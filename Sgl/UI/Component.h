#pragma once

#include "UIElement.h"
#include "ComponentsCollection.h"
#include "../Style/Properties.h"

namespace Sgl
{
	using ComponentEventHandler = EventHandler<Component, EventArgs>;

	class Component: public UIElement
	{
	public:
		static inline const PropertyId& WidthProperty = PropertyManager::Register<float>("Width");
		static inline const PropertyId& HeightProperty = PropertyManager::Register<float>("Height");
		static inline const PropertyId& MinWidthProperty = PropertyManager::Register<float>("MinWidth");
		static inline const PropertyId& MinHeightProperty = PropertyManager::Register<float>("MinHeight");
		static inline const PropertyId& MaxWidthProperty = PropertyManager::Register<float>("MaxWidth");
		static inline const PropertyId& MaxHeightProperty = PropertyManager::Register<float>("MaxHeight");
		static inline const PropertyId& ZIndexProperty = PropertyManager::Register<size_t>("ZIndex");
		static inline const PropertyId& MarginProperty = PropertyManager::Register<Thikness>("Margin");
		static inline const PropertyId& ToolTipProperty = PropertyManager::Register<const Component*>("ToolTip");
		static inline const PropertyId& HorizontalAlignmentProperty = PropertyManager::Register<HorizontalAlignment>("HorizontalAlignment");
		static inline const PropertyId& VerticalAligmentProperty = PropertyManager::Register<VerticalAligment>("VerticalAligment");
		static inline const PropertyId& VisibilityProperty = PropertyManager::Register<Visibility>("Visibility");
		static inline const PropertyId& BorderColorProperty = PropertyManager::Register<Color>("BorderColor");
		static inline const PropertyId& BorderThiknessProperty = PropertyManager::Register<Thikness>("BorderThikness");
		static inline const PropertyId& FontFamilyProperty = PropertyManager::Register<FontFamily>("FontFamily");
		static inline const PropertyId& FontWeightProperty = PropertyManager::Register<FontWeight>("FontWeight");
		static inline const PropertyId& FontColorProperty = PropertyManager::Register<Color>("FontColor");
		static inline const PropertyId& FontSizeProperty = PropertyManager::Register<uint16_t>("FontSize");

		static constexpr uint16_t DefaultFontSize = 14;

		SDL_FPoint position = { 0, 0 };
		ComponentsCollection children = ComponentsCollection(*this);
		Event<MouseEventHandler> mouseEnter;
		Event<MouseEventHandler> mouseLeave;
	private:
		bool _hover = false;
	public:
		Component();
		virtual ~Component() = default;

		void SetWidth(float value) { SetProperty(WidthProperty, value); }
		void SetHeight(float value) { SetProperty(HeightProperty, value); }
		void SetMinWidth(float value) { SetProperty(MinWidthProperty, value); }
		void SetMinHeight(float value) { SetProperty(MinHeightProperty, value); }
		void SetMaxWidth(float value) { SetProperty(MaxWidthProperty, value); }
		void SetMaxHeight(float value) { SetProperty(MaxHeightProperty, value); }
		void SetZIndex(size_t value) { SetProperty(ZIndexProperty, value); }
		void SetMargin(Thikness value) { SetProperty(MarginProperty, value); }
		void SetHorizontalAlignment(HorizontalAlignment value) { SetProperty(HorizontalAlignmentProperty, value); }
		void SetVerticalAlignment(VerticalAligment value) { SetProperty(VerticalAligmentProperty, value); }
		void SetVisibility(Visibility value) { SetProperty(VisibilityProperty, value); }
		void SetToolTip(const Component& value) { SetProperty<const Component*>(ToolTipProperty, &value); }
		void SetBorderColor(Color value) { SetProperty(BorderColorProperty, value); }
		void SetFontColor(Color value) { SetProperty(FontColorProperty, value); }
		void SetBorderThikness(Thikness value) { SetProperty(BorderThiknessProperty, value); }
		void SetFontFamily(const FontFamily& value) { SetProperty(FontFamilyProperty, value); }
		void SetFontSize(uint16_t value) { SetProperty(FontSizeProperty, value); }
		void SetFontWeight(FontWeight value) { SetProperty(FontWeightProperty, value); }

		auto GetWidth() const { return GetProperty<float>(WidthProperty); }
		auto GetHeight() const { return GetProperty<float>(HeightProperty); }
		auto GetMinWidth() const { return GetProperty<float>(MinWidthProperty); }
		auto GetMinHeight() const { return GetProperty<float>(MinHeightProperty); }
		auto GetMaxWidth() const { return GetProperty<float>(MaxWidthProperty); }
		auto GetMaxHeight() const { return GetProperty<float>(MaxHeightProperty); }
		auto GetZIndex() const { return GetProperty<size_t>(ZIndexProperty); }
		auto GetMargin() const { return GetProperty<Thikness>(MarginProperty); }
		auto GetHorizontalAlignment() const { return GetProperty<HorizontalAlignment>(HorizontalAlignmentProperty); }
		auto GetVerticalAlignment() const { return GetProperty<VerticalAligment>(VerticalAligmentProperty); }
		auto GetVisibility() const { return GetProperty<Visibility>(VisibilityProperty); }
		auto GetToolTip() const { return GetProperty<const Component*>(ToolTipProperty); }
		auto GetBorderColor() const { return GetProperty<Color>(BorderColorProperty); }
		auto GetFontColor() const { return GetProperty<Color>(FontColorProperty); }
		auto GetBorderThikness() const { return GetProperty<Thikness>(BorderThiknessProperty); }
		const auto& GetFontFamily() const { return GetProperty<FontFamily>(FontFamilyProperty); }
		auto GetFontSize() const { return GetProperty<uint16_t>(FontSizeProperty); }
		auto GetFontWeight() const { return GetProperty<FontWeight>(FontWeightProperty); }

		void OnRender(RenderContext renderContext) const override;
		void AddStyle(const Style& style) override;

		bool IsPointIn(SDL_FPoint point) const
		{
			return point.x >= position.x &&
				   point.x <= position.x + GetWidth() &&
				   point.y >= position.y &&
				   point.y <= position.y + GetHeight();
		}
	protected:
		virtual void OnMouseEnter(const MouseButtonEventArgs& e);
		virtual void OnMouseLeave(const MouseButtonEventArgs& e);

		friend class ComponentsCollection;
	};
}