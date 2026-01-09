#pragma once

#include <vector>
#include <unordered_map>
#include "../Base/Ref.h"
#include "../Base/Any.h"
#include "../Base/Exceptions.h"
#include "BindingOperations.h"
#include "StyleableProperty.h"
#include "AttachedProperty.h"

namespace Sgl
{
	class BindingBase;

	template<typename TTarget, typename TSource, typename TValue>
	class PropertyBinding;

	template<typename TTarget, typename TSource, typename TValue, typename TEvent>
	class EventBinding;

	enum class BindingMode
	{
		OneWay, OneWayToSource, TwoWay
	};

	class BindableObject : public INotifyPropertyChanged
	{
	private:
		class BindingContext
		{
		private:
			BindingBase* _binding;
			BindableObject* _bindgableObject;
		public:
			BindingContext(BindingBase* binding, BindableObject* bindableObject);
			void Clear();
		};

		struct Observer
		{
			std::reference_wrapper<PropertyBase> Property;
			PropertyChangedEventHandler Handler;

			bool operator==(const Observer& other) const
			{
				return Handler == other.Handler;
			}
		};		

		std::unordered_map<PropertyBase*, Any> _attachedValues;
		std::vector<Observer> _propertiesObservers;
		std::vector<std::unique_ptr<BindingBase>> _bindings;
		Ref<INotifyPropertyChanged> _dataContext;

		ValueSource _dataContextSource {};
	public:
		BindableObject() = default;
		BindableObject(const BindableObject& other);
		BindableObject(BindableObject&& other) noexcept;
		~BindableObject();	

		void SetDataContext(const Ref<INotifyPropertyChanged>& value, ValueSource source = ValueSource::Local);
		const Ref<INotifyPropertyChanged>& GetDataContext() const { return _dataContext; }

		void AddPropertyChangedEventHandler(PropertyBase& property, PropertyChangedEventHandler handler) override;
		void RemovePropertyChangedEventHandler(PropertyBase& property, PropertyChangedEventHandler handler) override;

		template<typename TTarget, typename TSource, typename TValue>
		BindingContext Bind(Property<TTarget, TValue>& targetProperty,
						    Property<TSource, TValue>& sourceProperty,
						    BindingMode mode = BindingMode::OneWay)
		{
			const auto& binding = _bindings.emplace_back(
				new PropertyBinding(targetProperty, sourceProperty, mode)
			);

			return BindingContext(binding.get(), this);
		}

		template<typename TTarget, typename TSource, typename TValue, CEvent TEvent>
		BindingContext Bind(Property<TTarget, TValue>& targetProperty,
							Property<TSource, TValue>& sourceProperty,
							TEvent Property<TTarget, TValue>::Owner::* event,
							BindingMode mode = BindingMode::OneWayToSource)
		{
			const auto& binding = _bindings.emplace_back(
				new EventBinding(targetProperty, sourceProperty, event, mode)
			);

			return BindingContext(binding.get(), this);
		}

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

		void ClearAttachedValue(PropertyBase& property)
		{
			_attachedValues.erase(&property);
		}
	protected:
		virtual void NotifyPropertyChanged(PropertyBase& property);
		virtual void OnDataContextChanged(const Ref<INotifyPropertyChanged>& dataContext) {}
		void ApplyBindings();
		void ClearBindings();

		template<typename TOwner, typename TValue, typename TField>
		bool SetProperty(StyleableProperty<TOwner, TValue>& property, TField& field,
						 StyleableProperty<TOwner, TValue>::Value value,
						 ValueSource& currentSource, ValueSource newSource)
		{
			if(currentSource > newSource || field == value)
			{
				return false;
			}

			field = value;
			currentSource = newSource;
			NotifyPropertyChanged(property);

			return true;
		}
	public:
		static inline StyleableProperty DataContextProperty { &SetDataContext, &GetDataContext };
	};

	class BindingBase
	{
	protected:
		bool _applied = false;
	public:
		virtual ~BindingBase() = default;

		bool IsApplied() const noexcept { return _applied; }

		virtual void Apply(BindableObject& bindableObject) = 0;
		virtual void Clear(BindableObject& bindableObject) = 0;
	};

	template<typename TTarget, typename TSource, typename TValue>
	class PropertyBinding : public BindingBase
	{
	private:
		Property<TTarget, TValue>& _targetProperty;
		Property<TSource, TValue>& _sourceProperty;
		BindingMode _mode;
	public:
		PropertyBinding(Property<TTarget, TValue>& targetProperty,
				Property<TSource, TValue>& sourceProperty,
				BindingMode mode):
			_targetProperty(targetProperty),
			_sourceProperty(sourceProperty),
			_mode(mode)
		{}

