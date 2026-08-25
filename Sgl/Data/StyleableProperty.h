#pragma once

#include <stdint.h>
#include "Property.h"

namespace Sgl
{
	enum class ValueSource : uint8_t
	{
		Default,
		Inheritance,
		Style,
		Local,
		PseudoClass
	};

	class Styleable;
	class IPropertyStateGuard;

	class StyleablePropertyBase : public PropertyBase
	{
	public:
		virtual IPropertyStateGuard* CreateStateGuard(Styleable& target) = 0;
	};

	template<typename TOwner, typename TValue>
	class StyleableProperty : public StyleablePropertyBase
	{
	public:
		using Owner = TOwner;
		using Value = TValue;
		using Setter = void(Owner::*)(Value, ValueSource);
		using Getter = TValue(Owner::*)() const;

		StyleableProperty(Setter setter, Getter getter):
			_setter(setter), 
			_getter(getter)
		{}
		StyleableProperty(const StyleableProperty&) = delete;
		StyleableProperty(StyleableProperty&&) = delete;

		void InvokeSetter(Owner& owner, Value value)
		{
			(owner.*_setter)(value, ValueSource::Local);
		}

		void InvokeSetter(Owner& owner, Value value, ValueSource source)
		{
			(owner.*_setter)(value, source);
		}

		Value InvokeGetter(Owner& owner) const
		{
			return (owner.*_getter)();
		}

		IPropertyStateGuard* CreateStateGuard(Styleable& target) final;
	private:
		Setter _setter;
		Getter _getter;
	};

	template<typename TOwner, typename TValue>
	StyleableProperty(void(TOwner::*)(TValue, ValueSource), TValue(TOwner::*)() const) -> StyleableProperty<TOwner, TValue>;
}