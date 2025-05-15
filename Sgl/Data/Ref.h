#pragma once

#include <type_traits>

namespace Sgl
{
	template<typename T>
	using Ref = std::reference_wrapper<T>;
}