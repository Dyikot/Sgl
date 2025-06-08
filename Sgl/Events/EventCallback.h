#pragma once

#include "../Data/Delegate.h"

namespace Sgl
{
	template<typename T>
	class EventCallback
	{
	private:
		Action<T> _delegate;
	public:
		EventCallback() = default;

		EventCallback(Action<T> delegate):
			_delegate(std::move(delegate))
		{}

		EventCallback(const EventCallback& other):
			_delegate(other._delegate)
		{}

		EventCallback(EventCallback&& other) noexcept:
			_delegate(std::move(other._delegate))
		{}

		bool HasDelegate() const noexcept { return _delegate; }

		void Invoke(const T& value) const
		{
			if(!_delegate)
			{
				throw std::exception("Delegate is empty!");
			}

			_delegate(value);
		}

		void TryInvoke(const T& value) const
		{
			if(_delegate)
			{
				_delegate(value);
			}
		}

		EventCallback& operator=(Action<T> delegate)
		{
			_delegate = std::move(delegate);
			return *this;
		}

		EventCallback& operator=(const EventCallback& other)
		{
			_delegate = other._delegate;
			return *this;
		}

		EventCallback& operator=(EventCallback&& other) noexcept
		{
			_delegate = std::move(other._delegate);
			return *this;
		}
	};

	template<>
	class EventCallback<void>
	{
	private:
		Action<> _delegate;
	public:
		EventCallback() = default;

		EventCallback(Action<> delegate):
			_delegate(std::move(delegate))
		{}

		EventCallback(const EventCallback& other):
			_delegate(other._delegate)
		{}

		EventCallback(EventCallback&& other) noexcept:
			_delegate(std::move(other._delegate))
		{}

		bool HasDelegate() const noexcept { return _delegate; }

		void Invoke() const
		{
			if(!_delegate)
			{
				throw std::exception("Delegate is empty!");
			}

			_delegate();
		}

		void TryInvoke() const
		{
			if(_delegate)
			{
				_delegate();
			}
		}

		EventCallback& operator=(Action<> delegate)
		{
			_delegate = std::move(delegate);
			return *this;
		}

		EventCallback& operator=(const EventCallback& other)
		{
			_delegate = other._delegate;
			return *this;
		}

		EventCallback& operator=(EventCallback&& other) noexcept
		{
			_delegate = std::move(other._delegate);
			return *this;
		}
	};
}