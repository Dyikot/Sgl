#pragma once

#include "../Base/Collections/Vector.h"
#include "Setter.h"

namespace Sgl
{
	class SetterCollection : public Vector<Ref<ISetter>>
	{
	public:
		using Vector::Vector;
		SetterCollection(const SetterCollection&) = default;
		SetterCollection(SetterCollection&&) noexcept = default;

		SetterCollection(std::initializer_list<ISetter*> setters)
		{
			_data.reserve(setters.size());

			for(auto setter : setters)
			{
				_data.emplace_back(setter);
			}
		}

		SetterCollection& operator=(const SetterCollection&) = default;
		SetterCollection& operator=(SetterCollection&&) noexcept = default;
	};
}