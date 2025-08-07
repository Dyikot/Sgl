#pragma once

#include "IObserver.h"

namespace Sgl
{
    /// <summary>
    /// Defines a provider for push-based notification.
    /// </summary>
    template<typename T>
    class IObservable
    {
    public:
        virtual ~IObservable() = default;

        /// <summary>
        /// Add an observer to the notifications list.
        /// </summary>
        /// <param name="observer">- the observer to subscribe to notifications</param>
        virtual void Subscribe(IObserver<T>& observer) = 0;

        /// <summary>
        /// Removes an observer from the notification list.
        /// </summary>
        /// <param name="observer">- the observer to unsubscribe from notifications</param>
        virtual void Unsubscribe(IObserver<T>& observer) = 0;
    };
}