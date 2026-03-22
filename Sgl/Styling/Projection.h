#pragma once

#include <concepts>
#include "../Base/Delegate.h"

namespace Sgl
{
	class StyleableElement;

	using TargetProjection = Func<StyleableElement&, StyleableElement&>;

	template<typename T>
	concept CTargetProjection = requires (T func, StyleableElement& target)
	{
		{ func(target) } -> std::same_as<StyleableElement&>;
	};

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

	template<typename T1, typename T2>
	inline ComposedProjection<T1, T2> operator>(T1 first, T2 second)
	{
		return ComposedProjection(std::move(first), std::move(second));
	}
}