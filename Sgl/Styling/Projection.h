#pragma once

#include <concepts>
#include "../Base/Delegate.h"

namespace Sgl
{
	class StyleableElement;

	/// <summary>
	/// Function type for projecting a styleable element to a target element.
	/// Used to apply styles to a specific part of a composite element.
	/// </summary>
	using TargetProjection = Func<StyleableElement&, StyleableElement&>;

	/// <summary>
	/// Concept that defines the requirements for a target projection function.
	/// Must take a StyleableElement reference and return a StyleableElement reference.
	/// </summary>
	template<typename T>
	concept CTargetProjection = requires (T func, StyleableElement& target)
	{
		{ func(target) } -> std::same_as<StyleableElement&>;
	};

	/// <summary>
	/// Composes two projection functions into a single projection.
	/// The first projection is applied, then the second.
	/// </summary>
	template<CTargetProjection T1, CTargetProjection T2>
	class ComposedProjection
	{
	public:
		ComposedProjection(T1 first, T2 second):
			_first(std::move(first)),
			_second(std::move(second))
		{}

		StyleableElement& operator()(StyleableElement& target) const
		{
			return _second(_first(target));
		}
	private:
		T1 _first;
		T2 _second;
	};

	/// <summary>
	/// Composes two projections using the > operator.
	/// </summary>
	/// <param name="first"> - the first projection to apply.</param>
	/// <param name="second"> - the second projection to apply.</param>
	/// <returns>A composed projection.</returns>
	template<typename T1, typename T2>
	inline ComposedProjection<T1, T2> operator>(T1 first, T2 second)
	{
		return ComposedProjection(std::move(first), std::move(second));
	}
}