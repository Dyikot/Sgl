#pragma once

#include "../Base/Primitives.h"

namespace Sgl
{
	class LayoutHelper
	{
	public:
		static bool IsPointInRect(float x, float y, FRect rect) noexcept
		{
			return x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h;
		}

		static bool IsPointInRect(int x, int y, Rect rect) noexcept
		{
			return x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h;
		}
	};
}