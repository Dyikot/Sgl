#pragma once

#include <variant>
#include "Delegate.h"

namespace Sgl
{
	/// <summary>
	/// Lazy initialization wrapper that defers construction of a value of type T until it is first accessed.
	/// </summary>
	template <typename T>
	class Lazy final
	{
	public:
		/// <summary>
		/// Constructs a Lazy instance that will default-initialize T when first accessed.
		/// </summary>
		Lazy() requires std::default_initializable<T>:
			_value([] { return T(); })
		{}

		/// <summary>
		/// Constructs a Lazy instance with a custom factory function that produces the value of type T.
		/// The factory is invoked at most once, upon first access.
		/// </summary>
		/// <param name="dataFactory"> - a callable returning T, used to initialize the value on demand.</param>
		explicit Lazy(Func<T> dataFactory):
			_value(std::move(dataFactory))
		{}

		Lazy(const Lazy&) = default;
		Lazy(Lazy&&) noexcept = default;

		/// <summary>
		/// Returns a reference to the underlying value, initializing it if necessary.
		/// </summary>
		T& GetValue() 
		{
			return GetDataValue();
		}

		/// <summary>
		/// Returns a const reference to the underlying value, initializing it if necessary.
		/// </summary>
		const T& GetValue() const 
		{ 
			return GetDataValue();
		}

		/// <summary>
		/// Checks whether the value has already been initialized.
		/// </summary>
		/// <returns>true if the value has been created; otherwise, false.</returns>
		bool IsValueCreated() const noexcept 
		{ 
			return _value.index() == 1;
		}

		Lazy& operator=(const Lazy&) = default;
		Lazy& operator=(Lazy&&) noexcept = default;

		T& operator*() { return GetDataValue(); }
		const T& operator*() const { return GetDataValue(); }

		T* operator->() {  return &GetDataValue(); }
		const T* operator->() const { return &GetDataValue(); }
	private:
		T& GetDataValue() const
		{
			if(_value.index() == 0)
			{
				_value = std::get<Func<T>>(_value)();
			}

			return std::get<T>(_value);
		}
	private:
		mutable std::variant<Func<T>, T> _value;
	};
}