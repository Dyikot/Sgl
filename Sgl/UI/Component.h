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
		static constexpr PropertyId WidthProperty = PropertyId::New<float>("Width");
		static constexpr PropertyId HeightProperty = PropertyId::New<float>("Height");
		static constexpr PropertyId MinWidthProperty = PropertyId::New<float>("MinWidth");
		static constexpr PropertyId MinHeightProperty = PropertyId::New<float>("MinHeight");
		static constexpr PropertyId MaxWidthProperty = PropertyId::New<float>("MaxWidth");
		static constexpr PropertyId MaxHeightProperty = PropertyId::New<float>("MaxHeight");
		static constexpr PropertyId ZIndexProperty = PropertyId::New<size_t>("ZIndex");
		static constexpr PropertyId MarginProperty = PropertyId::New<Thikness>("Margin");
		static constexpr PropertyId ToolTipProperty = PropertyId::New<const Component*>("ToolTip");
		static constexpr PropertyId HorizontalAlignmentProperty = PropertyId::New<HorizontalAlignment>("HorizontalAlignment");
		static constexpr PropertyId VerticalAligmentProperty = PropertyId::New<VerticalAligment>("VerticalAligment");
		static constexpr PropertyId VisibilityProperty = PropertyId::New<Visibility>("Visibility");
		static constexpr PropertyId BorderColorProperty = PropertyId::New<Color>("BorderColor");
		static constexpr PropertyId BorderThiknessProperty = PropertyId::New<Thikness>("BorderThikness");
		static constexpr PropertyId FontFamilyProperty = PropertyId::New<FontFamily>("FontFamily");
		static constexpr PropertyId FontWeightProperty = PropertyId::New<FontWeight>("FontWeight");
		static constexpr PropertyId FontColorProperty = PropertyId::New<Color>("FontColor");
		static constexpr PropertyId FontSizeProperty = PropertyId::New<uint16_t>("FontSize");

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

		auto GetWidth() const { return GetPropertyValue<float>(WidthProperty); }
		auto GetHeight() const { return GetPropertyValue<float>(HeightProperty); }
		auto GetMinWidth() const { return GetPropertyValue<float>(MinWidthProperty); }
		auto GetMinHeight() const { return GetPropertyValue<float>(MinHeightProperty); }
		auto GetMaxWidth() const { return GetPropertyValue<float>(MaxWidthProperty); }
		auto GetMaxHeight() const { return GetPropertyValue<float>(MaxHeightProperty); }
		auto GetZIndex() const { return GetPropertyValue<size_t>(ZIndexProperty); }
		auto GetMargin() const { return GetPropertyValue<Thikness>(MarginProperty); }
		auto GetHorizontalAlignment() const { return GetPropertyValue<HorizontalAlignment>(HorizontalAlignmentProperty); }
		auto GetVerticalAlignment() const { return GetPropertyValue<VerticalAligment>(VerticalAligmentProperty); }
		auto GetVisibility() const { return GetPropertyValue<Visibility>(VisibilityProperty); }
		auto GetToolTip() const { return GetPropertyValue<const Component*>(ToolTipProperty); }
		auto GetBorderColor() const { return GetPropertyValue<Color>(BorderColorProperty); }
		auto GetFontColor() const { return GetPropertyValue<Color>(FontColorProperty); }
		auto GetBorderThikness() const { return GetPropertyValue<Thikness>(BorderThiknessProperty); }
		const auto& GetFontFamily() const { return GetPropertyValue<FontFamily>(FontFamilyProperty); }
		auto GetFontSize() const { return GetPropertyValue<uint16_t>(FontSizeProperty); }
		auto GetFontWeight() const { return GetPropertyValue<FontWeight>(FontWeightProperty); }

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