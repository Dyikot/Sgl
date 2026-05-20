#include "ImageSource.h"

namespace fs = std::filesystem;

namespace Sgl
{
	class PathImageSource : public IImageSource
	{
	public:
		PathImageSource(const fs::path& imagePath): 
			_path(imagePath.lexically_normal().string())
		{}

		Texture CreateTexture(SDL_Renderer* renderer) const final
		{
			return Texture(renderer, _path);
		}
	private:
		std::string _path;
	};

	class AssetImageSource : public IImageSource
	{
	public:
		AssetImageSource(AssetId asset): 
			_asset(asset) 
		{}

		Texture CreateTexture(SDL_Renderer* renderer) const final
		{
			return Texture(renderer, LoadAssetRaw(_asset));
		}
	private:
		AssetId _asset;
	};

	ImageSource::ImageSource(const fs::path& imagePath):
		_source(New<PathImageSource>(imagePath))
	{}

	ImageSource::ImageSource(AssetId asset):
		_source(New<AssetImageSource>(asset))
	{}

	ImageSource::ImageSource(Ref<IImageSource> source):
		_source(std::move(source))
	{}

	Texture ImageSource::CreateTexture(SDL_Renderer* renderer) const
	{
		return _source->CreateTexture(renderer);
	}

	ImageSource& ImageSource::operator=(const ImageSource& other)
	{
		_source = other._source;
		return *this;
	}

	ImageSource& ImageSource::operator=(ImageSource&& other) noexcept
	{
		_source = std::move(other._source);
		return *this;
	}

	bool ImageSource::operator==(const ImageSource& other) const
	{
		return _source == other._source;
	}
}