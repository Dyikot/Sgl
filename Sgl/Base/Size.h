#pragma once

namespace Sgl
{
	// <summary>
	/// Represents a 2D size with unsigned integer dimensions.
	/// </summary>
	struct Size
	{
		uint32_t Width;
		uint32_t Height;
	};

	/// <summary>
	/// Represents a 2D size with floating-point dimensions.
	/// </summary>
	struct FSize
	{
		float Width;
		float Height;
	};
}