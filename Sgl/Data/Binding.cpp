#include "Binding.h"
#include "Bindable.h"

namespace Sgl
{
	const Ref<ObservableObject>& BindingBase::GetDataContext(const Bindable& bindableObject)
	{
		return bindableObject.GetDataContext();
	}
}