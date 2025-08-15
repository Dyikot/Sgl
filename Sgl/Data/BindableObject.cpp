#include "BindableObject.h"

namespace Sgl
{
	BindableObject::BindableObject(const BindableObject& other):
		DataContext(other.DataContext),
		_observers(other._observers)
	{}

	BindableObject::BindableObject(BindableObject&& other) noexcept:
		DataContext(std::move(other.DataContext)),
		_observers(std::move(other._observers))
	{}
}
