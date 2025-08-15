#pragma once

#include <vector>
#include <ranges>
#include "IPlaylist.h"

namespace Sgl
{
    class Playlist: public IPlaylist
    {
    private:
        std::vector<Music> _items;
        std::vector<Music>::const_iterator _current = _items.begin();
    public:
        Playlist() = default;
        Playlist(std::initializer_list<Music> init);
        Playlist(const Playlist& other);
        Playlist(Playlist&& other) noexcept;
        ~Playlist() = default;

        size_t Count() const noexcept;
        bool Contains(const Music& song) const;
        bool MoveCurrentNext() override;
        void MoveCurrentToBegin() override;
        bool Empty() const override;
        Music Current() const override;

        Playlist& operator=(const Playlist& other);
        Playlist& operator=(Playlist&& other) noexcept;
        const Music& operator[](size_t index) const;
    };
}