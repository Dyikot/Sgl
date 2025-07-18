#pragma once

#include <vector>
#include "Audio.h"
#include "../Base/Event.h"

namespace Sgl
{
	using std::shared_ptr;

	class Playlist
	{
	public:
		using PlayListEventHandler = EventHandler<Playlist>;
		using MusicCollection = std::vector<shared_ptr<Music>>;
		using Iterator = MusicCollection::iterator;

		Volume Volume;
		MusicCollection Items;
		
		Event<PlayListEventHandler> Started;
		Event<PlayListEventHandler> Ended;
	private:
		Iterator _currentIt = Items.begin();
	public:
		Playlist() = default;
		Playlist(const Playlist& other);
		Playlist(Playlist&& other) noexcept;

		shared_ptr<Music> GetCurrent();		
		void Play();

		Playlist& operator=(const Playlist& other);
		Playlist& operator=(Playlist&& other) noexcept;
	};
}