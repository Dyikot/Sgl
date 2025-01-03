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
		template<typename TData, typename TMember>
			requires std::derived_from<TData, ISupportDataBinding>
		void Add(PropertyId id,
				 TMember& target,
				 ISupportDataBinding& source,
				 TMember TData::* member)
		{
			operator[](id) = std::make_shared<Binding<TMember, TData>>(
				source, target, member, BindingMode::OneWayToSource
			);
		}

		template<typename TData, typename TMember>
			requires std::derived_from<TData, ISupportDataBinding>
		void Add(PropertyId id,
				 TMember& target,
				 ISupportDataBinding& source,
				 TMember TData::* member,
				 std::string_view memberName,
				 BindingMode mode = BindingMode::OneWayToSource)
		{
			operator[](id) = std::make_shared<Binding<TMember, TData>>(
				source, target, member, mode
			);
			_membersMap[std::string(memberName.data())] = id;

			if(mode != BindingMode::OneWayToSource)
			{
				source.GetPropertyChangedEvent() +=
					std::bind_front(&BindingMap::OnTargetUpdate, this);
			}
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