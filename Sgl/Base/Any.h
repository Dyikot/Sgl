#pragma once

#include <utility>
#include <typeinfo>

namespace Sgl
{
	/// <summary>
	/// A type-safe container that can hold a single value of any copyable type.
	/// Enables storing and retrieving heterogeneous values while preserving type information.
	/// Supports safe type checking and casting, and manages the lifetime of the contained object automatically.
	/// </summary>
	class Any final
	{
	private:
		class IStorage
		{
		public:
			virtual ~IStorage() = default;

			virtual IStorage* Copy() const = 0;
			virtual void CopyTo(void* dest) const = 0;
			virtual const std::type_info& Type() const = 0;

			template<typename T>
			T& Get() { return static_cast<Storage<T>*>(this)->Value; }
			template<typename T>
			const T& Get() const { return static_cast<const Storage<T>*>(this)->Value; }

			virtual bool operator==(const IStorage& other) const = 0;
		};

		template<typename T>
		class Storage : public IStorage
		{
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

			T Value;

			IStorage* Copy() const override
			{
				return new Storage<T>(Value);
			}

			void CopyTo(void* dest) const override
			{
				new (dest) Storage<T>(Value);
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
	public:
		/// <summary>
		/// Creates a new Any instance by constructing an object of type T in-place with the given arguments.
		/// The stored type is deduced from T, and the value is copyable and type-safe.
		/// </summary>
		template<typename T, typename... TArgs>
		static Any New(TArgs&&... args)
		{
			using StorageType = Storage<std::decay_t<T>>;

			Any obj;
			if constexpr (CanUseBuffer<std::decay_t<T>>())
			{
				new (obj._buffer) StorageType(std::forward<TArgs>(args)...);
				obj._data = BufferSentinal;
			}
			else
			{
				obj._data = new StorageType(std::forward<TArgs>(args)...);
			}
			return obj;
		}

		/// <summary>
		/// Constructs an empty Any with no contained value.
		/// </summary>
		Any() = default;

		/// <summary>
		/// Copy-constructs an Any, deeply copying the contained value (if any).
		/// </summary>
		Any(const Any& other);

		/// <summary>
		/// Move-constructs an Any, transferring ownership of the contained value.
		/// </summary>
		Any(Any&& other) noexcept;

		/// <summary>
		/// Constructs an Any by copying or moving the given value into storage.
		/// </summary>
		template<typename T>
		explicit Any(T&& value)
		{
			using StorageType = Storage<std::decay_t<T>>;

			if constexpr (CanUseBuffer<std::decay_t<T>>())
			{
				new (_buffer) StorageType(std::forward<T>(value));
				_data = BufferSentinal;
			}
			else
			{
				_data = new StorageType(std::forward<T>(value));
			}
		}

		/// <summary>
		/// Destroys the contained value, if any.
		/// </summary>
		~Any();

		/// <summary>
		/// Retrieves a reference to the stored value as type T.
		/// Behavior is undefined if the stored type is not T—use Is<T>() to check first.
		/// </summary>
		template<typename T>
		T& As()
		{
			return GetStorage()->Get<T>();
		}

		/// <summary>
		/// Retrieves a const reference to the stored value as type T.
		/// Behavior is undefined if the stored type is not T—use Is<T>() to check first.
		/// </summary>
		template<typename T>
		const T& As() const
		{
			return GetStorage()->Get<T>();
		}

		/// <summary>
		/// Checks whether the contained value is of type T.
		/// </summary>
		template<typename T>
		bool Is() const
		{
			return _data ? GetStorage()->Type() == typeid(T) : false;
		}

		/// <summary>
		/// Checks whether the contained value matches the given type_info.
		/// </summary>
		bool Is(const std::type_info& typeInfo) const
		{
			return _data ? GetStorage()->Type() == typeInfo : false;
		}

		/// <summary>
		/// Returns true if this Any holds a value; otherwise, false.
		/// </summary>
		bool HasValue() const noexcept
		{
			return _data != nullptr;
		}

		/// <summary>
		/// Assigns a new value of type T to this Any, replacing any existing value.
		/// The stored type becomes std::decay_t<T>.
		/// </summary>
		template<typename T>
		Any& operator=(T&& value)
		{
			using StorageType = Storage<std::decay_t<T>>;

			Destroy();
			if constexpr (CanUseBuffer<std::decay_t<T>>())
			{
				new (_buffer) StorageType(std::forward<T>(value));
				_data = BufferSentinal;
			}
			else
			{
				_data = new StorageType(std::forward<T>(value));
			}

			return *this;
		}

		/// <summary>
		/// Copy-assigns another Any, deeply copying its contained value.
		/// </summary>
		Any& operator=(const Any& other);

		/// <summary>
		/// Move-assigns another Any, transferring ownership of its contained value.
		/// </summary>
		Any& operator=(Any&& other) noexcept;

		/// <summary>
		/// Compares two Any objects for equality. Returns true only if both are empty,
		/// or if they contain values of the same type and those values are equal (via operator==).
		/// </summary>
		friend bool operator==(const Any& left, const Any& right);
	private:
		bool IsUsingBuffer() const noexcept { return _data == BufferSentinal; }
		void Destroy() noexcept;

		IStorage* GetStorage() noexcept
		{
			return IsUsingBuffer() ? reinterpret_cast<IStorage*>(_buffer) : _data;
		}

		const IStorage* GetStorage() const noexcept
		{
			return IsUsingBuffer() ? reinterpret_cast<const IStorage*>(_buffer) : _data;
		}

		template<typename T>
		static constexpr bool CanUseBuffer()
		{
			return sizeof(Storage<T>) <= BufferSize;
		}
	private:
		static constexpr size_t BufferSize = 24;
		static constexpr IStorage* BufferSentinal = reinterpret_cast<IStorage*>(1);

		IStorage* _data = nullptr;
		alignas(std::max_align_t) uint8_t _buffer[BufferSize] {};
	};
}
