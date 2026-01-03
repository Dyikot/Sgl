#pragma once

#include <concepts>
#include "INotityPropertyChanged.h"
#include "../Base/Ref.h"

namespace Sgl
{
	template<typename T>
	concept CBindable = std::derived_from<T, INotifyPropertyChanged>;

	class Binding
	{
	private:
		template<typename TTarget, typename TSource, typename TValue>
		struct PropertyChangedHandler
		{
			SglProperty<TTarget, TValue>& TargetProperty;
			TTarget& Target;
			SglProperty<TSource, TValue>& SourceProperty;

			void operator()(INotifyPropertyChanged& sender, SglPropertyBase& property)
			{
				auto& source = static_cast<TSource&>(sender);
				TargetProperty.InvokeSetter(Target, SourceProperty.InvokeGetter(source));
			}

			bool operator==(const PropertyChangedHandler& other) const
			{
				return TargetProperty == other.TargetProperty &&
					   SourceProperty == other.SourceProperty;
			}
		};

		template<typename TTarget, typename TSource, typename TValue, typename TSender, typename TEventArgs>
		struct EventRaisedHandler
		{
			SglProperty<TTarget, TValue>& TargetProperty;
			TTarget& Target;
			SglProperty<TSource, TValue>& SourceProperty;

			void operator()(TSender& sender, TEventArgs e)
			{
				auto& source = static_cast<TSource&>(sender);
				TargetProperty.InvokeSetter(Target, SourceProperty.InvokeGetter(source));
			}

			bool operator==(const EventRaisedHandler& other) const
			{
				return TargetProperty == other.TargetProperty &&
					   SourceProperty == other.SourceProperty;
			}
		};
	public:
		template<typename TTarget, CBindable TSource, typename TValue>
		static void Bind(SglProperty<TTarget, TValue>& targetProperty, 
						 SglProperty<TTarget, TValue>::Owner& target,
						 SglProperty<TSource, TValue>& sourceProperty,
						 SglProperty<TSource, TValue>::Owner& source)
		{
			source.AddPropertyChangedEventHandler(
				sourceProperty, 
				PropertyChangedHandler(targetProperty, target, sourceProperty)
			);
		}

		template<typename TTarget, typename TSource, typename TValue, CEvent TEvent>
		static void Bind(SglProperty<TTarget, TValue>& targetProperty,
						 SglProperty<TTarget, TValue>::Owner& target,
						 SglProperty<TSource, TValue>& sourceProperty,
						 SglProperty<TSource, TValue>::Owner& source,
						 TEvent SglProperty<TSource, TValue>::Owner::* event)
		{
			using TSender = TEvent::Sender;
			using TEventArgs = TEvent::EventArgs;
			static_assert(std::derived_from<TSource, TSender>);

			std::invoke(event, source) += EventRaisedHandler<TTarget, TSource, TValue, TSender, TEventArgs>(targetProperty, target, sourceProperty);
		}

		template<typename TTarget, CBindable TSource, typename TValue>
		static void Unbind(SglProperty<TTarget, TValue>& targetProperty,
						   SglProperty<TTarget, TValue>::Owner& target,
						   SglProperty<TSource, TValue>& sourceProperty,
						   SglProperty<TSource, TValue>::Owner& source)
		{
			source.RemovePropertyChangedEventHandler(
				sourceProperty,
				PropertyChangedHandler(targetProperty, target, sourceProperty)
			);
		}

		template<typename TTarget, typename TSource, typename TValue, CEvent TEvent>
		static void Unbind(SglProperty<TTarget, TValue>& targetProperty,
						   SglProperty<TTarget, TValue>::Owner& target,
						   SglProperty<TSource, TValue>& sourceProperty,
						   SglProperty<TSource, TValue>::Owner& source,
						   TEvent SglProperty<TSource, TValue>::Owner::* event)
		{
			using TSender = TEvent::Sender;
			using TEventArgs = TEvent::EventArgs;
			static_assert(std::derived_from<TSource, TSender>);

			std::invoke(event, source) -= EventRaisedHandler<TTarget, TSource, TValue, TSender, TEventArgs>(targetProperty, target, sourceProperty);
		}
	};
}