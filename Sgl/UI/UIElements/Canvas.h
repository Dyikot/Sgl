#pragma once

#include "../Panel.h"

namespace Sgl::UIElements
{
	class Canvas: public Panel
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
		static int GetLeft(UIElement& element);

		static void SetTop(UIElement& element, int value);
		static int GetTop(UIElement& element);

		static void SetRight(UIElement& element, int value);
		static int GetRight(UIElement& element);

		static void SetBottom(UIElement& element, int value);
		static int GetBottom(UIElement& element);

		void Render(RenderContext context) override;
	protected:
		void ArrangeContent(FRect rect) override;
	};
}