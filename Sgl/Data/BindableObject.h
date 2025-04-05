#pragma once

#include <functional>
#include <assert.h>
#include "Any.h"
#include "PropertyId.h"
#include "../Tools/Delegates.h"

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
		Callable<Function<void>> UpdateTarget;
		Callable<Function<void>> UpdateSource;
		bool IsLock = false;
	};

	class BindableObject: public IBindingSource
	{
	public:
		template<typename TData, typename TMember>
		using Setter = void (TData::*)(const TMember&);

		template<typename TData, typename TMember>
		using Getter = const TMember& (TData::*)() const;

		template<typename TData, typename TMember>
		struct UpdateTarget: public Function<void>
		{
			BindableObject* Object;
			Setter<TData, TMember> Setter;
			TData* Data;
			const PropertyId& Id;

			UpdateTarget(BindableObject* obejct, BindableObject::Setter<TData, TMember> setter,
						 TData* data, const PropertyId& id):
				Object(obejct), Setter(setter), Data(data), Id(id)
			{}

			void operator()() override
			{
				std::invoke(Setter, Data, std::cref(Object->GetPropertyValue<TMember>(Id)));
			}

			void operator()() const override {}
		};

		template<typename TData, typename TMember>
		struct UpdateSource: public Function<void>
		{
			BindableObject* Object;
			Setter<TData, TMember> Getter;
			TData* Data;
			const PropertyId& Id;

			UpdateSource(BindableObject* obejct, BindableObject::Getter<TData, TMember> getter,
						 TData* data, const PropertyId& id):
				Object(obejct), Getter(getter), Data(data), Id(id)
			{}

			void operator()() override
			{
				Object->_properties[Id].As<TMember>() = std::invoke(Getter, std::ref(Data));
			}

			void operator()() const override {}
		};
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

		template<typename TData, typename TMember>
		void Bind(const PropertyId& id, TData& data, Setter<TData, TMember> setter)
		{
			assert(id.Type == typeid(TMember));

			_bindings[id].UpdateTarget = new UpdateTarget<TData, TMember>(this, setter, &data, id);
		}

		template<std::derived_from<IBindingTarget> TData, typename TMember>
		void Bind(const PropertyId& id, TData& data, Getter<TData, TMember> getter)
		{
			assert(id.Type == typeid(TMember));

			data.AddSource(*this);
			_bindings[id].UpdateSource = new UpdateSource<TData, TMember>(this, getter, &data, id);
		}

		template<typename TData, typename TMember>
		void Bind(const PropertyId& id, TData& data, 
				  Setter<TData, TMember> setter,
				  Getter<TData, TMember> getter)
		{
			Bind(id, data, setter);
			Bind(id, data, getter);
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
