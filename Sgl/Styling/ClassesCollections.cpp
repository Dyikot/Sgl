#include "ClassesCollections.h"
#include <sstream>

namespace Sgl
{
	void ClassesCollection::Add(std::string className)
	{
		_items.push_back(std::move(className));
	}

	void ClassesCollection::Set(const std::string& classes)
	{
		_items = Parse(classes);
	}

	void ClassesCollection::Remove(const std::string& className)
	{
		std::erase(_items, className);
	}

	size_t ClassesCollection::Count() const noexcept
	{
		return _items.size();
	}

	std::vector<std::string> ClassesCollection::Parse(const std::string& value)
	{
		std::vector<std::string> classes;
		auto stream = std::stringstream(value);
		std::string buffer;

		while(stream >> buffer)
		{
			classes.push_back(buffer);
		}

		return classes;
	}
}

