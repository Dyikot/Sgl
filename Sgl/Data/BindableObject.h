#pragma once

#include <vector>
#include "../Base/Exceptions.h"
#include "../Base/Ref.h"
#include "Binding.h"
#include "StyleableProperty.h"

namespace Sgl
{
	enum class BindingMode
	{
		OneWay, OneWayToSource, TwoWay
	};

	class BindableObject : public INotifyPropertyChanged
	{
	private:
		struct Observer
		{
			std::reference_wrapper<AbstractPropertyBase> Property;
			PropertyChangedEventHandler Handler;

			bool operator==(const Observer& other) const
			{
				return Handler == other.Handler;
			}
		};		

		Ref<INotifyPropertyChanged> _dataContext;
		std::vector<Observer> _observers;

		ValueSource _dataContextSource {};
	public:
		BindableObject() = default;
		BindableObject(const BindableObject& other);
		BindableObject(BindableObject&& other) noexcept;

		void SetDataContext(const Ref<INotifyPropertyChanged>& value);
		void SetDataContext(const Ref<INotifyPropertyChanged>& value, ValueSource source);
		const Ref<INotifyPropertyChanged>& GetDataContext() const { return _dataContext; }

		void AddPropertyChangedEventHandler(AbstractPropertyBase& property, PropertyChangedEventHandler handler) override;
		void RemovePropertyChangedEventHandler(AbstractPropertyBase& property, PropertyChangedEventHandler handler) override;

		template<typename TTarget, typename TSource, typename TValue>
		void Bind(AbstractProperty<TTarget, TValue>& targetProperty,
				  AbstractProperty<TSource, TValue>& sourceProperty,
				  BindingMode mode = BindingMode::OneWay)
		{
			if(!_dataContext)
			{
				throw Exception("Unable to set a binding. Data context is null.");
			}

			auto& target = static_cast<TTarget&>(*this);
			auto& source = _dataContext.GetValueAs<TSource>();

			switch(mode)
			{
				case BindingMode::OneWay:
				{
					targetProperty.InvokeSetter(target, sourceProperty.InvokeGetter(source));
					Binding::Bind(targetProperty, target, sourceProperty, source);
					break;
				}

				case BindingMode::OneWayToSource:
				{
					sourceProperty.InvokeSetter(source, targetProperty.InvokeGetter(target));
					Binding::Bind(sourceProperty, source, targetProperty, target);
					break;
				}

				case BindingMode::TwoWay:
				{
					targetProperty.InvokeSetter(target, sourceProperty.InvokeGetter(source));
					Binding::Bind(targetProperty, target, sourceProperty, source);
					Binding::Bind(sourceProperty, source, targetProperty, target);
					break;
				}
			}			
		}

		template<typename TTarget, typename TSource, typename TValue, CEvent TEvent>
		void Bind(AbstractProperty<TTarget, TValue>& targetProperty,
				  AbstractProperty<TSource, TValue>& sourceProperty,
				  TEvent AbstractProperty<TTarget, TValue>::Owner::* event,
				  BindingMode mode = BindingMode::OneWayToSource)
		{
			if(!_dataContext)
			{
				throw Exception("Unable to set a binding. Data context is null.");
			}

			auto& target = static_cast<TTarget&>(*this);
			auto& source = _dataContext.GetValueAs<TSource>();

			switch(mode)
			{
				case BindingMode::OneWay:
				{
					targetProperty.InvokeSetter(target, sourceProperty.InvokeGetter(source));
					Binding::Bind(targetProperty, target, sourceProperty, source);
					break;
				}

				case BindingMode::OneWayToSource:
				{
					sourceProperty.InvokeSetter(source, targetProperty.InvokeGetter(target));
					Binding::Bind(sourceProperty, source, targetProperty, target, event);
					break;
				}

				case BindingMode::TwoWay:
				{
					targetProperty.InvokeSetter(target, sourceProperty.InvokeGetter(source));
					Binding::Bind(targetProperty, target, sourceProperty, source);
					Binding::Bind(sourceProperty, source, targetProperty, target, event);
					break;
				}
			}
		}

		template<typename TTarget, typename TSource, typename TValue>
		void Unbind(AbstractProperty<TTarget, TValue>& targetProperty,
					AbstractProperty<TSource, TValue>& sourceProperty,
					BindingMode mode = BindingMode::OneWay)
		{
			if(!_dataContext)
			{
				throw Exception("Unable to remove a binding. Data context is null.");
			}

			auto& target = static_cast<TTarget&>(*this);
			auto& source = _dataContext.GetValueAs<TSource>();

			switch(mode)
			{
				case BindingMode::OneWay:
				{
					Binding::Unbind(targetProperty, target, sourceProperty, source);
					break;
				}

				case BindingMode::OneWayToSource:
				{
					Binding::Unbind(sourceProperty, source, targetProperty, target);
					break;
				}

				case BindingMode::TwoWay:
				{
					Binding::Unbind(targetProperty, target, sourceProperty, source);
					Binding::Unbind(sourceProperty, source, targetProperty, target);
					break;
				}
			}
		}

		template<typename TTarget, typename TSource, typename TValue, CEvent TEvent>
		void Unbind(AbstractProperty<TTarget, TValue>& targetProperty,
					AbstractProperty<TSource, TValue>& sourceProperty,
					TEvent AbstractProperty<TTarget, TValue>::Owner::* event,
					BindingMode mode = BindingMode::OneWayToSource)
		{
			if(!_dataContext)
			{
				throw Exception("Unable to set a binding. Data context is null.");
			}

			auto& target = static_cast<TTarget&>(*this);
			auto& source = _dataContext.GetValueAs<TSource>();

			switch(mode)
			{
				case BindingMode::OneWay:
				{
					Binding::Unbind(targetProperty, target, sourceProperty, source);
					break;
				}

				case BindingMode::OneWayToSource:
				{
					Binding::Unbind(sourceProperty, source, targetProperty, target, event);
					break;
				}

				case BindingMode::TwoWay:
				{
					Binding::Unbind(targetProperty, target, sourceProperty, source);
					Binding::Unbind(sourceProperty, source, targetProperty, target, event);
					break;
				}
			}
		}
	protected:
		virtual void NotifyPropertyChanged(AbstractPropertyBase& property);

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
}