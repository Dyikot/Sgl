#pragma once

#include <vector>
#include "Audio.h"
#include "../Events/Event.h"

namespace Sgl
{
	class Playlist
	{
	public:
		using PlayListEventHandler = EventHandler<Playlist, EventArgs>;

		Volume Volume;
		std::vector<std::shared_ptr<Music>> Items;

		Event<PlayListEventHandler> Started;
		Event<PlayListEventHandler> Ended;
	private:
		std::vector<std::shared_ptr<Music>>::iterator _currentIt = Items.begin();
	public:
		Playlist() = default;
		Playlist(const Playlist& other);
		Playlist(Playlist&& other) noexcept;

		auto GetCurrent() { return _currentIt != Items.end() ? *_currentIt: nullptr; }
		
		void Play();

		Playlist& operator=(const Playlist& other);
		Playlist& operator=(Playlist&& other) noexcept;
	};
}