#pragma once

#include "../UIElement/Panel.h"

namespace Sgl::UIElements
{
	class Canvas : public Panel
	{
	public:
		struct Context
		{
			int Left = 0;
			int Top = 0;
			int Right = 0;
			int Bottom = 0;
		};
	public:
		Canvas() = default;
		Canvas(const Canvas&) = default;
		Canvas(Canvas&&) = default;

		static int GetLeft(const Ref<UIElement>& element)
		{
			return element->GetLayoutContext<Context>().Left;
		}

		static int GetTop(const Ref<UIElement>& element)
		{
			return element->GetLayoutContext<Context>().Top;
		}

		static int GetRight(const Ref<UIElement>& element)
		{
			return element->GetLayoutContext<Context>().Right;
		}

		static int GetBottom(const Ref<UIElement>& element)
		{
			return element->GetLayoutContext<Context>().Bottom;
		}

		static void SetLeft(const Ref<UIElement>& element, int value)
		{
			element->GetLayoutContext<Context>().Left = value;
		}

		static void SetTop(const Ref<UIElement>& element, int value)
		{
			element->GetLayoutContext<Context>().Top = value;
		}

		static void SetRight(const Ref<UIElement>& element, int value)
		{
			element->GetLayoutContext<Context>().Right = value;
		}

		static void SetBottom(const Ref<UIElement>& element, int value)
		{
			element->GetLayoutContext<Context>().Bottom = value;
		}
	protected:
		FSize MeasureContent(FSize avaliableSize) override;
		void ArrangeContent(FRect rect) override;	
	};
}