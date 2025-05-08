#pragma once

#include <vector>
#include "../Graphic/Views.h"

namespace Sgl::Math
{
	std::vector<int> TriangulateConvexShape(PointsView points);
	std::vector<int> TriangulateConvexShape(PointsView points, SDL_FPoint center);
}