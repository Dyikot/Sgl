#pragma once

#include <filesystem>
#include "AssetsEmbedded.h"
#include "../../Render/Texture.h"
#include "../Ref.h"

namespace Sgl
{
	//! @brief Interface representing an abstract source of image data
	class IImageSource : public RefCounted
	{
	public:
		//! @brief Creates a GPU texture from this image source using the specified renderer
		//! @param renderer The SDL_Renderer used to create the GPU resource
		//! @return A new Texture object representing the image data
		virtual Texture CreateTexture(SDL_Renderer* renderer) const = 0;
	};

	//! @brief A value-type wrapper for managing reference-counted image sources
	class ImageSource
	{
	public:
		ImageSource() = default;

		//! @brief Constructs an ImageSource that loads data from the specified file path
		//! @param imagePath The filesystem path to the image file
		explicit ImageSource(const std::filesystem::path& imagePath);

		//! @brief Constructs an ImageSource that loads data from a predefined asset identifier
		//! @param asset The AssetId referencing a registered asset
		explicit ImageSource(AssetId asset);

		//! @brief Constructs an ImageSource wrapping an existing IImageSource implementation
		//! @param source A reference-counted pointer to an IImageSource implementation
		explicit ImageSource(Ref<IImageSource> source);

		ImageSource(const ImageSource&) = default;
		ImageSource(ImageSource&&) = default;

		//! @brief Creates a GPU texture from the underlying image source
		//! @param renderer The SDL_Renderer used to create the GPU resource
		//! @return A new Texture object
		Texture CreateTexture(SDL_Renderer* renderer) const;

		ImageSource& operator=(const ImageSource& other);
		ImageSource& operator=(ImageSource&& other) noexcept;
		bool operator==(const ImageSource& other) const;
		explicit operator bool() const noexcept { return static_cast<bool>(_source); }
	private:
		Ref<IImageSource> _source;

		friend class std::hash<ImageSource>;
	};
}

namespace std
{
	template<>
	struct hash<Sgl::ImageSource>
	{
		size_t operator()(const Sgl::ImageSource& source) const
		{
			return hash<void*>()(source._source.Get());
		}
	};
}