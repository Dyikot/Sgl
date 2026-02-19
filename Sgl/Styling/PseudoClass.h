#pragma once

#include <bitset>
#include <string_view>
#include <unordered_map>

namespace Sgl 
{
	using PseudoClassId = unsigned long long;
	using PseudoClassesSet = std::bitset<64>;

	class PseudoClassesRegistry
	{
	public:
		static PseudoClassId Register(std::string_view name);
		static PseudoClassId GetByName(std::string_view name);
	private:
		static inline PseudoClassId _nextId = 0;
		static inline std::unordered_map<std::string_view, PseudoClassId> _nameToId;
	};	
}