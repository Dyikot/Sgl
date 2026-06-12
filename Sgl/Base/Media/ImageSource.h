#pragma once

#include <filesystem>
#include "AssetsEmbedded.h"
#include "../../Render/Texture.h"
#include "../Ref.h"

namespace Sgl
{
	/// <summary>
	/// Interface representing an abstract source of image data.
	/// </summary>
	class IImageSource
	{
	public:
		virtual ~IImageSource() = default;

		/// <summary>
		/// Creates a GPU texture from this image source using the specified renderer.
		/// </summary>
		/// <param name="renderer"> - the SDL_Renderer used to create the GPU resource.</param>
		/// <returns>A new Texture object representing the image data.</returns>
		virtual Texture CreateTexture(SDL_Renderer* renderer) const = 0;
	};

	/// <summary>
	/// A value-type wrapper for managing reference-counted image sources.
	/// </summary>
	class ImageSource
	{
	public:
		/// <summary>
		/// Constructs an ImageSource that loads data from the specified file path.
		/// </summary>
		/// <param name="imagePath"> - the filesystem path to the image file.</param>
		explicit ImageSource(const std::filesystem::path& imagePath);

		/// <summary>
		/// Constructs an ImageSource that loads data from a predefined asset identifier.
		/// </summary>
		/// <param name="asset"> - the AssetId referencing a registered asset.</param>
		explicit ImageSource(AssetId asset);

		/// <summary>
		/// Constructs an ImageSource wrapping an existing IImageSource implementation.
		/// </summary>
		/// <param name="source">- a reference-counted pointer to an IImageSource implementation.</param>
		explicit ImageSource(Ref<IImageSource> source);

		ImageSource(const ImageSource&) = default;
		ImageSource(ImageSource&&) = default;

		/// <summary>
		/// Creates a GPU texture from the underlying image source.
		/// </summary>
		/// <param name="renderer"> - the SDL_Renderer used to create the GPU resource.</param>
		/// <returns>A new Texture object.</returns>
		Texture CreateTexture(SDL_Renderer* renderer) const;

		ImageSource& operator=(const ImageSource& other);
		ImageSource& operator=(ImageSource&& other) noexcept;
		bool operator==(const ImageSource& other) const;
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