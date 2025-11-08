#pragma once

#include <memory>
#include <tuple>
#include "Delegate.h"

namespace Sgl
{
	template <typename T>
	class Lazy final
	{
	private:
		using DataFactory = Func<std::unique_ptr<T>>;

		mutable std::unique_ptr<T> _data;
		mutable DataFactory _dataFactory;
	public:
		Lazy() requires std::default_initializable<T>:
			_dataFactory([] { return std::make_unique<T>(); })
		{}

		explicit Lazy(DataFactory dataFactory):
			_dataFactory(std::move(dataFactory))
		{}

		Lazy(const Lazy&) = default;
		Lazy(Lazy&&) noexcept = default;

		T& GetValue()
		{
			EnsureCreated();
			return *_data;
		}

		const T& GetValue() const
		{
			EnsureCreated();
			return *_data;
		}

		bool IsValueCreated() const 
		{ 
			return _data.operator bool();
		}

		Lazy& operator=(const Lazy&) = default;
		Lazy& operator=(Lazy&&) noexcept = default;

		T& operator*() 
		{ 
			return GetValue();
		}

		const T& operator*() const 
		{ 
			return GetValue(); 
		}

		T* operator->() 
		{  
			EnsureCreated();
			return _data.Get();
		}

		const T* operator->() const 
		{ 
			EnsureCreated();
			return _data.Get();
		}
	private:
		void EnsureCreated() const
		{
			if(!_data)
			{
				_data = _dataFactory();
				_dataFactory = nullptr;
			}
		}
	};
}