#pragma once

#include <concepts>
#include "../Base/Ref.h"
#include "INotityPropertyChanged.h"

namespace Sgl
{
	template<typename T>
	concept CBindable = std::derived_from<T, INotifyPropertyChanged>;

	class Binding
	{
	private:
		template<typename TTarget, typename TSource, typename TValue, typename TSender, typename TEventArgs>
		struct BindingHandler
		{
			AbstractProperty<TTarget, TValue>& TargetProperty;
			TTarget& Target;
			AbstractProperty<TSource, TValue>& SourceProperty;

			void operator()(TSender& sender, TEventArgs e)
			{
				auto& source = static_cast<TSource&>(sender);
				TargetProperty.InvokeSetter(Target, SourceProperty.InvokeGetter(source));
			}

			bool operator==(const BindingHandler& other) const
			{
				return TargetProperty == other.TargetProperty &&
					   SourceProperty == other.SourceProperty;
			}
		};
	public:
		template<typename TTarget, CBindable TSource, typename TValue>
		static void Bind(AbstractProperty<TTarget, TValue>& targetProperty, 
						 AbstractProperty<TTarget, TValue>::Owner& target,
						 AbstractProperty<TSource, TValue>& sourceProperty,
						 AbstractProperty<TSource, TValue>::Owner& source)
		{
			source.AddPropertyChangedEventHandler(
				sourceProperty, 
				BindingHandler<TTarget, TSource, TValue, INotifyPropertyChanged, AbstractPropertyBase&>(targetProperty, target, sourceProperty)
			);
		}

		template<typename TTarget, typename TSource, typename TValue, CEvent TEvent>
		static void Bind(AbstractProperty<TTarget, TValue>& targetProperty,
						 AbstractProperty<TTarget, TValue>::Owner& target,
						 AbstractProperty<TSource, TValue>& sourceProperty,
						 AbstractProperty<TSource, TValue>::Owner& source,
						 TEvent AbstractProperty<TSource, TValue>::Owner::* event)
		{
			using TSender = TEvent::Sender;
			using TEventArgs = TEvent::EventArgs;
			static_assert(std::derived_from<TSource, TSender>);

			std::invoke(event, source) += BindingHandler<TTarget, TSource, TValue, TSender, TEventArgs>(targetProperty, target, sourceProperty);
		}

		template<typename TTarget, CBindable TSource, typename TValue>
		static void Unbind(AbstractProperty<TTarget, TValue>& targetProperty,
						   AbstractProperty<TTarget, TValue>::Owner& target,
						   AbstractProperty<TSource, TValue>& sourceProperty,
						   AbstractProperty<TSource, TValue>::Owner& source)
		{
			source.RemovePropertyChangedEventHandler(
				sourceProperty,
				BindingHandler<TTarget, TSource, TValue, INotifyPropertyChanged, AbstractPropertyBase&>(targetProperty, target, sourceProperty)
			);
		}

		template<typename TTarget, typename TSource, typename TValue, CEvent TEvent>
		static void Unbind(AbstractProperty<TTarget, TValue>& targetProperty,
						   AbstractProperty<TTarget, TValue>::Owner& target,
						   AbstractProperty<TSource, TValue>& sourceProperty,
						   AbstractProperty<TSource, TValue>::Owner& source,
						   TEvent AbstractProperty<TSource, TValue>::Owner::* event)
		{
			using TSender = TEvent::Sender;
			using TEventArgs = TEvent::EventArgs;
			static_assert(std::derived_from<TSource, TSender>);

			std::invoke(event, source) -= BindingHandler<TTarget, TSource, TValue, TSender, TEventArgs>(targetProperty, target, sourceProperty);
		}
	};
}