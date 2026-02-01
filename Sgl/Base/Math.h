#pragma once

#include <vector>

namespace Sgl::Math
{
	/// <summary>
	/// Generates a vector of sine values for a full cycle (0 to 2π) sampled uniformly over the specified number of points.
	/// </summary>
	/// <param name="size"> - the number of samples to generate. If zero, returns an empty vector.</param>
	/// <returns>A vector containing sin(2π * i / size) for i in [0, size).</returns>
	std::vector<float> SinRange(size_t size) noexcept;

	/// <summary>
	/// Generates a vector of cosine values for a full cycle (0 to 2π) sampled uniformly over the specified number of points.
	/// </summary>
	/// <param name="size"> - the number of samples to generate. If zero, returns an empty vector.</param>
	/// <returns>A vector containing cos(2π * i / size) for i in [0, size).</returns>
	std::vector<float> CosRange(size_t size) noexcept;

	/// <summary>
	/// Generates a triangle index list for rendering a filled ellipse (or circle) as a fan of triangles.
	/// The ellipse is approximated by the given number of boundary points, and indices are output in 
	/// a format suitable for indexed drawing.
	/// </summary>
	/// <param name="verticesNumber"> - the number of vertices on the ellipse perimeter (must be ≥ 3).</param>
	/// <returns>A vector of vertex indices forming triangles that cover the ellipse, typically using a central point (index 0) and consecutive edge points.</returns>
	std::vector<int> TriangulateEllipse(size_t verticesNumber);
}