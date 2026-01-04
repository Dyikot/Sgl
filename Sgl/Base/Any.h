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
				if(Type() != other.Type())
				{
					return false;
				}

				if constexpr(std::equality_comparable<T>)
				{
					const auto& otherStorage = static_cast<const Storage<T>&>(other);
					return Value == otherStorage.Value;
				}

				return false;
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
		Any(const Any& other);
		Any(Any&& other) noexcept;

		template<typename T>
		explicit Any(T&& value):
			_data(std::make_unique<Storage<std::decay_t<T>>>(std::forward<T>(value)))
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
		bool Is() const
		{
			return Is(typeid(T));
		}

		bool Is(const std::type_info& typeInfo) const;

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

		Any& operator=(const Any& other);
		Any& operator=(Any&& other) noexcept;
		friend bool operator==(const Any& left, const Any& right);
	};
}