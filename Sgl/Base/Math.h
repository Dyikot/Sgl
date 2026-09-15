#pragma once

#include <vector>
#include <array>
#include "Primitives.h"

namespace Sgl::Math
{
	//! @brief Generates a vector of sine values for a full cycle (0 to 2π) sampled uniformly over the specified number of points
	//! @param count The number of samples to generate. If zero, returns an empty vector.
	//! @return A vector containing sin(2π * i / count) for i in [0, count)
	std::vector<float> SinRange(size_t count);

	//! @brief Generates a vector of cosine values for a full cycle (0 to 2π) sampled uniformly over the specified number of points
	//! @param count The number of samples to generate. If zero, returns an empty vector.
	//! @return A vector containing cos(2π * i / count) for i in [0, count)
	std::vector<float> CosRange(size_t count);

	//! @brief Generates an array of indices for fan triangulation of a polygon
	//! @tparam N The total number of indices to generate
	//! @param lastVertex The index of the last vertex to close the polygon
	//! @return An array of indices representing the triangulated fan
	template<size_t N>
	constexpr std::array<int, N> FanTriangulate(int lastVertex)
	{
		std::array<int, N> indices;
		const int center = 0;
		size_t j = 0;
		for(size_t i = 1; i < N / 3; i++)
		{
			indices[j++] = center;
			indices[j++] = i;
			indices[j++] = i + 1;
		}
		indices[j++] = center;
		indices[j++] = lastVertex;
		indices[j++] = 1;
		return indices;
	}
}