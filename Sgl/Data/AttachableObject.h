#pragma once

#include <unordered_map>
#include <stdexcept>
#include "../Base/Any.h"
#include "AttachProperty.h"

namespace Sgl
{
	class AttachableObject
	{
	private:
		std::unordered_map<size_t, Any> _attachedProperties;
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

		template<typename T>
		void ClearAttachProperty(AttachedProperty<T>& attachProperty)
		{
			_attachedProperties.erase(attachProperty.Id);
		}
	};
}