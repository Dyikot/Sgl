#pragma once

#include <concepts>
#include <memory>

namespace Sgl
{
	
	template<typename T>
		requires std::is_object_v<T> && std::copy_constructible<T>
	class Nullable final
	{
	private:
		using CopyFunc = T* (*)(T*);
		using Deleter = std::default_delete<T>;

		T* _data = nullptr;
		CopyFunc _copyFunc = [](T* data) { return new T(*data); };
	public:
		Nullable() = default;

		Nullable(std::nullptr_t): _data(nullptr) {}

		explicit Nullable(T* data): _data(data) {}

		template<std::derived_from<T> TDerived>
		explicit Nullable(TDerived* data): 
			_data(data), 
			_copyFunc([](T* data) -> T* { return new TDerived(static_cast<const TDerived&>(*data)); })
		{}

		Nullable(const Nullable& other): 
			_data(other._data ? other._copyFunc(other._data) : nullptr),
			_copyFunc(other._copyFunc)
		{}

		Nullable(Nullable&& other) noexcept:
			_data(std::exchange(other._data, nullptr)),
			_copyFunc(other._copyFunc)
		{}

		~Nullable() { Deleter()(_data); }

		T* Get() const noexcept 
		{ 
			return _data; 
		}

		template<typename TOther>
		TOther* GetAs() const
		{
			return static_cast<TOther*>(_data);
		}

		template<std::derived_from<T> TOther>
		TOther* TryGetAs() const
		{
			return dynamic_cast<TOther*>(_data);
		}

		T& GetValue() const
		{
			if(_data == nullptr)
			{
				throw std::runtime_error("Data is null");
			}

			return *_data;
		}

		template<typename TValue>
		TValue& GetValueAs() const
		{
			if(_data == nullptr)
			{
				throw std::runtime_error("Data is null");
			}

			return static_cast<TValue&>(*_data);
		}

		T* operator->() const noexcept
		{ 
			return _data; 
		}

		T& operator*() const 
		{ 
			return *_data; 
		}

		friend bool operator==(const Nullable& left, const Nullable& right)
		{
			return left._data == right._data;
		}

		explicit operator bool() const noexcept
		{
			return _data != nullptr;
		}

		Nullable& operator=(nullptr_t)
		{
			Deleter()(_data);
			_data = nullptr;

			return *this;
		}

		Nullable& operator=(const Nullable& other)
		{
			if(*this != other)
			{
				Deleter()(_data);
				_data = other._data ? other._copyFunc(other._data) : nullptr;
				_copyFunc = other._copyFunc;
			}

			return *this;
		}

		Nullable& operator=(Nullable&& other) noexcept
		{
			if(*this != other)
			{
				_data = std::exchange(other._data, nullptr);
				_copyFunc = other._copyFunc;
			}

			return *this;
		}
	};
}

namespace std
{
	template<typename T>
	struct hash<Sgl::Nullable<T>>
	{
		size_t operator()(const Sgl::Nullable<T>& nullable) const noexcept
		{
			return hash<T*>()(nullable.Get());
		}
	};
}