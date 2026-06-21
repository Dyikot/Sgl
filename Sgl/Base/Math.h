#pragma once

#include <vector>
#include <array>
#include "Primitives.h"

namespace Sgl::Math
{
	/// <summary>
	/// Generates a vector of sine values for a full cycle (0 to 2π) sampled uniformly over the specified number of points.
	/// </summary>
	/// <param name="count"> - the number of samples to generate. If zero, returns an empty vector.</param>
	/// <returns>A vector containing sin(2π * i / size) for i in [0, size).</returns>
	std::vector<float> SinRange(size_t count);

	/// <summary>
	/// Generates a vector of cosine values for a full cycle (0 to 2π) sampled uniformly over the specified number of points.
	/// </summary>
	/// <param name="count"> - the number of samples to generate. If zero, returns an empty vector.</param>
	/// <returns>A vector containing cos(2π * i / size) for i in [0, size).</returns>
	std::vector<float> CosRange(size_t count);

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