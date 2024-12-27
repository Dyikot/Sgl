#pragma once

#include <functional>
#include "../Style/Property.h"

namespace Sgl
{
	class BindingBase
	{
	public:
		const PropertyId Id;	
	public:
		BindingBase(PropertyId id):
			Id(id)
		{}
		virtual ~BindingBase() = default;
	};

	template<typename TSource>
	class Binding
	{
	public:
		TSource& Source;
	public:
		Binding(TSource& source, PropertyId id):
			Source(source),
			Id(id)
		{}
	};
}