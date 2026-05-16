#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace Sgl
{
	/// <summary>
	/// Transparent hash functor supporting heterogeneous lookup for string-like types.
	/// </summary>
	struct StringHash
	{
		using Hash = std::hash<std::string_view>;
		using is_transparent = void;

		std::size_t operator()(const char* str) const { return Hash()(str); }
		std::size_t operator()(std::string_view str) const { return Hash()(str); }
		std::size_t operator()(std::string const& str) const { return Hash()(str); }
	};

	/// <summary>
	/// Splits a string into a vector of substrings based on a delimiter character.
	/// </summary>
	/// <param name="str"> - the string view to split.</param>
	/// <param name="delimiter"> - the character used as the delimiter.</param>
	/// <returns>A vector of strings containing the split substrings.</returns>
	std::vector<std::string> SplitString(std::string_view str, char delimiter);
}