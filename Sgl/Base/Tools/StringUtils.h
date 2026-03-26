#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace Sgl
{
	/// <summary>
	/// Splits a string into a vector of substrings based on a delimiter character.
	/// </summary>
	/// <param name="str"> - the string view to split.</param>
	/// <param name="delimiter"> - the character used as the delimiter.</param>
	/// <returns>A vector of strings containing the split substrings.</returns>
	std::vector<std::string> SplitString(std::string_view str, char delimiter);
}