#include "ImageBrush.h"

namespace Sgl
{
	ImageBrush::ImageBrush(std::string source) noexcept:
		_source(std::move(source))
	{}

	ImageBrush::ImageBrush(const ImageBrush& other) noexcept:
		_source(other._source),
		_texture(other._texture)
	{}

	ImageBrush::ImageBrush(ImageBrush && other) noexcept:
		_source(std::move(other._source)),
		_texture(std::move(other._texture))
	{}

	void ImageBrush::SetSource(const std::string& value)
	{
		_source = value;
		_texture = nullptr;
	}

	const Texture& ImageBrush::GetTexture() const
	{
		return _texture.GetValue();
	}

	const Texture& ImageBrush::GetTexture(SDL_Renderer* renderer) const
	{
		if(!_texture)
		{
			_texture = New<Texture>(renderer, _source);
		}

		return _texture.GetValue();
	}

	ImageBrush& ImageBrush::operator=(const ImageBrush& other)
	{
		_source = other._source;
		_texture = other._texture;
		return *this;
	}

	ImageBrush& ImageBrush::operator=(ImageBrush&& other) noexcept
	{
		_source = std::move(other._source);
		_texture = std::move(other._texture);
		return *this;
	}

	bool operator==(const ImageBrush& left, const ImageBrush& right)
	{
		return left._source == right._source;
	}
}

