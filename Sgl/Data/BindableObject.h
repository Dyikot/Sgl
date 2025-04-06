#pragma once

#include <functional>
#include <assert.h>
#include "Any.h"
#include "PropertyId.h"
#include "Delegates.h"

namespace Sgl
{
	class IBindingSource
	{
	public:
		virtual ~IBindingSource() = default;

		virtual void Update(const PropertyId& id) = 0;
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
		Func<void> UpdateTarget;
		Func<void> UpdateSource;
		bool IsLock = false;
	};

	class BindableObject: public IBindingSource
	{
	private:
		AnyMap<PropertyId> _properties;
		std::unordered_map<PropertyId, Binding> _bindings;
	public:
		void Update(const PropertyId& id)
		{
			if(auto found = _bindings.find(id); found != _bindings.end() && !found->second.IsLock)
			{
				found->second.UpdateSource();
			}
		}

		template<typename TMember, typename TData>
		void Bind(const PropertyId& id, TData& data, CAction<TData*, TMember> auto&& setter)
		{
			assert(id.Type == typeid(TMember));

			_bindings[id].UpdateTarget = [this, setter = std::move(setter), &data, &id]
			{
				std::invoke(setter, data, std::cref(GetPropertyValue<TMember>(id)));
			};
		}

		template<typename TMember, std::derived_from<IBindingTarget> TData>
		void Bind(const PropertyId& id, TData& data, CFunc<TMember, TData*> auto&& getter)
		{
			assert(id.Type == typeid(TMember));

			data.AddSource(*this);
			_bindings[id].UpdateSource = [this, getter = std::move(getter), &data, &id]
			{
				_properties[id].As<TMember>() = std::invoke(getter, std::ref(data));
			};
		}

		template<typename TMember, typename TData>
		void Bind(const PropertyId& id, TData& data, 
				  CAction<TData*, TMember> auto&& setter,
				  CFunc<TMember, TData*> auto&& getter)
		{
			Bind(id, data, std::forward<decltype(setter)>(setter));
			Bind(id, data, std::forward<decltype(getter)>(getter));
		}

		template<typename TData>
		void Unbind(const PropertyId& id, TData& data)
		{
			if constexpr(std::is_base_of_v<IBindingTarget, TData>)
			{
				data.RemoveSource(*this);
			}

			_bindings.erase(id);
		}
	protected:
		template<typename TValue>
		const TValue& GetPropertyValue(const PropertyId& id) const
		{
			return _properties.at(id).As<TValue>();
		}

		template<typename TValue>
		TValue& GetPropertyValue(const PropertyId& id)
		{
			return _properties[id].As<TValue>();
		}

		Any& GetProperty(const PropertyId& id)
		{
			return _properties[id];
		}

		const Any& GetProperty(const PropertyId& id) const 
		{
			return _properties.at(id);
		}

		template<typename TValue>
		void SetProperty(const PropertyId& id, const TValue& value)
		{
			_properties[id].As<TValue>() = value;
			OnPropertyChanged(id);
		}

		template<typename TValue, typename... TArgs>
		void AddProperty(const PropertyId& id, TArgs&&... args)
		{
			assert(typeid(TValue) == id.Type);

			_properties.Add<TValue>(id, std::forward<TArgs>(args)...);
		}

		void OnPropertyChanged(const PropertyId& id)
		{
			if(auto found = _bindings.find(id); found != _bindings.end())
			{
				auto& binding = found->second;

				binding.IsLock = true;
				binding.UpdateTarget();
				binding.IsLock = false;
			}
		}
	};
}
