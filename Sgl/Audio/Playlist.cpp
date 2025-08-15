#include "Playlist.h"


namespace Sgl
{
	Playlist::Playlist(std::initializer_list<Music> init):
		_items(init),
		_current(_items.begin())
	{}

	Playlist::Playlist(const Playlist& other):
		_items(other._items), 
		_current(_items.begin()) {}

	Playlist::Playlist(Playlist&& other) noexcept:
		_items(std::move(other._items)),
		_current(_items.begin())
	{}

	size_t Playlist::Count() const noexcept
	{
		return _items.size();
	}

	bool Playlist::Contains(const Music& song) const
	{
		return std::ranges::find(_items, song) != _items.end();
	}

	bool Playlist::MoveCurrentNext()
	{
		if(_items.empty() || _current == _items.end())
		{
			return false;
		}

		return ++_current != _items.end();
	}

	void Playlist::MoveCurrentToBegin()
	{
		_current = _items.begin();
	}

	bool Playlist::Empty() const
	{
		return _items.empty();
	}

	Music Playlist::Current() const
	{
		if(_items.empty() || _current == _items.end())
		{
			throw std::runtime_error("Playlist is empty or no current song");
		}

		return *_current;
	}

	Playlist& Playlist::operator=(const Playlist & other)
	{
		if(this != &other)
		{
			_items = other._items;
			_current = _items.begin();
		}

		return *this;
	}

	Playlist& Playlist::operator=(Playlist&& other) noexcept
	{
		if(this != &other)
		{
			_items = std::move(other._items);
			_current = _items.begin();
		}

		return *this;
	}

	const Music& Playlist::operator[](size_t index) const
	{
		return _items[index];
	}
}
