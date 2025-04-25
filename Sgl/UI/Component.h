#pragma once

#include <unordered_map>
#include <set>
#include "UIElement.h"
#include "../Style/Properties.h"

namespace Sgl
{
	class Component;
	class Scene;

	using ComponentEventHandler = EventHandler<Component, EventArgs>;

	struct ZIndexComparer
	{
		bool operator()(const Component& left, const Component& right) const;
	};

	class ComponentsCollection: public std::multiset<std::reference_wrapper<Component>, ZIndexComparer>, 
								public IVisual
	{
	public:
		Nullable<Component> hoverComponent;
	public:
		void OnMouseMove(const MouseButtonEventArgs& e);
		void OnMouseDown(const MouseButtonEventArgs& e);
		void OnMouseUp(const MouseButtonEventArgs& e);
		void OnRender(RenderContext renderContext) const override;
	};

	class Component: public UIElement
	{
	public:
		static constexpr PropertyId widthProperty = PropertyId::New<float>("Width");
		static constexpr PropertyId heightProperty = PropertyId::New<float>("Height");
		static constexpr PropertyId minWidthProperty = PropertyId::New<float>("MinWidth");
		static constexpr PropertyId minHeightProperty = PropertyId::New<float>("MinHeight");
		static constexpr PropertyId maxWidthProperty = PropertyId::New<float>("MaxWidth");
		static constexpr PropertyId maxHeightProperty = PropertyId::New<float>("MaxHeight");
		static constexpr PropertyId zIndexProperty = PropertyId::New<size_t>("ZIndex");
		static constexpr PropertyId marginProperty = PropertyId::New<Thikness>("Margin");
		static constexpr PropertyId toolTipProperty = PropertyId::New<const Component*>("ToolTip");
		static constexpr PropertyId horizontalAlignmentProperty = PropertyId::New<HorizontalAlignment>("HorizontalAlignment");
		static constexpr PropertyId verticalAligmentProperty = PropertyId::New<VerticalAligment>("VerticalAligment");
		static constexpr PropertyId visibilityProperty = PropertyId::New<Visibility>("Visibility");
		static constexpr PropertyId borderColorProperty = PropertyId::New<Color>("BorderColor");
		static constexpr PropertyId borderThiknessProperty = PropertyId::New<Thikness>("BorderThikness");
		static constexpr PropertyId fontFamilyProperty = PropertyId::New<FontFamily>("FontFamily");
		static constexpr PropertyId fontWeightProperty = PropertyId::New<FontWeight>("FontWeight");
		static constexpr PropertyId fontColorProperty = PropertyId::New<Color>("FontColor");
		static constexpr PropertyId fontSizeProperty = PropertyId::New<uint16_t>("FontSize");

		static constexpr uint16_t defaultFontSize = 14;

		SDL_FPoint position = { 0, 0 };
		ComponentsCollection children;
		UIElement& parent;
		Scene& scene;
	private:
		bool _hover = false;
	public:
		explicit Component(UIElement& parent);
		virtual ~Component() = default;		

		Event<MouseEventHandler> MouseEnter;
		Event<MouseEventHandler> MouseLeave;

		void SetWidth(float value) { SetProperty(widthProperty, value); }
		void SetHeight(float value) { SetProperty(heightProperty, value); }
		void SetMinWidth(float value) { SetProperty(minWidthProperty, value); }
		void SetMinHeight(float value) { SetProperty(minHeightProperty, value); }
		void SetMaxWidth(float value) { SetProperty(maxWidthProperty, value); }
		void SetMaxHeight(float value) { SetProperty(maxHeightProperty, value); }
		void SetZIndex(size_t value) { SetProperty(zIndexProperty, value); }
		void SetMargin(Thikness value) { SetProperty(marginProperty, value); }
		void SetHorizontalAlignment(HorizontalAlignment value) { SetProperty(horizontalAlignmentProperty, value); }
		void SetVerticalAlignment(VerticalAligment value) { SetProperty(verticalAligmentProperty, value); }
		void SetVisibility(Visibility value) { SetProperty(visibilityProperty, value); }
		void SetToolTip(const Component& value) { SetProperty<const Component*>(toolTipProperty, &value); }
		void SetBorderColor(Color value) { SetProperty(borderColorProperty, value); }
		void SetFontColor(Color value) { SetProperty(fontColorProperty, value); }
		void SetBorderThikness(Thikness value) { SetProperty(borderThiknessProperty, value); }
		void SetFontFamily(const FontFamily& value) { SetProperty(fontFamilyProperty, value); }
		void SetFontSize(uint16_t value) { SetProperty(fontSizeProperty, value); }
		void SetFontWeight(FontWeight value) { SetProperty(fontWeightProperty, value); }

		auto GetWidth() const { return GetPropertyValue<float>(widthProperty); }
		auto GetHeight() const { return GetPropertyValue<float>(heightProperty); }
		auto GetMinWidth() const { return GetPropertyValue<float>(minWidthProperty); }
		auto GetMinHeight() const { return GetPropertyValue<float>(minHeightProperty); }
		auto GetMaxWidth() const { return GetPropertyValue<float>(maxWidthProperty); }
		auto GetMaxHeight() const { return GetPropertyValue<float>(maxHeightProperty); }
		auto GetZIndex() const { return GetPropertyValue<size_t>(zIndexProperty); }
		auto GetMargin() const { return GetPropertyValue<Thikness>(marginProperty); }
		auto GetHorizontalAlignment() const { return GetPropertyValue<HorizontalAlignment>(horizontalAlignmentProperty); }
		auto GetVerticalAlignment() const { return GetPropertyValue<VerticalAligment>(verticalAligmentProperty); }
		auto GetVisibility() const { return GetPropertyValue<Visibility>(visibilityProperty); }
		auto GetToolTip() const { return GetPropertyValue<const Component*>(toolTipProperty); }
		auto GetBorderColor() const { return GetPropertyValue<Color>(borderColorProperty); }
		auto GetFontColor() const { return GetPropertyValue<Color>(fontColorProperty); }
		auto GetBorderThikness() const { return GetPropertyValue<Thikness>(borderThiknessProperty); }
		const auto& GetFontFamily() const { return GetPropertyValue<FontFamily>(fontFamilyProperty); }
		auto GetFontSize() const { return GetPropertyValue<uint16_t>(fontSizeProperty); }
		auto GetFontWeight() const { return GetPropertyValue<FontWeight>(fontWeightProperty); }

		void OnRender(RenderContext renderContext) const override;
		void AddStyle(const Style& style) override;
		void SwitchCursorOn(const Cursor& cursor) override;
		void SwitchCursorOnDefault() override;
		UIElement& GetRootElement() override;

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
	private:
		friend class ComponentsCollection;
	};	
}