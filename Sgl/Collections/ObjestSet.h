#pragma once

#include "../UI/Object.h"
#include <set>

namespace Sgl
{
	template<typename T> requires std::derived_from<T, Object>
	class ObjestSet: public std::multiset<std::reference_wrapper<T>, ZIndexComparer>
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