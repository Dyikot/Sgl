#pragma once

namespace Sgl
{
	class PropertyBase
	{
	public:
		PropertyBase() = default;
		PropertyBase(const PropertyBase&) = delete;
		PropertyBase(PropertyBase&&) = delete;
		virtual ~PropertyBase() = default;

		friend bool operator==(const PropertyBase& left, const PropertyBase& right)
		{
			return &left == &right;
		}
	};

	template<typename TOwner, typename TValue>
	class Property : public PropertyBase
	{
	public:
		using Owner = TOwner;
		using Value = TValue;

		virtual void InvokeSetter(Owner& owner, Value value) = 0;
		virtual Value InvokeGetter(Owner& owner) const = 0;
	};
}