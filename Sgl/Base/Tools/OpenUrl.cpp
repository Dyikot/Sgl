#include "OpenUrl.h"
#include "../Logging.h"
#include <SDL3/SDL_misc.h>

namespace Sgl
{
	bool OpenUrl(std::string_view url)
	{
		if(url.empty())
		{
			Logging::LogWarning("Unable to open URL: URL is empty.");
			return false;
		}

		if(url.length() > 2048)
		{
			Logging::LogWarning("Unable to open URL: URL exceeds maximum length of 2048 characters.");
			return false;
		}

		if(!url.starts_with("https://") && !url.starts_with("http://"))
		{
			Logging::LogWarning("Unable to open URL: '{}'. URL must start with http:// or https://.", url);
			return false;
		}

		return SDL_OpenURL(url.data());
	}
}