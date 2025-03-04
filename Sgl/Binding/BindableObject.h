#pragma once

#include <functional>
#include <assert.h>
#include "../Data/Any.h"
#include "PropertyManager.h"

namespace Sgl
{
	class IBindingSource
	{
	public:
		virtual ~IBindingSource() = default;

		virtual void Update(PropertyId id) = 0;
	};

	class IBindingTarget
	{
	public:
		virtual ~IBindingTarget() = default;

		virtual void AddSource(IBindingSource& source) = 0;
		virtual void RemoveSource(IBindingSource& source) = 0;
	};

	struct Binding
	{
		std::function<void()> UpdateSource;
		std::function<void()> UpdateTarget;
		bool IsLock = false;
	};	

	class BindableObject: public IBindingSource
	{
	public:
		template<typename TData, typename TMember>
		using Setter = void (TData::*)(const TMember&);

		template<typename TData, typename TMember>
		using Getter = const TMember& (TData::*)() const;
	private:
		AnyMap<PropertyId> _properties;
		std::unordered_map<PropertyId, Binding> _bindings;
	public:
		virtual ~BindableObject() = default;

		void Update(PropertyId id)
		{
			if(auto found = _bindings.find(id); found != _bindings.end() && !found->second.IsLock)
			{
				found->second.UpdateSource();
			}
		}

		template<typename TData, typename TMember>
		void Bind(PropertyId id, TData& data, Setter<TData, TMember> setter)
		{
			assert(PropertyManager::GetTypeNameBy(id) == typeid(TMember).name());

			_bindings[id].UpdateTarget = [this, setter, &data, id]()
			{
				std::invoke(setter, data, std::cref(GetPropertyValue<TMember>(id)));
			};
		}

		template<typename TData, typename TMember> requires std::derived_from<TData, IBindingTarget>
		void Bind(PropertyId id, TData& data, Getter<TData, TMember> getter)
		{
			assert(PropertyManager::GetTypeNameBy(id) == typeid(TMember).name());

			data.AddSource(*this);

			_bindings[id].UpdateSource = [this, getter, &data, id]()
			{
				_properties[id].As<TMember>() = std::invoke(getter, std::ref(data));
			};
		}

		template<typename TData, typename TMember>
		void Bind(PropertyId id, TData& data, 
				  Setter<TData, TMember> setter,
				  Getter<TData, TMember> getter)
		{
			Bind(id, data, setter);
			Bind(id, data, getter);
		}

		template<typename TData>
		void Unbind(PropertyId id, TData& data)
		{
			if constexpr(std::is_base_of_v<IBindingTarget, TData>)
			{
				data.RemoveSource(*this);
			}

			_bindings.erase(id);
		}
	protected:
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

		Any& GetProperty(PropertyId id) { return _properties[id]; }
		const Any& GetProperty(PropertyId id) const { return _properties.at(id); }

		template<typename TValue>
		void SetProperty(PropertyId id, const TValue& value)
		{
			_properties[id].As<TValue>() = value;
			OnPropertyChanged(id);
		}

		template<typename TValue, typename... TArgs>
		void AddProperty(PropertyId id, TArgs&&... args)
		{
			_properties.Add<TValue>(id, std::forward<TArgs>(args)...);
		}

		void OnPropertyChanged(PropertyId id);
	};
}
