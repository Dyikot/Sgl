#pragma once

#include "../Data/Delegate.h"

namespace Sgl
{
	class Style;

	class IStylable
	{
	public:
		virtual ~IStylable() = default;

		virtual void ApplyDefaultStyle() = 0;
		virtual const Style& GetStyle() const = 0;
	};

	class Style
	{
	public:
		template<typename T>
		struct SetterWrapper
		{
			T& Target;
			Action<T&> Setter;

			void operator()() const
			{
				Setter(Target);
			}
		};
    private:
		Action<> _source;
		IStylable& _target;
    public:
		Style(IStylable& target):
			_target(target)
		{}

		Style(const Style& other):
			_source(other._source),
			_target(other._target)
		{}

		Style(Style&& other) noexcept:
			_source(std::move(other._source)),
			_target(other._target)
		{}

		template<std::derived_from<IStylable> T, typename... TSetters>
		void Set(TSetters... setters)
		{
			Action<T&> setter = [setters...](T& target)
			{
				(setters(target), ...);
			};

			_source = SetterWrapper<T>(static_cast<T&>(_target), std::move(setter));
		}

		template<std::derived_from<IStylable> T, typename... TSetters>
		void Use(TSetters... setters)
		{
			Set<T, TSetters...>(setters...);
			Apply();
		}

		void Apply() const
		{
			if(_source)
			{
				_source();
			}
		}
	};
}