#pragma once
#include <atomic>
#include <memory>
#include <utility>
#include <typeinfo>
#include <type_traits>

namespace Sgl
{
	//! @brief Base class for all objects managed by intrusive Ref<T>
	class RefCounted
	{
	public:
		RefCounted() = default;
		RefCounted(const RefCounted&) = delete;
		RefCounted(RefCounted&&) = delete;
		RefCounted& operator=(const RefCounted&) = delete;
		RefCounted& operator=(RefCounted&&) = delete;

	protected:
		virtual ~RefCounted() = default;

		//! @brief Increments the reference count
		void AddRef() noexcept
		{
			_references.fetch_add(1, std::memory_order_relaxed);
		}

		//! @brief Decrements the reference count. If it hits zero, the object deletes itself.
		void Release() noexcept
		{
			if(_references.fetch_sub(1, std::memory_order_acq_rel) == 1)
			{
				delete this;
			}
		}

	private:
		std::atomic<uint32_t> _references = 1;

		template<typename T>
		friend class Ref;
	};

	//! @brief A thread-safe, intrusive reference-counted handle for managing shared ownership of an object of type T
	template<typename T>
	class Ref final
	{
	public:
		//! @brief Constructs an empty reference
		Ref() noexcept = default;

		//! @brief Constructs an empty reference explicitly from nullptr
		Ref(std::nullptr_t) noexcept {}

		//! @brief Constructs a reference from a raw pointer
		explicit Ref(T* data) noexcept:
			_data(data)
		{
			AddRef();
		}

		//! @brief Copy-constructs a reference from another Ref instance, incrementing the shared reference count
		Ref(const Ref& other) noexcept:
			_data(other._data)
		{
			AddRef();
		}

		//! @brief Copy-constructs a reference from a Ref of a derived type (safe upcasting)
		template<std::derived_from<T> TDerived>
		Ref(const Ref<TDerived>& other) noexcept:
			_data(other._data)
		{
			AddRef();
		}

		//! @brief Move-constructs a reference, transferring ownership without modifying the reference count
		Ref(Ref&& other) noexcept:
			_data(other._data)
		{
			other._data = nullptr;
		}

		//! @brief Move-constructs a reference from a Ref of a derived type (safe upcasting with move)
		template<std::derived_from<T> TDerived>
		Ref(Ref<TDerived>&& other) noexcept:
			_data(other._data)
		{
			other._data = nullptr;
		}

		//! @brief Destroys this reference. If it was the last owner, the object is deleted.
		~Ref()
		{
			static_assert(std::derived_from<T, RefCounted>, "Ref<T> requires T to be derived from RefCounted");
			Release();
		}

		//! @brief Performs a safe downcast to a derived type TDerived, returning a new Ref<TDerived>
		template<std::derived_from<T> TDerived>
		Ref<TDerived> As() const
		{
			if(auto casted = dynamic_cast<TDerived*>(_data))
			{
				return Ref<TDerived>(casted);
			}

			return nullptr;
		}

		//! @brief Checks whether the managed object is of (or derives from) type TDerived
		template<std::derived_from<T> TDerived>
		bool Is() const
		{
			return dynamic_cast<TDerived*>(_data) != nullptr;
		}

		//! @brief Checks whether the managed object is exactly of type U
		template<typename U>
		bool OfType() const
		{
			return _data && typeid(*_data) == typeid(U);
		}

		//! @brief Returns a raw pointer to the managed object, or nullptr if empty
		T* Get() const noexcept
		{
			return _data;
		}

		//! @brief Returns a raw pointer to the managed object cast to type TOther*. No runtime checks are performed.
		template<typename TOther>
		TOther* GetAs() const
		{
			return static_cast<TOther*>(_data);
		}

		//! @brief Returns a reference to the managed object
		T& GetValue() const
		{
			return *_data;
		}

		//! @brief Returns a reference to the managed object cast to type TValue
		template<typename TValue>
		TValue& GetValueAs() const
		{
			return *static_cast<TValue*>(_data);
		}

		//! @brief Releases the managed object and resets the reference to empty
		void Reset() noexcept
		{
			Release();
			_data = nullptr;
		}

		//! @brief Swaps the contents of this reference with another
		//! @param other The reference to swap with
		void swap(Ref& other) noexcept
		{
			std::swap(_data, other._data);
		}

		//! @brief Returns a pointer to the managed object for member access
		T* operator->() const noexcept
		{
			return _data;
		}

		//! @brief Returns a reference to the managed object for dereferencing
		T& operator*() const noexcept
		{
			return *_data;
		}

		//! @brief Assigns nullptr, releasing the currently managed object
		Ref& operator=(std::nullptr_t)
		{
			Release();
			_data = nullptr;
			return *this;
		}

		//! @brief Assigns a raw pointer, releasing the previously managed object
		//! @param data The raw pointer to manage
		Ref& operator=(T* data) noexcept
		{
			Release();
			_data = data;
			AddRef();
			return *this;
		}

		//! @brief Copy-assigns from another Ref instance, incrementing the shared reference count
		//! @param other The reference to copy from
		Ref& operator=(const Ref& other) noexcept
		{
			if(this != &other)
			{
				Release();
				_data = other._data;
				AddRef();
			}

			return *this;
		}

		//! @brief Copy-assigns from a Ref of a derived type (safe upcasting)
		//! @param other The derived reference to copy from
		template<std::derived_from<T> TDerived>
		Ref& operator=(const Ref<TDerived>& other) noexcept
		{
			Release();
			_data = other._data;
			AddRef();
			return *this;
		}

		//! @brief Move-assigns from another Ref instance, transferring ownership
		//! @param other The reference to move from
		Ref& operator=(Ref&& other) noexcept
		{
			if(this != &other)
			{
				Release();
				_data = other._data;
				other._data = nullptr;
			}

			return *this;
		}

		//! @brief Move-assigns from a Ref of a derived type (safe upcasting with move)
		//! @param other The derived reference to move from
		template<std::derived_from<T> TDerived>
		Ref& operator=(Ref<TDerived>&& other) noexcept
		{
			Release();
			_data = other._data;
			other._data = nullptr;
			return *this;
		}

		//! @brief Checks whether the reference manages a non-null object
		//! @return True if the reference is non-empty; otherwise, false
		explicit operator bool() const noexcept
		{
			return _data != nullptr;
		}

		//! @brief Compares two references for equality based on the underlying pointer
		//! @param left The first reference to compare
		//! @param right The second reference to compare
		//! @return True if both references point to the same object; otherwise, false
		friend bool operator==(const Ref& left, const Ref& right) noexcept
		{
			return left._data == right._data;
		}

	private:
		Ref(T* data, std::in_place_t) noexcept:
			_data(data)
		{}

		void AddRef() noexcept
		{
			if(_data)
			{
				_data->AddRef();
			}
		}

		void Release() noexcept
		{
			if(_data)
			{
				_data->Release();
				_data = nullptr;
			}
		}

		template<typename U>
		friend class Ref;

		template<typename U, typename... TArgs>
		friend Ref<U> New(TArgs&&... args);

	private:
		T* _data = nullptr;
	};

	//! @brief Creates a new Ref<T> by constructing an object of type T with the given arguments
	template<typename T, typename... TArgs>
	Ref<T> New(TArgs&&... args)
	{
		return Ref<T>(new T(std::forward<TArgs>(args)...), std::in_place);
	}
}