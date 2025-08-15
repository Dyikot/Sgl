#include "MusicPlayer.h"

namespace Sgl
{
	void MusicPlayer::SetVolume(Volume value)
	{
		Mix_VolumeMusic(value.ToMixVolume());
	}

	Volume MusicPlayer::GetVolume()
	{
		return Volume(static_cast<uint8_t>(Mix_VolumeMusic(-1)));
	}

	void MusicPlayer::SetPosition(TimeSpan value) const noexcept
	{
		Mix_SetMusicPosition(value.ToSeconds());
	}

	TimeSpan MusicPlayer::GetPosition() const noexcept
	{
		if(_current)
		{
			return TimeSpan::FromMilliseconds(
				Mix_GetMusicPosition(_current.ToMixMusic()) * 1e3);
		}

		return TimeSpan::Zero;
	}

	Music MusicPlayer::GetCurrent() const noexcept
	{
		return _current;
	}

	void MusicPlayer::Play(Music music, int loops)
	{
		RemovePlaylist();
		PlayMusic(music, loops);
	}

	void MusicPlayer::Play(IPlaylist& playlist)
	{
		if(playlist.Empty())
		{
			return;
		}

		_playlist = &playlist;
		PlayNext();
	}

	void MusicPlayer::Pause() noexcept
	{
		if(!Mix_PausedMusic())
		{
			Mix_PauseMusic();
		}
	}

	void MusicPlayer::Resume() noexcept
	{
		Mix_ResumeMusic();
	}

	void MusicPlayer::Rewind() noexcept
	{
		Mix_RewindMusic();
	}

	void MusicPlayer::Halt() noexcept
	{
		_isHalt = true;
		Mix_HaltMusic();
	}

	bool MusicPlayer::IsPaused() const noexcept
	{
		return Mix_PausedMusic();
	}

	bool MusicPlayer::IsPlaying() const noexcept
	{
		return Mix_PlayingMusic();
	}

	void MusicPlayer::RemovePlaylist() noexcept
	{
		_playlist = nullptr;
	}

	void OnMusicFinished()
	{
		auto& player = MusicPlayer::Current();		
		player._current = nullptr;

		if(player._playlist)
		{
			if(!player._isHalt && player._playlist->MoveCurrentNext())
			{
				player.PlayNext();
			}
			else
			{
				player.RemovePlaylist();
			}
		}
	}

	MusicPlayer::MusicPlayer()
	{
		Mix_HookMusicFinished(OnMusicFinished);
	}

	void MusicPlayer::PlayMusic(Music music, int loops)
	{
		_current = music;
		_isHalt = false;
		Mix_PlayMusic(music.ToMixMusic(), loops);
	}

	void MusicPlayer::PlayNext()
	{
		PlayMusic(_playlist->Current(), 0);
	}	
}