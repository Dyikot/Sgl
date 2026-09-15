#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace Sgl
{
	//! @brief Transparent hash functor supporting heterogeneous lookup for string-like types
	struct StringHash
	{
		using Hash = std::hash<std::string_view>;
		using is_transparent = void;

		std::size_t operator()(const char* str) const { return Hash()(str); }
		std::size_t operator()(std::string_view str) const { return Hash()(str); }
		std::size_t operator()(std::string const& str) const { return Hash()(str); }
	};

	//! @brief Splits a string into a vector of substrings based on a delimiter character
	//! @param str The string view to split
	//! @param delimiter The character used as the delimiter
	//! @return A vector of strings containing the split substrings
	std::vector<std::string> SplitString(std::string_view str, char delimiter);
}