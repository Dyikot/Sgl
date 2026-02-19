#pragma once

#include <vector>
#include "../Base/Ref.h"
#include "../Base/Any.h"
#include "../Base/Exceptions.h"
#include "BindingOperations.h"
#include "StyleableProperty.h"
#include "AttachPropertiesRegister.h"

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
		using DestoyingEventHandler = EventHandler<BindableObject>;

		class BindingContext
		{
		public:
			BindingContext(BindingBase* binding, BindableObject* bindableObject);
			void Clear();
		private:
			BindingBase* _binding;
			BindableObject* _bindgableObject;
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
	public:
		BindableObject() = default;
		BindableObject(const BindableObject& other);
		BindableObject(BindableObject&& other) noexcept;
		~BindableObject();	

		Event<DestoyingEventHandler> Destroying;

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

		template<typename... TArgs>
		void UseAttachedProperties()
		{
			((Destroying += [](BindableObject& sender, EventArgs e)
			{
				using TProperties = TArgs::AttachedProperties;
				auto& reg = AttachPropertiesRegister<TProperties>::Get();
				reg.ClearAttachedProperty(&sender);
			}), ...);
		}

		template<typename T, typename TValue, typename TSetValue>
			requires std::constructible_from<TSetValue, TValue>
		void SetAttachedValue(TValue T::* field, const TSetValue& value)
		{
			auto& reg = AttachPropertiesRegister<T>::Get();
			auto& property = reg.GetAttachedProperty(this);
			property.*field = value;
		}

		template<typename T, typename TValue>
		TValue GetAttachedValue(TValue T::* field) const
		{
			auto& reg = AttachPropertiesRegister<T>::Get();
			auto& property = reg.GetAttachedProperty(this);
			return property.*field;
		}

		static inline StyleableProperty DataContextProperty { &SetDataContext, &GetDataContext };
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
	private:
		std::vector<Observer> _propertiesObservers;
		std::vector<std::unique_ptr<BindingBase>> _bindings;
		Ref<INotifyPropertyChanged> _dataContext;

		ValueSource _dataContextSource {};
	};

	class BindingBase
	{
	public:
		virtual ~BindingBase() = default;

		bool IsApplied() const noexcept { return _applied; }

		virtual void Apply(BindableObject& bindableObject) = 0;
		virtual void Clear(BindableObject& bindableObject) = 0;
	protected:
		bool _applied = false;
	};

	template<typename TTarget, typename TSource, typename TValue>
	class PropertyBinding : public BindingBase
	{
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
	private:
		Property<TTarget, TValue>& _targetProperty;
		Property<TSource, TValue>& _sourceProperty;
		BindingMode _mode;
	};

	template<typename TTarget, typename TSource, typename TValue, typename TEvent>
	class EventBinding : public BindingBase
	{
	private:
		using TOwner = Property<TTarget, TValue>::Owner;
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
	private:
		Property<TTarget, TValue>& _targetProperty;
		Property<TSource, TValue>& _sourceProperty;
		TEvent TOwner::* _event;
		BindingMode _mode;
	};
}