#pragma once

#include "../../Style/Properties.h"
#include "../../Graphic/IVisual.h"
#include "../../Events/MouseAndKeyEvents.h"

namespace Sgl
{
	class UIElementsCollection;

	class IUIElement: 
		public IVisual,
		public IMouseEventsListener,
		public IKeyEventsListener
	{
	public:
		~IUIElement() = default;

		virtual void OnMouseEnter(const MouseButtonEventArgs& e) = 0;
		virtual void OnMouseLeave(const MouseButtonEventArgs& e) = 0;

		virtual UIElementsCollection& Children() = 0;

		virtual void SetPosition(FPoint value) = 0;
		virtual FPoint GetPosition() const = 0;

		virtual void SetWidth(float value) = 0;
		virtual float GetWidth() const = 0;

		virtual void SetHeight(float value) = 0;
		virtual float GetHeight() const = 0;

		virtual void SetMinWidth(float value) = 0;
		virtual float GetMinWidth() const = 0;

		virtual void SetMinHeight(float value) = 0;
		virtual float GetMinHeight() const = 0;

		virtual void SetMaxWidth(float value) = 0;
		virtual float GetMaxWidth() const = 0;

		virtual void SetMaxHeight(float value) = 0;
		virtual float GetMaxHeight() const = 0;

		virtual void SetZIndex(size_t value) = 0;
		virtual size_t GetZIndex() const = 0;

		virtual void SetMargin(Thickness value) = 0;
		virtual Thickness GetMargin() const = 0;

		virtual void SetPadding(Thickness value) = 0;
		virtual Thickness GetPadding() const = 0;

		virtual void SetBorder(Border value) = 0;
		virtual Border GetBorder() const = 0;

		virtual void SetFont(const Font& value) = 0;
		virtual const Font& GetFont() const = 0;

		virtual void SetTooltip(IUIElement* value) = 0;
		virtual IUIElement* GetTooltip() const = 0;

		virtual void SetVisibility(Visibility value) = 0;
		virtual Visibility GetVisibility() const = 0;

		virtual void SetVerticalAlignment(VerticalAlignment value) = 0;
		virtual VerticalAlignment GetVerticalAlignment() const = 0;

		virtual void SetHorizontalAlignment(HorizontalAlignment value) = 0;
		virtual HorizontalAlignment GetHorizontalAlignment() const = 0;

		friend class UIElementsCollection;
	};

	struct ZIndexComparer
	{
		bool operator()(const IUIElement& left, const IUIElement& right) const
		{
			return left.GetZIndex() < right.GetZIndex();
		}
	};

	template<>
	struct StyleProperties<IUIElement>: public StyleProperties<IVisual>
	{
		float Width = 0;
		float Height = 0;
		float MinWidth = 0;
		float MinHeight = 0;
		float MaxWidth = std::numeric_limits<float>::max();
		float MaxHeight = std::numeric_limits<float>::max();
		size_t ZIndex = 1;
		Thickness Margin;
		Thickness Padding;
		Border Border =
		{
			.Thickness = Thickness(1),
			.Color = Colors::Black
		};
		Font Font =
		{
			.Size = 14,
			.Family = {},
			.Style = FontStyle::Normal,
			.Weight = FontWeight::Normal,
			.Color = Colors::Black
		};
		IUIElement* Tooltip = nullptr;
		Visibility Visibility = Visibility::Visible;
		VerticalAlignment VerticalAlignment = VerticalAlignment::Top;
		HorizontalAlignment HorizontalAlignment = HorizontalAlignment::Left;
	};
}