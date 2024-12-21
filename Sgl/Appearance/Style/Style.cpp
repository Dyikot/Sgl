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

	void Style::Add(PropertyId id, Any&& object)
	{
		if(!IsTypeCorrect(id, object))
		{
			throw std::invalid_argument(
				std::format("Property type <{}> is not equal object type <{}>\n",
							PropertyManager::GetTypeNameOf(id),
							object.Type().name()));
		}

		_propertyMap.emplace(id, std::move(object));
	}

	void Style::Add(PropertyId id, const Any& object)
	{
		if(!IsTypeCorrect(id, object))
		{
			throw std::invalid_argument(
				std::format("Property type <{}> is not equal object type <{}>\n",
							PropertyManager::GetTypeNameOf(id),
							object.Type().name()));
		}

		_propertyMap.emplace(id, object);
	}

	void Style::Add(const Style& style)
	{
		for(auto& [id, object] : style._propertyMap)
		{
			_propertyMap.emplace(id, object);
		}
	}

	void Style::Remove(PropertyId id) noexcept
	{
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

	Any& Style::operator[](PropertyId id)
	{
		return _propertyMap[id];
	}

	bool Style::IsTypeCorrect(PropertyId id, const Any& object) const
	{
		return PropertyManager::GetTypeNameOf(id) == object.Type().name();
	}
}