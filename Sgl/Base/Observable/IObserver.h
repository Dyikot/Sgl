#pragma once

namespace Sgl
{
	template<typename T>
	class IObserver
	{
	public:
		virtual ~IObserver() = default;

		virtual void OnNext(T value) = 0;
	};
}