		void Apply(BindableObject& bindableObject) final
		{
			auto& dataContext = bindableObject.GetDataContext();

			if(!dataContext)
			{
				throw Exception("Unable to set a binding. Data context is null.");
			}

			auto& target = static_cast<TTarget&>(bindableObject);
			auto& source = dataContext.GetValueAs<TSource>();

			switch(_mode)
			{
				case BindingMode::OneWay:
				{
					_targetProperty.InvokeSetter(target, _sourceProperty.InvokeGetter(source));
					BindingOperations::Bind(_targetProperty, target, _sourceProperty, source);
					break;
				}

				case BindingMode::OneWayToSource:
				{
					_sourceProperty.InvokeSetter(source, _targetProperty.InvokeGetter(target));
					BindingOperations::Bind(_sourceProperty, source, _targetProperty, target);
					break;
				}

				case BindingMode::TwoWay:
				{
					_targetProperty.InvokeSetter(target, _sourceProperty.InvokeGetter(source));
					BindingOperations::Bind(_targetProperty, target, _sourceProperty, source);
					BindingOperations::Bind(_sourceProperty, source, _targetProperty, target);
					break;
				}
			}

			_applied = true;
		}

		void Clear(BindableObject& bindableObject) final
		{
			auto& dataContext = bindableObject.GetDataContext();

			if(!dataContext)
			{
				throw Exception("Unable to set a binding. Data context is null.");
			}

			auto& target = static_cast<TTarget&>(bindableObject);
			auto& source = dataContext.GetValueAs<TSource>();

			switch(_mode)
			{
				case BindingMode::OneWay:
				{
					BindingOperations::Unbind(_targetProperty, target, _sourceProperty, source);
					break;
				}

				case BindingMode::OneWayToSource:
				{
					BindingOperations::Unbind(_sourceProperty, source, _targetProperty, target);
					break;
				}

				case BindingMode::TwoWay:
				{
					BindingOperations::Unbind(_targetProperty, target, _sourceProperty, source);
					BindingOperations::Unbind(_sourceProperty, source, _targetProperty, target);
					break;
				}
			}

			_applied = false;
		}
	};

	template<typename TTarget, typename TSource, typename TValue, typename TEvent>
	class EventBinding : public BindingBase
	{
	private:
		using TOwner = Property<TTarget, TValue>::Owner;

		Property<TTarget, TValue>& _targetProperty;
		Property<TSource, TValue>& _sourceProperty;
		TEvent TOwner::* _event;
		BindingMode _mode;
	public:
		EventBinding(Property<TTarget, TValue>& targetProperty,
					 Property<TSource, TValue>& sourceProperty,
					 TEvent TOwner::* event,
					 BindingMode mode):
			_targetProperty(targetProperty),
			_sourceProperty(sourceProperty),
			_event(event),
			_mode(mode)
		{}

		void Apply(BindableObject& bindableObject) final
		{
			auto& dataContext = bindableObject.GetDataContext();

			if(!dataContext)
			{
				throw Exception("Unable to set a binding. Data context is null.");
			}

			auto& target = static_cast<TTarget&>(bindableObject);
			auto& source = dataContext.GetValueAs<TSource>();

			switch(_mode)
			{
				case BindingMode::OneWay:
				{
					_targetProperty.InvokeSetter(target, _sourceProperty.InvokeGetter(source));
					BindingOperations::Bind(_targetProperty, target, _sourceProperty, source);
					break;
				}

				case BindingMode::OneWayToSource:
				{
					_sourceProperty.InvokeSetter(source, _targetProperty.InvokeGetter(target));
					BindingOperations::Bind(_sourceProperty, source, _targetProperty, target, _event);
					break;
				}

				case BindingMode::TwoWay:
				{
					_targetProperty.InvokeSetter(target, _sourceProperty.InvokeGetter(source));
					BindingOperations::Bind(_targetProperty, target, _sourceProperty, source);
					BindingOperations::Bind(_sourceProperty, source, _targetProperty, target, _event);
					break;
				}
			}

			_applied = true;
		}

		void Clear(BindableObject& bindableObject) final
		{
			auto& dataContext = bindableObject.GetDataContext();

			if(!dataContext)
			{
				throw Exception("Unable to set a binding. Data context is null.");
			}

			auto& target = static_cast<TTarget&>(bindableObject);
			auto& source = dataContext.GetValueAs<TSource>();

			switch(_mode)
			{
				case BindingMode::OneWay:
				{
					BindingOperations::Unbind(_targetProperty, target, _sourceProperty, source);
					break;
				}

				case BindingMode::OneWayToSource:
				{
					BindingOperations::Unbind(_sourceProperty, source, _targetProperty, target, _event);
					break;
				}

				case BindingMode::TwoWay:
				{
					BindingOperations::Unbind(_targetProperty, target, _sourceProperty, source);
					BindingOperations::Unbind(_sourceProperty, source, _targetProperty, target, _event);
					break;
				}
			}

			_applied = false;
		}
	};
}