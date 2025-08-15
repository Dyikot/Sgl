#pragma once

#include "Music.h"

namespace Sgl
{
    class IPlaylist
    {
    public:
        virtual ~IPlaylist() = default;

        virtual bool MoveCurrentNext() = 0;
        virtual void MoveCurrentToBegin() = 0;
        virtual bool Empty() const = 0;
        virtual Music Current() const = 0;
    };
}