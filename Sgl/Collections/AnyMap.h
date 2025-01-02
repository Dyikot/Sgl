#pragma once

#include "unordered_map"
#include "../Any.h"

namespace Sgl
{
	template<typename TKey>
	class AnyMap: public std::unordered_map<TKey, Any>
	{
	public:
		using Base = std::unordered_map<TKey, Any>;
	public:
		template<typename TValue, typename... TArgs>
		void Add(const TKey& key, TArgs&&... args)
		{
			Base::emplace(key, Any::New<TValue>(std::forward<TArgs>(args)...));
		}
	};
}