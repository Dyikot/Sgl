#pragma once

#include "../UI/Component.h"
#include <set>

namespace Sgl
{
	template<typename T> requires std::derived_from<T, Component>
	class ComponentSet: public std::multiset<std::reference_wrapper<T>, ZIndexComparer>
	{
	public:
		template<typename TPredicate>
		const T* find(TPredicate predicate) const
		{
			for(const T& child : *this)
			{
				if(predicate(child))
				{
					return &child;
				}
			}

			return nullptr;
		}

		bool contains(const T& object) const
		{
			for(const T& child : *this)
			{
				if(child == object)
				{
					return true;
				}
			}

			return false;
		}
	};

}