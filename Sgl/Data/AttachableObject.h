#pragma once

#include <unordered_map>
#include "../Base/Any.h"
#include "AttachProperty.h"
#include "ObservableObject.h"

namespace Sgl
{
	class AttachableObject : public ObservableObject
	{
	private:
		std::unordered_map<PropertyId, Any> _attachedProperties;
	public:
		AttachableObject() = default;
		AttachableObject(const AttachableObject&) = default;
		AttachableObject(AttachableObject&&) = default;
		~AttachableObject() = default;

		template<typename T>
		void SetAttachProperty(AttachedProperty<T>& attachProperty, const T& value)
		{
			_attachedProperties[attachProperty.Id] = Any::New<T>(value);
		}

		template<typename T>
		const T& GetAttachProperty(AttachedProperty<T>& attachProperty) const
		{
			if(auto it = _attachedProperties.find(attachProperty.Id); it != _attachedProperties.end())	
			{
				return it->second.As<T>();
			}

			return attachProperty.DefaultValue;
		}

		void ClearAttachProperty(SglProperty& attachProperty)
		{
			_attachedProperties.erase(attachProperty.Id);
		}
	};
}