#pragma once

#include <string_view>

namespace Sgl
{
	//! @brief Opens the specified URL in the default web browser
	//! @param url The URL to open (must start with http:// or https://)
	//! @return True if the URL was opened successfully, false otherwise
	bool OpenUrl(std::string_view url);
}