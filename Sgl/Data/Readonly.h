#pragma once

#include <optional>

namespace Sgl
{
	template<typename T>
	class Readonly final
	{
	private:
		std::optional<T> _value;
	public:
		Readonly() = default;

		Readonly(T&& value) noexcept
		{
			if(!_value.has_value())
			{
				throw std::runtime_error("Readonly object already initialized!");
			}

			_value = std::forward<T>(value);
		}

		const T& Value() const { return _value.value(); }
		const std::optional<T>& TryGetValue() const { return _value; }

		bool IsInitialized() const { return _value.has_value(); }

		const T* operator->() const { return _value.operator->(); }
		const T& operator*() const { return _value.operator*(); }
	};
}