#pragma once

#include "../Data/AnyMap.h"

namespace Sgl
{
	using PropertyId = size_t;

	class BindableObject
	{
	private:
		AnyMap<PropertyId> _properties;
	public:
		virtual ~BindableObject() = default;

		template<typename TValue>
		const TValue& GetPropertyValue(PropertyId id) const
		{
			return _properties.at(id).As<TValue>();
		}

		template<typename TValue>
		TValue& GetPropertyValue(PropertyId id)
		{
			return _properties[id].As<TValue>();
		}

		Any& GetProperty(PropertyId id)
		{
			return _properties[id];
		}

		const Any& GetProperty(PropertyId id) const
		{
			return _properties.at(id);
		}

		template<typename TValue>
		void SetProperty(PropertyId id, const TValue& value)
		{
			_properties[id].As<TValue>() = value;
			OnPropertyChanged(id);
		}
	protected:
		template<typename TValue, typename... TArgs>
		void AddProperty(PropertyId id, TArgs&&... args)
		{
			_properties.Add<TValue>(id, std::forward<TArgs>(args)...);
		}

		virtual void OnPropertyChanged(PropertyId id) {}
	};
}
