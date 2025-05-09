#pragma once

namespace Sgl
{
	template<typename T>
	class Readonly final
	{
	private:
		T* _value = nullptr;
	public:
		Readonly() = default;

		Readonly(T& value) noexcept
		{
			if(_value != nullptr)
			{
				throw std::runtime_error("Readonly object already initialized!");
			}

			_value = &value;
		}

		T& Value() { return *_value; }
		const T& Value() const { return *_value; }

		T* TryGetValue() { return _value; }
		const T* TryGetValue() const { return _value; }

		bool IsInitialized() const { return _value != nullptr; }

		operator T* () { return _value; }
		operator const T* () const { return _value; }

		const T* operator->() const { return _value; }
		const T& operator*() const { return *_value; }

		T* operator->() { return _value; }
		T& operator*() { return *_value; }
	};
}