#include "Any.h"
#include <utility>

namespace Sgl
{
	Any::Any(const Any& other):
		_data(nullptr)
	{
		if (other.IsUsingBuffer())
		{
			auto otherStorage = reinterpret_cast<const IStorage*>(other._buffer);
			otherStorage->CopyTo(_buffer);
			_data = BufferSentinal;
		}
		else if (other._data)
		{
			_data = other._data->Copy();
		}
	}

	Any::Any(Any&& other) noexcept:
		_data(other.IsUsingBuffer() ? BufferSentinal : std::exchange(other._data, nullptr))
	{
		if (other.IsUsingBuffer())
		{
			std::memcpy(_buffer, other._buffer, sizeof(_buffer));
			other._data = nullptr;
		}
	}

	Any::~Any()
	{
		Destroy();
	}

	Any& Any::operator=(const Any& other)
	{
		if(this != &other)
		{
			Destroy();

			if (other.IsUsingBuffer())
			{
				auto otherStorage = reinterpret_cast<const IStorage*>(other._buffer);
				otherStorage->CopyTo(_buffer);
				_data = BufferSentinal;
			}
			else
			{
				_data = other._data ? other._data->Copy() : nullptr;
			}
		}

		return *this;
	}

	Any& Any::operator=(Any&& other) noexcept
	{
		if(this != &other)
		{
			Destroy();
			if (other.IsUsingBuffer())
			{
				std::memcpy(_buffer, other._buffer, sizeof(_buffer));
				_data = BufferSentinal;
				other._data = nullptr;
			}
			else
			{
				_data = std::exchange(other._data, nullptr);
			}
		}

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

		return *left.GetStorage() == *right.GetStorage();
	}

	void Any::Destroy() noexcept
	{
		if(IsUsingBuffer())
		{
			reinterpret_cast<IStorage*>(_buffer)->~IStorage();
			_data = nullptr;
		}
		else
		{
			delete _data;
			_data = nullptr;
		}
	}
}
