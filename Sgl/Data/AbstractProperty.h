#pragma once

namespace Sgl
{
	class AbstractPropertyBase
	{
	public:
		AbstractPropertyBase() = default;
		AbstractPropertyBase(const AbstractPropertyBase&) = delete;
		AbstractPropertyBase(AbstractPropertyBase&&) = delete;
		virtual ~AbstractPropertyBase() = default;

		friend bool operator==(const AbstractPropertyBase& left, const AbstractPropertyBase& right)
		{
			return &left == &right;
		}
	};

	template<typename TOwner, typename TValue>
	class AbstractProperty : public AbstractPropertyBase
	{
	public:
		using Owner = TOwner;
		using Value = TValue;

		virtual void InvokeSetter(Owner& owner, Value value) = 0;
		virtual Value InvokeGetter(Owner& owner) const = 0;
	};
}