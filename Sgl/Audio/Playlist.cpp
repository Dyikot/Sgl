#include "Playlist.h"

namespace Sgl
{
	Playlist::Playlist(const Playlist& other):
		Volume(other.Volume),
		Items(other.Items),
		Current(other.Current)
	{}

	Playlist::Playlist(Playlist&& other) noexcept:
		Volume(other.Volume),
		Items(std::move(other.Items)),
		Current(std::move(other.Current))
	{}

	void Playlist::Play()
	{
		if(Items.size() == 0)
		{
			return;
		}

		if(Current == Items.begin())
		{
			Started.TryRaise(*this, EmptyEventArgs);
		}
		else if(Current == Items.end())
		{
			Ended.TryRaise(*this, EmptyEventArgs);
			Current = Items.begin();
		}

		Music& current = *Current;

		if(!current.IsPlaying())
		{
			current.Play();
			Current++;
		}
		else if(current.IsPaused())
		{
			current.Resume();
		}
	}

	Playlist& Playlist::operator=(const Playlist& other)
	{
		Volume = other.Volume;
		Items = other.Items;
		Current = other.Current;
		return *this;
	}

	Playlist& Playlist::operator=(Playlist&& other) noexcept
	{
		Volume = other.Volume;
		Items = std::move(other.Items);
		Current = std::move(other.Current);
		return *this;
	}
}
