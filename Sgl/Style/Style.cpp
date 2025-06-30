#include "Style.h"

namespace Sgl
{
	void StyleCollection::Add(shared_ptr<IStyle> style)
	{
		_items.emplace(style->GetSelector().Class, style);
	}

	shared_ptr<IStyle> StyleCollection::TryFind(const std::string& className)
	{
		if(auto it = _items.find(className); it != _items.end())	
		{
			return it->second;
		}

		return nullptr;
	}
}
