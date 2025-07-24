#pragma once

#include "Audio.h"
#include "../Base/Event.h"

namespace Sgl
{
	class Playlist
	{
	public:
		using PlayListEventHandler = EventHandler<Playlist>;
		using Iterator = std::vector<Music>::iterator;

		Event<PlayListEventHandler> Started;
		Event<PlayListEventHandler> Ended;

		std::vector<Music> Items;
	private:
		Iterator _currentIt = Items.begin();
	public:
		Playlist() = default;
		Playlist(const Playlist&) = delete;
		Playlist(Playlist&& other) noexcept;

		Iterator GetCurrent() const;		
		void Play();
		void Pause();

		Playlist& operator=(const Playlist&) = delete;
		Playlist& operator=(Playlist&& other) noexcept;
	};
}