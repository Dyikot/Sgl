#pragma once

#include <concepts>
#include "../Base/Delegate.h"

namespace Sgl
{
	class Styleable;

	/// <summary>
	/// Function type for projecting a styleable element to a target element.
	/// Used to apply styles to a specific part of a composite element.
	/// </summary>
	using TargetSelector = Func<Styleable&, Styleable&>;

	/// <summary>
	/// Concept that defines the requirements for a target selector function.
	/// Must take a StyleableElement reference and return a StyleableElement reference.
	/// </summary>
	template<typename T>
	concept CTargetSelector = requires (T func, Styleable& target)
	{
		{ func(target) } -> std::same_as<Styleable&>;
	};

	/// <summary>
	/// Composes two selector functions into a single selector.
	/// The first selector is applied, then the second.
	/// </summary>
	template<CTargetSelector T1, CTargetSelector T2>
	class ComposedTargetSelector
	{
	public:
		ComposedTargetSelector(T1 first, T2 second):
			_first(std::move(first)),
			_second(std::move(second))
		{}

		Styleable& operator()(Styleable& target) const
		{
			return _second(_first(target));
		}
	private:
		T1 _first;
		T2 _second;
	};

	/// <summary>
	/// Composes two target selectors using the > operator.
	/// </summary>
	/// <param name="first"> - the first selector to apply.</param>
	/// <param name="second"> - the second selector to apply.</param>
	/// <returns>A composed selector.</returns>
	template<typename T1, typename T2>
	inline ComposedTargetSelector<T1, T2> operator>(T1 first, T2 second)
	{
		return ComposedTargetSelector(std::move(first), std::move(second));
	}
}