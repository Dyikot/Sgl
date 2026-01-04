#pragma once

#include <vector>
#include "../Base/Exceptions.h"
#include "../Base/Ref.h"
#include "Binding.h"

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
			std::reference_wrapper<SglPropertyBase> Property;
			PropertyChangedEventHandler Handler;

			bool operator==(const Observer& other) const
			{
				return Handler == other.Handler;
			}
		};		

		Ref<INotifyPropertyChanged> _dataContext;
		std::vector<Observer> _observers;
	public:
		BindableObject() = default;
		BindableObject(const BindableObject& other);
		BindableObject(BindableObject&& other) noexcept;

		void SetDataContext(const Ref<INotifyPropertyChanged>& value);
		const Ref<INotifyPropertyChanged>& GetDataContext() const { return _dataContext; }

		void AddPropertyChangedEventHandler(SglPropertyBase& property, PropertyChangedEventHandler handler) override;
		void RemovePropertyChangedEventHandler(SglPropertyBase& property, PropertyChangedEventHandler handler) override;

		template<typename TTarget, typename TSource, typename TValue>
		void Bind(SglProperty<TTarget, TValue>& targetProperty,
				  SglProperty<TSource, TValue>& sourceProperty,
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
		void Bind(SglProperty<TTarget, TValue>& targetProperty,
				  SglProperty<TSource, TValue>& sourceProperty,
				  TEvent SglProperty<TTarget, TValue>::Owner::* event,
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
		void Unbind(SglProperty<TTarget, TValue>& targetProperty,
					SglProperty<TSource, TValue>& sourceProperty,
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
		void Unbind(SglProperty<TTarget, TValue>& targetProperty,
					SglProperty<TSource, TValue>& sourceProperty,
					TEvent SglProperty<TTarget, TValue>::Owner::* event,
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
		virtual void NotifyPropertyChanged(SglPropertyBase& property);

		template<typename TOwner, typename TValue, typename TField>
		bool SetProperty(SglProperty<TOwner, TValue>& property, TField& field,
						 SglProperty<TOwner, TValue>::Value value)
		{
			if(field == value)
			{
				return false;
			}

			field = value;
			NotifyPropertyChanged(property);

			return true;
		}
	public:
		static inline SglProperty DataContextProperty { &SetDataContext, &GetDataContext };
	};
}