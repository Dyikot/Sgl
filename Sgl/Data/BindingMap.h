#pragma once

#include "Binding.h"
#include <unordered_map>
#include <memory>

namespace Sgl
{
	class BindingMap: public std::unordered_map<PropertyId, std::shared_ptr<BindingBase>>
	{
	private:
		std::unordered_map<std::string, size_t> _membersMap;
	public:
		template<typename TBinding> requires std::derived_from<TBinding, BindingBase>
		void Add(PropertyId id, TBinding* binding)
		{
			operator[](id) = std::shared_ptr<TBinding>(binding);
		}

		template<typename TBinding> requires std::derived_from<TBinding, BindingBase>
		void Add(PropertyId id, const std::string_view memberName, TBinding* binding)
		{
			Add(id, binding);
			_membersMap[std::string(memberName.data())] = id;
		}

		void UpdateSource(PropertyId id)
		{
			if(auto it = find(id); it != end())
			{
				it->second->TryUpdateSource();
			}
		}

		void UpdateTarget(PropertyId id)
		{
			if(auto it = find(id); it != end())
			{
				it->second->TryUpdateTarget();
			}
		}

		void UpdateTarget(const std::string& memberName)
		{
			UpdateTarget(_membersMap[memberName]);
		}

		void OnTargetUpdate(void* sender, const PropertyChangedEventArgs& e)
		{
			UpdateTarget(e.MemberName);
		}
	};
}