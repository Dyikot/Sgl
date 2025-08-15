#include "StyleMap.h"

namespace Sgl
{
	StyleMap::StyleMap(const StyleMap& other):
		_items(other._items)
	{}

	StyleMap::StyleMap(StyleMap&& other) noexcept:
		_items(std::move(other._items))
	{}

	Ref<IStyle> StyleMap::TryGet(std::string_view key) const
	{
		if(auto it = _items.find(key); it != _items.end())
		{
			return it->second;
		}

		return nullptr;
	}

	void StyleMap::Remove(std::string_view key)
	{
		_items.erase(key);
	}

	bool StyleMap::IsEmpty() const
	{
		return _items.empty();
	}

	StyleMap& StyleMap::operator=(const StyleMap& other)
	{
		_items = other._items;
		return *this;
	}

	StyleMap& StyleMap::operator=(StyleMap&& other) noexcept
	{
		_items = std::move(other._items);
		return *this;
	}
}