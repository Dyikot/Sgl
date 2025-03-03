#pragma once

#include <unordered_map>
#include <set>
#include "UIElement.h"
#include "../Style/Properties.h"

namespace Sgl
{
	class Component;

	struct ZIndexComparer
	{
		bool operator()(const Component& left, const Component& right) const;
	};

	class ComponentsCollection: public std::multiset<std::reference_wrapper<Component>, ZIndexComparer>, 
								public IVisual
	{
	public:
		Component* HoverComponent = nullptr;
	public:
		void OnMouseMove(const MouseButtonEventArgs& e);
		void OnMouseDown(const MouseButtonEventArgs& e);
		void OnMouseUp(const MouseButtonEventArgs& e);
		void OnRender(RenderContext& renderContext) const override;
	};

	class Component: public UIElement
	{
	public:
		static inline const PropertyId WidthProperty = PropertyManager::Register<float>("Width");
		static inline const PropertyId HeightProperty = PropertyManager::Register<float>("Height");
		static inline const PropertyId MinWidthProperty = PropertyManager::Register<float>("MinWidth");
		static inline const PropertyId MinHeightProperty = PropertyManager::Register<float>("MinHeight");
		static inline const PropertyId MaxWidthProperty = PropertyManager::Register<float>("MaxWidth");
		static inline const PropertyId MaxHeightProperty = PropertyManager::Register<float>("MaxHeight");
		static inline const PropertyId ZIndexProperty = PropertyManager::Register<size_t>("ZIndex");
		static inline const PropertyId MarginProperty = PropertyManager::Register<Thikness>("Margin");
		static inline const PropertyId ToolTipProperty = PropertyManager::Register<const Component*>("ToolTip");
		static inline const PropertyId HorizontalAlignmentProperty = PropertyManager::Register<HorizontalAlignment>("HorizontalAlignment");
		static inline const PropertyId VerticalAligmentProperty = PropertyManager::Register<VerticalAligment>("VerticalAligment");
		static inline const PropertyId VisibilityProperty = PropertyManager::Register<Visibility>("Visibility");
		static inline const PropertyId BorderColorProperty = PropertyManager::Register<Color>("BorderColor");
		static inline const PropertyId BorderThiknessProperty = PropertyManager::Register<Thikness>("BorderThikness");
		static inline const PropertyId FontFamilyProperty = PropertyManager::Register<FontFamily>("FontFamily");
		static inline const PropertyId FontWeightProperty = PropertyManager::Register<FontWeight>("FontWeight");
		static inline const PropertyId FontColorProperty = PropertyManager::Register<Color>("FontColor");
		static inline const PropertyId FontSizeProperty = PropertyManager::Register<uint16_t>("FontSize");

		static constexpr uint16_t DefaultFontSize = 14;

		UIElement& Parent;
		ComponentsCollection Children;
		SDL_FPoint Position = { 0, 0 };
	private:
		bool _hover = false;
	public:
		explicit Component(UIElement& parent);
		virtual ~Component() = default;		

		Event<MouseEventHandler> MouseEnter;
		Event<MouseEventHandler> MouseLeave;

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
		void SetBorderThikness(const Thikness& value) { SetProperty(BorderThiknessProperty, value); }
		void SetFontFamily(const FontFamily& value) { SetProperty(FontFamilyProperty, value); }
		void SetFontSize(uint16_t value) { SetProperty(FontSizeProperty, value); }
		void SetFontWeight(FontWeight value) { SetProperty(FontWeightProperty, value); }

		const float& GetWidth() const { return GetPropertyValue<float>(WidthProperty); }
		const float& GetHeight() const { return GetPropertyValue<float>(HeightProperty); }
		const float& GetMinWidth() const { return GetPropertyValue<float>(MinWidthProperty); }
		const float& GetMinHeight() const { return GetPropertyValue<float>(MinHeightProperty); }
		const float& GetMaxWidth() const { return GetPropertyValue<float>(MaxWidthProperty); }
		const float& GetMaxHeight() const { return GetPropertyValue<float>(MaxHeightProperty); }
		const size_t& GetZIndex() const { return GetPropertyValue<size_t>(ZIndexProperty); }
		const Thikness& GetMargin() const { return GetPropertyValue<Thikness>(MarginProperty); }
		const HorizontalAlignment& GetHorizontalAlignment() const { return GetPropertyValue<HorizontalAlignment>(HorizontalAlignmentProperty); }
		const VerticalAligment& GetVerticalAlignment() const { return GetPropertyValue<VerticalAligment>(VerticalAligmentProperty); }
		const Visibility& GetVisibility() const { return GetPropertyValue<Visibility>(VisibilityProperty); }
		const Component* const& GetToolTip() const { return GetPropertyValue<const Component*>(ToolTipProperty); }
		Color GetBorderColor() const { return GetPropertyValue<Color>(BorderColorProperty); }
		Color GetFontColor() const { return GetPropertyValue<Color>(FontColorProperty); }
		const Thikness& GetBorderThikness() const { return GetPropertyValue<Thikness>(BorderThiknessProperty); }
		const FontFamily& GetFontFamily() const { return GetPropertyValue<FontFamily>(FontFamilyProperty); }
		uint16_t GetFontSize() const { return GetPropertyValue<uint16_t>(FontSizeProperty); }
		FontWeight GetFontWeight() const { return GetPropertyValue<FontWeight>(FontWeightProperty); }

		void OnRender(RenderContext& renderContext) const override;
		void AddStyle(const Style& style) override;
		void SwitchCursorOn(const Cursor& cursor) override;
		void SwitchCursorOnDefault() override;
		bool IsPointIn(SDL_FPoint point) const;
	protected:
		virtual void OnMouseEnter(const MouseButtonEventArgs& e);
		virtual void OnMouseLeave(const MouseButtonEventArgs& e);
	private:
		friend class ComponentsCollection;
	};	
}