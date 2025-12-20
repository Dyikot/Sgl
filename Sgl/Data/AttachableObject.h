#pragma once

#include <vector>
#include "../Base/Any.h"
#include "AttachProperty.h"
#include "ObservableObject.h"

namespace Sgl
{
	class AttachableObject : public ObservableObject
	{
	private:
		std::vector<Any> _attachedProperties;
	public:
		AttachableObject(): _attachedProperties(AttachedPropertyBase::_nextId) {}
		AttachableObject(const AttachableObject&) = default;
		AttachableObject(AttachableObject&&) = default;
		~AttachableObject() = default;

		template<typename T>
		void SetAttachProperty(AttachedProperty<T>& attachProperty, const T& value)
		{
			if(auto& property = _attachedProperties[attachProperty.Id]; property.HasValue())
			{
				property.As<T>() = value;
			}
			else
			{
				_attachedProperties[attachProperty.Id] = Any::New<T>(value);
			}
		}

		template<typename T>
		const T& GetAttachProperty(AttachedProperty<T>& attachProperty) const
		{
			if(auto& property = _attachedProperties[attachProperty.Id]; property.HasValue())
			{
				return property.As<T>();
			}

			return attachProperty.DefaultValue;
		}

		void ClearAttachProperty(AttachedPropertyBase& attachProperty)
		{
			_attachedProperties[attachProperty.Id] = nullptr;
		}
	};
}