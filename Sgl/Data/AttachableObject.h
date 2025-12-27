#pragma once

#include <unordered_map>
#include "../Base/Any.h"
#include "BindableObject.h"
#include "AttachedProperty.h"

namespace Sgl
{
	class AttachableObject : public BindableObject
	{
	private:
		std::unordered_map<AttachedPropertyBase*, Any> _attachedValues;
	public:
		AttachableObject() = default;
		AttachableObject(const AttachableObject& other): 
			_attachedValues(other._attachedValues)
		{}
		AttachableObject(AttachableObject&& other) noexcept: 
			_attachedValues(std::move(other._attachedValues)) 
		{}

		template<typename T>
		void SetAttachedValue(AttachedProperty<T>& property, const T& value)
		{
			Any& attachedValue = _attachedValues[&property];

			if(attachedValue.HasValue())
			{
				attachedValue.As<T>() = value;
			}
			else
			{
				attachedValue = Any::New<T>(value);
			}
		}

		template<typename T>
		const T& GetAttachedValue(AttachedProperty<T>& property) const
		{
			if(auto it = _attachedValues.find(&property); it != _attachedValues.end())	
			{
				return it->second.As<T>();
			}

			return property.DefaultValue;
		}

		void ClearAttachedValue(AttachedPropertyBase& property)
		{
			_attachedValues.erase(&property);
		}
	};
}