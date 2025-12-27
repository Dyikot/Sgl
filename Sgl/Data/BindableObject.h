#pragma once

#include <vector>
#include "../Base/Exceptions.h"
#include "../Base/Ref.h"
#include "Binding.h"

namespace Sgl
{
	class BindableObject : public INotityPropertyChanged
	{
	private:
		struct Observer
		{
			std::reference_wrapper<SglPropertyBase> Property;
			PropertyChangedEventHandler Handler;

			bool operator==(const Observer&) const = default;
		};		

		Ref<INotityPropertyChanged> _dataContext;
		std::vector<Observer> _observers;
	public:
		BindableObject() = default;
		BindableObject(const BindableObject& other);
		BindableObject(BindableObject&& other) noexcept;

		void SetDataContext(const Ref<INotityPropertyChanged>& value);
		const Ref<INotityPropertyChanged>& GetDataContext() const { return _dataContext; }

		void AddPropertyChangedEventHandler(SglPropertyBase& property, PropertyChangedEventHandler handler) override;
		void RemovePropertyChangedEventHandler(SglPropertyBase& property, PropertyChangedEventHandler handler) override;

		template<typename TTarget, typename TSource, typename TValue>
		void BindSet(SglProperty<TTarget, TValue>& targetProperty,
					 SglProperty<TSource, TValue>& sourceProperty)
		{
			if(!_dataContext)
			{
				throw Exception("Unable to preform a binding. Data context is null.");
			}

			auto& source = _dataContext.GetValueAs<TSource>();
			BindSet(targetProperty, sourceProperty, source);
		}

		template<typename TTarget, typename TSource, typename TValue>
		void BindSet(SglProperty<TTarget, TValue>& targetProperty,
					 SglProperty<TSource, TValue>& sourceProperty,
					 SglProperty<TSource, TValue>::Owner& source)
		{
			auto& target = static_cast<TTarget&>(*this);
			targetProperty.InvokeSetter(target, sourceProperty.InvokeGetter(source));
			Binding::Bind(targetProperty, target, sourceProperty, source);
		}

		template<typename TTarget, typename TSource, typename TValue>
		void BindGet(SglProperty<TSource, TValue>& sourceProperty,
					 SglProperty<TTarget, TValue>& targetProperty)
		{
			if(!_dataContext)
			{
				throw Exception("Unable to preform a binding. Data context is null.");
			}

			auto& target = _dataContext.GetValueAs<TTarget&>();
			BindGet(sourceProperty, targetProperty, target);
		}

		template<typename TTarget, typename TSource, typename TValue>
		void BindGet(SglProperty<TSource, TValue>& sourceProperty,
					 SglProperty<TTarget, TValue>& targetProperty,
					 SglProperty<TTarget, TValue>::Owner& target)
		{
			auto& source = static_cast<TSource&>(*this);
			targetProperty.InvokeSetter(target, sourceProperty.InvokeGetter(source));
			Binding::Bind(targetProperty, target, sourceProperty, source);
		}

		template<typename TTarget, typename TSource, typename TValue, CEvent TEvent>
		void BindGet(SglProperty<TSource, TValue>& sourceProperty,
					 SglProperty<TTarget, TValue>& targetProperty,
					 TEvent SglProperty<TSource, TValue>::Owner::* event)
		{
			if(!_dataContext)
			{
				throw Exception("Unable to preform a binding. Data context is null.");
			}

			auto& target = _dataContext.GetValueAs<TTarget&>();
			BindGet(sourceProperty, targetProperty, target, event);
		}		

		template<typename TTarget, typename TSource, typename TValue, CEvent TEvent>
		void BindGet(SglProperty<TSource, TValue>& sourceProperty,
					 SglProperty<TTarget, TValue>& targetProperty,					 
					 SglProperty<TTarget, TValue>::Owner& target,
					 TEvent SglProperty<TSource, TValue>::Owner::* event)
		{
			auto& source = static_cast<TSource&>(*this);
			targetProperty.InvokeSetter(target, sourceProperty.InvokeGetter(sourceProperty));
			Binding::Bind(targetProperty, target, sourceProperty, source, event);
		}		

		template<typename TTarget, typename TSource, typename TValue>
		void Bind(SglProperty<TTarget, TValue>& targetProperty,
				  SglProperty<TSource, TValue>& sourceProperty)
		{
			if(!_dataContext)
			{
				throw Exception("Unable to preform a binding. Data context is null.");
			}

			auto& source = _dataContext.GetValueAs<TSource>();
			Bind(targetProperty, sourceProperty, source);
		}

		template<typename TTarget, typename TSource, typename TValue>
		void Bind(SglProperty<TTarget, TValue>& targetProperty,
				  SglProperty<TSource, TValue>& sourceProperty,
				  SglProperty<TSource, TValue>::Owner& source)
		{
			if(!_dataContext)
			{
				throw Exception("Unable to preform a binding. Data context is null.");
			}

			auto& target = static_cast<TTarget&>(*this);
			targetProperty.InvokeSetter(target, sourceProperty.InvokeGetter(source));
			Binding::Bind(targetProperty, target, sourceProperty, source);
			Binding::Bind(sourceProperty, source, targetProperty, target);
		}

		template<typename TTarget, typename TSource, typename TValue, CEvent TEvent>
		void Bind(SglProperty<TTarget, TValue>& targetProperty,
				  SglProperty<TSource, TValue>& sourceProperty,
				  TEvent SglProperty<TTarget, TValue>::Owner::* event)
		{
			if(!_dataContext)
			{
				throw Exception("Unable to preform a binding. Data context is null.");
			}

			auto& source = _dataContext.GetValueAs<TSource>();
			Bind(targetProperty, sourceProperty, source, event);
		}

		template<typename TTarget, typename TSource, typename TValue, CEvent TEvent>
		void Bind(SglProperty<TTarget, TValue>& targetProperty,
				  SglProperty<TSource, TValue>& sourceProperty,
				  SglProperty<TSource, TValue>::Owner& source,
				  TEvent SglProperty<TTarget, TValue>::Owner::* event)
		{
			auto& target = static_cast<TTarget&>(*this);
			targetProperty.InvokeSetter(target, sourceProperty.InvokeGetter(source));
			Binding::Bind(targetProperty, target, sourceProperty, source);
			Binding::Bind(sourceProperty, source, targetProperty, target, event);
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