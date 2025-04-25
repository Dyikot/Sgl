#pragma once

#include <type_traits>
#include <concepts>

namespace Sgl
{
	template<typename T> requires std::default_initializable<T>
	class Property
	{
	public:
		
	};
}