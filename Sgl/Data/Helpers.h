#pragma once

#include <type_traits>

namespace Sgl
{
	template<typename T>
	using Ref = std::reference_wrapper<T>;

	struct Size
	{
		size_t Width;
		size_t Height;
	};

	struct FSize
	{
		float Width;
		float Height;
	};
}