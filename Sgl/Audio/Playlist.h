#pragma once

#include <vector>
#include "Audio.h"
#include "../Data/Ref.h"
#include "../Events/Event.h"

namespace Sgl
{
	class Playlist;

	using PlayListEventHandler = EventHandler<Playlist, EventArgs>;

	class Playlist
	{
	public:
		Volume Volume;
		std::vector<Ref<Music>> Items;
		std::vector<Ref<Music>>::iterator Current = Items.begin();
	public:
		Playlist() = default;
		Playlist(const Playlist& other);
		Playlist(Playlist&& other) noexcept;
		// TODO: Implement start on separate thread
		void Play();

		Event<PlayListEventHandler> Started;
		Event<PlayListEventHandler> Ended;

		Playlist& operator=(const Playlist& other);
		Playlist& operator=(Playlist&& other) noexcept;
	};
}