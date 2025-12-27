#pragma once

#include <concepts>
#include "INotityPropertyChanged.h"

namespace Sgl
{
	template<typename T>
	concept CBindable = std::derived_from<T, INotityPropertyChanged>;

	class Binding
	{
	public:
		template<typename TTarget, CBindable TSource, typename TValue>
		static void Bind(SglProperty<TTarget, TValue>& targetProperty, 
						 SglProperty<TTarget, TValue>::Owner& target,
						 SglProperty<TSource, TValue>& sourceProperty,
						 SglProperty<TSource, TValue>::Owner& source)
		{
			auto handler = [&targetProperty, &target]
				(INotityPropertyChanged& sender, SglPropertyBase& property)
			{
				auto& source = static_cast<TSource&>(sender);
				auto& sourceProperty = static_cast<SglProperty<TSource, TValue>&>(property);
				targetProperty.InvokeSetter(target, sourceProperty.InvokeGetter(source));
			};

			source.AddPropertyChangedEventHandler(sourceProperty, handler);
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

			auto handler = [&targetProperty, &target, &sourceProperty]
				(TSender& sender, TEventArgs e)
			{
				auto& source = static_cast<TSource&>(sender);
				targetProperty.InvokeSetter(target, sourceProperty.InvokeGetter(source));
			};

			std::invoke(event, source) += handler;
		}
	};
}