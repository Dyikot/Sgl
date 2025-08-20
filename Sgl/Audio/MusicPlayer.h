#pragma once

#include "IPlaylist.h"
#include "../Base/Event.h"

namespace Sgl
{
	class MusicPlayer
	{
	private:
		bool _isHalt = true;
		Music _current;
		IPlaylist* _playlist = nullptr;
	public:
		static MusicPlayer& Current()
		{
			static MusicPlayer musicPlayer;
			return musicPlayer;
		}

		void SetVolume(Volume value);
		Volume GetVolume();

		void SetPosition(TimeSpan value) const noexcept;
		TimeSpan GetPosition() const noexcept;

		Music GetCurrent() const noexcept;

		void Play(Music music, int loops = 0);
		void Play(IPlaylist& playlist);
		void Pause() noexcept;
		void Resume() noexcept;
		void Rewind() noexcept;
		void Halt() noexcept;
		bool IsPaused() const noexcept;
		bool IsPlaying() const noexcept;
	private:
		MusicPlayer();
		MusicPlayer(const MusicPlayer&) = delete;
		MusicPlayer(MusicPlayer&&) = delete;

		void PlayMusic(Music music, int loops);
		void PlayNext();
		void RemovePlaylist() noexcept;
		friend void OnMusicFinished();
	};
}