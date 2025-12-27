#include "TextureSource.h"

namespace Sgl
{
	TextureSource::TextureSource(std::string_view filePath) noexcept:
		_texture(filePath)
	{}

	TextureSource::TextureSource(const TextureSource& other) noexcept:
		_texture(other._texture)
	{}

	TextureSource::TextureSource(TextureSource && other) noexcept:
		_texture(std::move(other._texture))
	{}

	bool TextureSource::IsCreated() const noexcept
	{
		return _texture.index() == 0;
	}

	const Ref<Texture>& TextureSource::GetTexture() const
	{
		return std::get<Ref<Texture>>(_texture);
	}

	const Ref<Texture>& TextureSource::CreateTexture(RenderContext context) const
	{
		if(_texture.index() == 1)
		{
			auto filePath = std::get<std::string_view>(_texture);
			_texture = New<Texture>(context.GetRenderer(), filePath);
		}

		return std::get<Ref<Texture>>(_texture);
	}

	TextureSource& TextureSource::operator=(const TextureSource& other)
	{
		_texture = other._texture;
		return *this;
	}

	TextureSource& TextureSource::operator=(TextureSource&& other) noexcept
	{
		_texture = std::move(other._texture);
		return *this;
	}

	bool TextureSource::operator==(const Texture& texture) const
	{
		return IsCreated() && GetTexture().GetValue() == texture;
	}
}

