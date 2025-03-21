#pragma once

namespace Sgl
{
	template<typename T>
	class Nullable final
	{
	private:
		T* _value = nullptr;
	public:
		Nullable() = default;

		Nullable(nullptr_t):
			_value(nullptr)
		{}

		Nullable(T& value):
			_value(&value)
		{}

		Nullable(T* value):
			_value(value)
		{}

		T& Value() { return *_value; }
		const T& Value() const { return *_value; }

		T* ValueOrNull() { return _value; }
		const T* ValueOrNull() const { return _value; }

		operator bool() const { return _value; }
		operator T* () { return _value; }

		Nullable<T>& operator|=(T& right)
		{
			if(!_value)
			{
				_value = &right;
			}

			return *this;
		}

		Nullable<T>& operator|=(const Nullable<T>& right)
		{
			if(!_value)
			{
				_value = right._value;
			}

			return *this;
		}

		const T* operator->() const { return _value; }
		const T& operator*() const { return *_value; }
		T* operator->() { return _value; }
		T& operator*() { return *_value; }

		static friend Nullable<T> operator|(const Nullable<T>& left, const Nullable<T>& right)
		{ 
			return left ? left : right;
		}

		static friend const T& operator|(const Nullable<T>& left, const T& right)
		{
			return left ? *left : right;
		}

		static friend T operator|(const Nullable<T>& left, T&& right)
		{
			return left ? *left : right;
		}

		static friend T operator|(Nullable<T>&& left, T&& right)
		{
			return left ? *left : right;
		}
	};
}