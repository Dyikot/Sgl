#pragma once

#include "../UIElement/Panel.h"

namespace Sgl::UIElements
{
	class Canvas : public Panel
	{
	public:
		struct AttachedProperties
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

		int GetLeft(const Ref<UIElement>& element)
		{
			return element->GetAttachedValue(&AttachedProperties::Left);
		}

		int GetTop(const Ref<UIElement>& element)
		{
			return element->GetAttachedValue(&AttachedProperties::Top);
		}

		int GetRight(const Ref<UIElement>& element)
		{
			return element->GetAttachedValue(&AttachedProperties::Right);
		}

		int GetBottom(const Ref<UIElement>& element)
		{
			return element->GetAttachedValue(&AttachedProperties::Bottom);
		}

		void SetLeft(const Ref<UIElement>& element, int value)
		{
			element->SetAttachedValue(&AttachedProperties::Left, value);
		}

		void SetTop(const Ref<UIElement>& element, int value)
		{
			element->SetAttachedValue(&AttachedProperties::Top, value);
		}

		void SetRight(const Ref<UIElement>& element, int value)
		{
			element->SetAttachedValue(&AttachedProperties::Right, value);
		}

		void SetBottom(const Ref<UIElement>& element, int value)
		{
			element->SetAttachedValue(&AttachedProperties::Bottom, value);
		}
	protected:
		FSize MeasureContent(FSize avaliableSize) override;
		void ArrangeContent(FRect rect) override;	
	};
}