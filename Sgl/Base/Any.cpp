#include "Any.h"

namespace Sgl
{
	Any::Any(const Any& other):
		_data(other._data ? other._data->Copy() : nullptr)
	{}

	Any::Any(Any&& other) noexcept:
		_data(std::move(other._data))
	{}

	bool Any::Is(const std::type_info& typeInfo) const
	{
		const auto& type = HasValue() ? _data->Type() : typeid(nullptr);
		return typeInfo == type;
	}

	Any& Any::operator=(const Any& other)
	{
		_data = other._data->Copy();
		return *this;
	}

	Any& Any::operator=(Any&& other) noexcept
	{
		_data = std::move(other._data);
		return *this;
	}

	bool operator==(const Any& left, const Any& right)
	{
		bool leftHasValue = left.HasValue();
		bool rightHasValue = right.HasValue();

		if(!leftHasValue || !rightHasValue)
		{
			return !leftHasValue && !rightHasValue;
		}

		return *left._data == *right._data;
	}
}