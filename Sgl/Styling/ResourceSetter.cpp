#include "ResourceSetter.h"
#include "../Application.h"

namespace Sgl
{
	ResourceSetterBase::ResourceSetterBase(PropertyBase& property, std::string key):
		SetterBase(property),
		_key(std::move(key))
	{}

	Color ResourceSetterBase::GetColor() const
	{
		return App->Resources.GetColor(_key);
	}

	Brush ResourceSetterBase::GetBrush() const
	{
		return App->Resources.GetBrush(_key);
	}
}