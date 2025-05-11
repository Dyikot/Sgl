#include "ResourcesMap.h"
#include "../Tools/Log.h"
#include "../Graphic/Texture.h"

namespace Sgl
{
	void ResourcesMap::AddTexture(std::string_view key, std::string_view path)
	{
		auto texture = IMG_LoadTexture(_renderer, path.data());
		PrintSDLErrorIf(texture == nullptr);
		
		if(texture)
		{
			emplace(key, CreateAny<Texture>(texture));
		}
	}
}
