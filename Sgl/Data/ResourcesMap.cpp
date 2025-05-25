#include "ResourcesMap.h"
#include "../Tools/Log.h"
#include "../Render/Texture.h"

namespace Sgl
{
	void ResourcesMap::AddTexture(std::string_view key, std::string_view path)
	{	
		emplace(key, CreateAny<Texture>(key, _renderer));
	}
}
