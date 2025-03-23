#include "BindableObject.h"

namespace Sgl
{
	void BindableObject::OnPropertyChanged(const PropertyId& id)
	{
		if(auto found = _bindings.find(id); found != _bindings.end())
		{
			auto& binding = found->second;

			binding.IsLock = true;
			binding.UpdateTarget();
			binding.IsLock = false;
		}
	}
}