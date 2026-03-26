#pragma once

#include <string_view>

namespace Sgl
{
	/// <summary>
	/// Opens the specified URL in the default web browser.
	/// </summary>
	/// <param name="url"> - the URL to open (must start with http:// or https://).</param>
	/// <returns>True if the URL was opened successfully, false otherwise.</returns>
	bool OpenUrl(std::string_view url);
}