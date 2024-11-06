#include "Style.h"


namespace Sgl
{
	Style::Style() noexcept:
		_idToPropertyMap() 
	{}

	Style::Style(const Style & style) noexcept:
		_idToPropertyMap()
	{
		for(auto& [id, property] : style._idToPropertyMap)
		{
			_idToPropertyMap.insert(std::make_pair(id, property->Copy()));
		}
	}

	Style::~Style() noexcept
	{
		for(auto& [_, value] : _idToPropertyMap)
		{
			delete value;
		}
	}

	void Style::Add(const Style& style)
	{
		for(auto& [id, property] : style._idToPropertyMap)
		{
			_idToPropertyMap.insert(std::make_pair(id, property->Copy()));
		}
	}

	void Style::Remove(PropertyId id) noexcept
	{
		_idToPropertyMap.erase(id);
	}

	bool Style::Contains(PropertyId id) const noexcept
	{
		return _idToPropertyMap.contains(id);
	}

	void Style::Clear() noexcept
	{
		_idToPropertyMap.clear();
	}

	size_t Style::Count() const noexcept
	{
		return _idToPropertyMap.size();
	}

	IProperty* const Style::operator[](PropertyId id)
	{
		return _idToPropertyMap[id];
	}

	bool Style::IsTypeCorrect(PropertyId id, IProperty* property) const
	{
		return PropertyManager::GetTypeNameBy(id) == property->Type().name();
	}	
}