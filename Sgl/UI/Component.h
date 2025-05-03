#pragma once

#include "UIElement.h"
#include "ComponentsCollection.h"

namespace Sgl
{
	using ComponentEventHandler = EventHandler<Component, EventArgs>;

	class Component: public UIElement
	{
	public:
		SDL_FPoint position = { 0, 0 };
		ComponentsCollection children = ComponentsCollection(*this);
		Event<MouseEventHandler> mouseEnter;
		Event<MouseEventHandler> mouseLeave;
	private:
		bool _hover = false;
	public:
		Component() = default;
		explicit Component(std::unique_ptr<Style> style):
			UIElement(std::move(style))
		{ }
		virtual ~Component() = default;

		void SetWidth(float value) { style->width = value; }
		void SetHeight(float value) { style->height = value; }
		void SetMinWidth(float value) { style->minWidth = value; }
		void SetMinHeight(float value) { style->minHeight = value; }
		void SetMaxWidth(float value) { style->maxWidth = value; }
		void SetMaxHeight(float value) { style->maxHeight = value; }
		void SetZIndex(size_t value) { style->zIndex = value; }
		void SetMargin(Thickness value) { style->margin = value; }
		void SetHorizontalAlignment(HorizontalAlignment value) { style->horizontalAlignment = value; }
		void SetVerticalAlignment(VerticalAlignment value) { style->verticalAlignment = value; }
		void SetVisibility(Visibility value) { style->visibility = value; }
		void SetToolTip(const Component& value) { style->tooltip = Nullable<const Component>(&value); }
		void SetBorderColor(Color value) { style->borderColor = value; }
		void SetFontColor(Color value) { style->fontColor = value; }
		void SetBorderThickness(Thickness value) { style->borderThickness = value; }
		void SetFontFamily(const FontFamily& value) { style->fontFamily = value; }
		void SetFontSize(uint16_t value) { style->fontSize = value; }
		void SetFontWeight(FontWeight value) { style->fontWeight = value; }

		auto GetWidth() const { return style->width; }
		auto GetHeight() const { return style->height; }
		auto GetMinWidth() const { return style->minWidth; }
		auto GetMinHeight() const { return style->minHeight; }
		auto GetMaxWidth() const { return style->maxWidth; }
		auto GetMaxHeight() const { return style->maxHeight; }
		auto GetZIndex() const { return style->zIndex; }
		auto GetMargin() const { return style->margin; }
		auto GetHorizontalAlignment() const { return style->horizontalAlignment; }
		auto GetVerticalAlignment() const { return style->verticalAlignment; }
		auto GetVisibility() const { return style->visibility; }
		auto GetToolTip() const { return style->tooltip; }
		auto GetBorderColor() const { return style->borderColor; }
		auto GetFontColor() const { return style->fontColor; }
		auto GetBorderThickness() const { return style->borderThickness; }
		const auto& GetFontFamily() const { return style->fontFamily; }
		auto GetFontSize() const { return style->fontSize; }
		auto GetFontWeight() const { return style->fontWeight; }

		void OnRender(RenderContext renderContext) const override;

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