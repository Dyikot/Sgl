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
		struct Wrapper
		{
			T& Target;
			Action<T&> Setter;

			void operator()() const
			{
				Setter(Target);
			}
		};
    private:
		Action<> _style;
		IStylable& _target;
    public:
		Style(IStylable& target):
			_target(target)
		{}

		Style(const Style& other):
			_style(other._style),
			_target(other._target)
		{}

		Style(Style&& other) noexcept:
			_style(std::move(other._style)),
			_target(other._target)
		{}

		template<std::derived_from<IStylable> T, typename... TSetters>
		void Set(TSetters... setters)
		{
			Action<T&> setter = [setters...](T& target)
			{
				(setters(target), ...);
			};

			_style = Wrapper<T>(static_cast<T&>(_target), std::move(setter));
		}

		template<std::derived_from<IStylable> T, typename... TSetters>
		void Use(TSetters... setters)
		{
			Set<T, TSetters...>(setters...);
			Apply();
		}

		void Apply() const
		{
			if(_style)
			{
				_style();
			}
		}
	};
}