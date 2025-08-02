#pragma once

#include "../../Base/Observable/IObserver.h"

namespace Sgl
{
	template<typename TOwner, typename T>
	class PropertyObserver: public IObserver<T>
	{
	public:
		using Setter = void(TOwner::*)(T);
	private:
		TOwner& _owner;
		Setter _setter;
	public:
		PropertyObserver(TOwner& owner, Setter setter):
			_owner(owner),
			_setter(setter)
		{}

		void OnNext(T value) override
		{
			(_owner.*_setter)(value);
		}
	};
}