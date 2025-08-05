#pragma once

namespace Sgl
{
	/// <summary>
	/// <para>None - The content preserves its original size.</para>
	/// <para>Fill - The content is resized to fill the destination dimensions. The aspect ratio is not preserved.</para>
	/// <para>Uniform - The content is resized to fit in the destination dimensions while it preserves its native aspect ratio.</para>
	/// <para>UniformToFill - The content is resized to fill the destination dimensions while it preserves its native aspect ratio. If the aspect ratio of the destination rectangle differs from the source, the source content is clipped to fit in the destination dimensions.</para>
	/// </summary>
	enum class Stretch
	{
		None, Fill, Uniform, UniformToFill
	};
}