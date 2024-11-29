#include "Style.h"


namespace Sgl
{
	Style::Style() noexcept:
		_propertyMap() 
	{}

	Style::Style(const Style & style) noexcept:
		_propertyMap()
	{
		Add(style);
	}

	Style::~Style() noexcept
	{
		for(auto& [_, value] : _propertyMap)
		{
			delete value;
		}
	}

	void Style::Add(const Style& style)
	{
		for(auto& [id, property] : style._propertyMap)
		{
			_propertyMap.insert(std::make_pair(id, property->Copy()));
		}
	}

	void Style::Remove(PropertyId id) noexcept
	{
		delete _propertyMap.at(id);
		_propertyMap.erase(id);
	}

	bool Style::Contains(PropertyId id) const noexcept
	{
		return _propertyMap.contains(id);
	}

	void Style::Clear() noexcept
	{
		_propertyMap.clear();
	}

	size_t Style::Count() const noexcept
	{
		return _propertyMap.size();
	}

	IProperty* Style::operator[](PropertyId id)
	{
		return _propertyMap[id];
	}

	bool Style::IsTypeCorrect(PropertyId id, IProperty* property) const
	{
		return PropertyManager::GetTypeNameOf(id) == property->Type().name();
	}	
}