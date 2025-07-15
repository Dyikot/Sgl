#include "Playlist.h"

namespace Sgl
{
	Playlist::Playlist(const Playlist& other):
		Volume(other.Volume),
		Items(other.Items),
		_currentIt(other._currentIt)
	{}

	Playlist::Playlist(Playlist&& other) noexcept:
		Volume(other.Volume),
		Items(std::move(other.Items)),
		_currentIt(std::move(other._currentIt))
	{}

	shared_ptr<Music> Playlist::GetCurrent()
	{
		return _currentIt != Items.end() ? *_currentIt : nullptr;
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

		if(!current->IsPlaying())
		{
			current->Play();
			_currentIt++;
		}
		else if(current->IsPaused())
		{
			current->Resume();
		}
	}

	Playlist& Playlist::operator=(const Playlist& other)
	{
		Volume = other.Volume;
		Items = other.Items;
		_currentIt = other._currentIt;
		return *this;
	}

	Playlist& Playlist::operator=(Playlist&& other) noexcept
	{
		Volume = other.Volume;
		Items = std::move(other.Items);
		_currentIt = std::move(other._currentIt);
		return *this;
	}
}
