#pragma once

namespace Sgl
{
	/// <summary>
	/// Provides a mechanism for receiving push-based notifications.
	/// </summary>
	template<typename T>
	class IObserver
	{
	public:
		virtual ~IObserver() = default;

		/// <summary>
		/// Provides the observer with new data.
		/// </summary>
		/// <param name="value">- the new data</param>
		virtual void OnNext(T value) = 0;
	};
}