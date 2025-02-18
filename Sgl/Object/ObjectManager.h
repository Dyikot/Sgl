#pragma once

#include <unordered_set>
#include <unordered_map>
#include <string>
#include <typeinfo>
#include <stdexcept>

namespace Sgl
{
	template<typename TId>
	class ObjectManager
	{
	protected:
		static inline std::unordered_map<TId, std::string> _objectTypeMap;
		static inline std::unordered_set<std::string> _objectNames;
	public:
		template<typename T>
		static const TId Register(std::string&& name)
		{
			auto [_, isInserted] = _objectNames.insert(std::move(name));

			if(isInserted)
			{
				TId id = _objectNames.size() - 1;
				_objectTypeMap.emplace(id, typeid(T).name());
				return id;
			}
			else
			{
				throw std::invalid_argument("Property with this name already exist\n");
			}
		}

		static std::string_view GetTypeNameOf(TId id)
		{
			return _objectTypeMap.at(id);
		}
	};
}