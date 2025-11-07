#pragma once

#include "../Panel.h"

namespace Sgl::UIElements
{
	class Canvas : public Panel
	{
	public:
		static inline AttachedProperty<int> LeftProperty;
		static inline AttachedProperty<int> TopProperty;
		static inline AttachedProperty<int> RightProperty;
		static inline AttachedProperty<int> BottomProperty;
	public:
		Canvas() = default;
		Canvas(const Canvas&) = default;
		Canvas(Canvas&&) = default;
		~Canvas() = default;

		static void SetLeft(UIElement& element, int value);
		static int GetLeft(const UIElement& element);

		static void SetTop(UIElement& element, int value);
		static int GetTop(const UIElement& element);

		static void SetRight(UIElement& element, int value);
		static int GetRight(const UIElement& element);

		static void SetBottom(UIElement& element, int value);
		static int GetBottom(const UIElement& element);
	protected:
		FSize MeasureContent(FSize avaliableSize) override;
		void ArrangeContent(FRect rect) override;
	};
}