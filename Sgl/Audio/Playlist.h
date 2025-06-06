#pragma once

#include <vector>
#include "Audio.h"
#include "../Events/Event.h"

namespace Sgl
{
	class Playlist
	{
	public:
		using MusicRef = std::reference_wrapper<Music>;
		using PlayListEventHandler = EventHandler<Playlist, EventArgs>;

		Volume Volume;
		std::vector<MusicRef> Items;

		Event<PlayListEventHandler> Started;
		Event<PlayListEventHandler> Ended;
	private:
		std::vector<MusicRef>::iterator _current = Items.begin();
	public:
		Playlist() = default;
		Playlist(const Playlist& other);
		Playlist(Playlist&& other) noexcept;

		Music* GetCurrent() { return _current != Items.end() ? &_current->get(): nullptr; }
		
		void Play();

		Playlist& operator=(const Playlist& other);
		Playlist& operator=(Playlist&& other) noexcept;
	};
}