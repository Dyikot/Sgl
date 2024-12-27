#pragma once

#include <functional>

namespace Sgl
{
	template<typename... TArgs>
	using Action = std::function<void(TArgs...)>;

	template<typename T>
	using Predicate = std::function<bool(T)>;
}