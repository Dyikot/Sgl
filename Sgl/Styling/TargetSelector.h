#pragma once

#include <concepts>
#include "../Base/Delegate.h"

namespace Sgl
{
	class Styleable;

	//! @brief Function type for projecting a styleable element to a target element
	using TargetSelector = Func<Styleable&, Styleable&>;

	//! @brief Concept that defines the requirements for a target selector function
	template<typename T>
	concept CTargetSelector = requires (T func, Styleable& target)
	{
		{ func(target) } -> std::same_as<Styleable&>;
	};

	//! @brief Composes two selector functions into a single selector
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

	//! @brief Composes two target selectors using the > operator
	//! @param first The first selector to apply
	//! @param second The second selector to apply
	//! @return A composed selector
	template<typename T1, typename T2>
	inline ComposedTargetSelector<T1, T2> operator>(T1 first, T2 second)
	{
		return ComposedTargetSelector(std::move(first), std::move(second));
	}
}