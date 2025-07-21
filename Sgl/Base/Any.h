#pragma once

#include <memory>

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
		class Storage: public IStorage
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

		std::unique_ptr<IStorage> _value;
	public:
		template<typename T, typename... TArgs>
		static Any New(TArgs&&... args)
		{
			Any obj;
			obj._value = std::make_unique<Storage<std::decay_t<T>>>(std::forward<TArgs>(args)...);
			return obj;
		}

		Any() = default;

		template<std::copyable T>
		Any(T&& value):
			_value(std::make_unique<Storage<std::decay_t<T>>>(std::forward<T>(value)))
		{}

		Any(const Any& other):
			_value(other._value->Copy())
		{}

		Any(Any&& other) noexcept:
			_value(std::move(other._value))
		{}

		template<typename T>
		T& As()
		{
			return _value->Get<T>();
		}

		template<typename T>
		const T& As() const
		{
			return _value->Get<T>();
		}

		template<typename T>
		bool Is() const
		{
			const auto& type = HasValue() ? _value->Type() : typeid(nullptr);
			return type == typeid(T);
		}

		bool HasValue() const noexcept
		{
			return _value != nullptr;
		}

		Any& operator=(const Any& other)
		{
			_value = other._value->Copy();
			return *this;
		}

		Any& operator=(Any&& other) noexcept
		{
			_value = std::move(other._value);
			return *this;
		}

		friend bool operator==(const Any& left, const Any& right)
		{
			return left.HasValue() && right.HasValue() && *left._value == *right._value;
		}
	};
}