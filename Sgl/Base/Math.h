#pragma once

#include <vector>
#include <numbers>
#include <numeric>
#include <cmath>
#include "Primitives.h"

namespace Sgl
{
	class Math
	{
	public:
		static std::vector<float> SinRange(size_t size) noexcept;
		static std::vector<float> CosRange(size_t size) noexcept;
		static std::vector<int> TriangulateEllipse(size_t pointsNumber);
		static bool IsPointInRect(FPoint point, FRect rect) noexcept;
	};
}