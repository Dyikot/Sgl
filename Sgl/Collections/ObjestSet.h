#pragma once

#include "../UI/Object.h"
#include <set>

namespace Sgl
{
	template<typename T> requires std::derived_from<T, Object>
	class ObjestSet: public std::multiset<T*, ZIndexComparer>
	{
	public:
		template<typename TPredicate>
		const T* find(TPredicate predicate) const
		{
			for(auto child : *this)
			{
				if(predicate(child))
				{
					return child;
				}
			}

			return nullptr;
		}

		bool contains(const T* object) const
		{
			for(auto& child : *this)
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