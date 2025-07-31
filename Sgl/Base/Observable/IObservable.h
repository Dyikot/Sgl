#pragma once

#include "IObserver.h"

namespace Sgl
{
    template<typename T>
    class IObservable
    {
    public:
        virtual ~IObservable() = default;

        virtual void Subscribe(IObserver<T>& observer) = 0;
        virtual void Unsubscribe(IObserver<T>& observer) = 0;
    };
}