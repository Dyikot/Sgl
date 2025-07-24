#include "Playlist.h"

namespace Sgl
{
	Playlist::Playlist(Playlist&& other) noexcept:
		Items(std::move(other.Items)),
		_currentIt(std::move(other._currentIt))
	{}

	Playlist::Iterator Playlist::GetCurrent() const
	{
		return _currentIt;
	}

	void Playlist::Play()
	{
		if(Items.size() == 0)
		{
			return;
		}

		if(_currentIt == Items.begin())
		{
			Started.TryInvoke(*this, EmptyEventArgs);
		}
		else if(_currentIt == Items.end())
		{
			Ended.TryInvoke(*this, EmptyEventArgs);
			_currentIt = Items.begin();
		}

		auto& current = *_currentIt;

		if(!current.IsPlaying())
		{
			current.Play();
			_currentIt++;
		}
		else if(current.IsPaused())
		{
			current.Resume();
		}
	}

	void Playlist::Pause()
	{
		if(_currentIt != Items.end())
		{
			_currentIt->Pause();
		}
	}

	Playlist& Playlist::operator=(Playlist&& other) noexcept
	{
		Items = std::move(other.Items);
		_currentIt = std::move(other._currentIt);
		return *this;
	}
}
