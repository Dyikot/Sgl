#pragma once

#include <memory>
#include <concepts>

namespace Sgl
{
	class Any final
	{
	private:
		class IStorage
		{
		public:
			virtual ~IStorage() = default;

			virtual std::unique_ptr<IStorage> Copy() const = 0;
			virtual const std::type_info& Type() const = 0;

			template<typename T>
			T& Get()
			{
				return static_cast<Storage<T>*>(this)->Value;
			}

			template<typename T>
			const T& Get() const
			{
				return static_cast<const Storage<T>*>(this)->Value;
			}

			virtual bool operator==(const IStorage& other) const = 0;
		};

		template<typename T>
		class Storage : public IStorage
		{
		public:
			T Value;
		public:
			Storage() = default;

			template<typename... TArgs>
			Storage(TArgs&&... args):
				Value(std::forward<TArgs>(args)...)
			{}

			Storage(const Storage& other):
				Value(other.Value)
			{}

			Storage(Storage&& other) noexcept:
				Value(std::move(other.Value))
			{}

			std::unique_ptr<IStorage> Copy() const override
			{
				return std::make_unique<Storage<T>>(Value);
			}

			const std::type_info& Type() const override
			{
				return typeid(T);
			}

			bool operator==(const IStorage& other) const override
			{
				return Type() == other.Type() && Value == other.Get<T>();
			}
		};

		std::unique_ptr<IStorage> _data;
	public:
		template<typename T, typename... TArgs>
		static Any New(TArgs&&... args)
		{
			Any obj;
			obj._data = std::make_unique<Storage<std::decay_t<T>>>(std::forward<TArgs>(args)...);
			return obj;
		}

		Any() = default;

		template<typename T>
		explicit Any(T&& value):
			_data(std::make_unique<Storage<std::decay_t<T>>>(std::forward<T>(value)))
		{}

		Any(const Any& other):
			_data(other._data ? other._data->Copy() : nullptr)
		{}

		Any(Any&& other) noexcept:
			_data(std::move(other._data))
		{}

		template<typename T>
		T& As()
		{
			return _data->Get<T>();
		}

		template<typename T>
		const T& As() const
		{
			return _data->Get<T>();
		}

		template<typename T>
		T* TryAs()
		{
			if(HasValue() && _data->Type() == typeid(T))
			{
				return &As<T>();
			}

			return nullptr;
		}

		template<typename T>
		const T* TryAs() const
		{
			if(HasValue() && _data->Type() == typeid(T))
			{
				return &As<T>();
			}

			return nullptr;
		}

		template<typename T>
		bool Is() const
		{
			const auto& type = HasValue() ? _data->Type() : typeid(nullptr);
			return type == typeid(T);
		}

		bool HasValue() const noexcept
		{
			return _data != nullptr;
		}

		template<typename T>
		Any& operator=(T&& value)
		{
			_data = std::make_unique<Storage<std::decay_t<T>>>(std::forward<T>(value));
			return *this;
		}

		Any& operator=(const Any& other)
		{
			_data = other._data->Copy();
			return *this;
		}

		Any& operator=(Any&& other) noexcept
		{
			_data = std::move(other._data);
			return *this;
		}

		friend bool operator==(const Any& left, const Any& right)
		{
			bool leftHasValue = left.HasValue();
			bool rightHasValue = right.HasValue();

			if(leftHasValue && rightHasValue)
			{
				return *left._data == *right._data;
			}
			
			return !leftHasValue && !rightHasValue;
		}
	};
}