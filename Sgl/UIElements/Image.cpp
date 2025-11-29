#include "Image.h"

namespace Sgl::UIElements
{
	Image::Image(const Image& other):
		UIElement(other),
		_sourceBounds(other._sourceBounds),
		_source(other._source),
		_stretch(other._stretch),
		_sourceTexture(nullptr),
		_isImageTextureValid(other._isImageTextureValid)
	{}

	Image::Image(Image&& other) noexcept:
		UIElement(std::move(other)),
		_sourceBounds(other._sourceBounds),
		_source(std::move(other._source)),
		_stretch(other._stretch),
		_sourceTexture(std::move(other._sourceTexture)),
		_isImageTextureValid(other._isImageTextureValid)
	{}

	void Image::SetSource(const std::string & value)
	{
		if(SetProperty(SourceProperty, _source, value))
		{
			InvalidateImageTexture();
			InvalidateArrange();
		}
	}

	void Image::SetStretch(Stretch value)
	{
		if(SetProperty(StretchProperty, _stretch, value))
		{
			InvalidateArrange();
		}
	}

	void Image::Render(RenderContext context)
	{
		RenderBackground(context);

		if(_sourceTexture)
		{
			switch(_stretch)
			{
				case Stretch::None:
				{
					FRect clip =
					{
						.x = 0,
						.y = 0,
						.w = _sourceBounds.w,
						.h = _sourceBounds.h
					};

					context.DrawTexture(_sourceTexture, _sourceBounds, clip);
					break;
				}

				case Stretch::Fill:
				case Stretch::Uniform:
				{
					context.DrawTexture(_sourceTexture, _sourceBounds);
					break;
				}

				case Stretch::UniformToFill:
				{
					FRect clip =
					{
						.x = _sourceBounds.x,
						.y = _sourceBounds.y,
						.w = _sourceBounds.w,
						.h = _sourceBounds.h
					};

					context.DrawTexture(_sourceTexture, _bounds, clip);
					break;
				}
			}
		}

		UIElement::Render(context);
	}

	void Image::ArrangeCore(FRect rect)
	{
		UIElement::ArrangeCore(rect);

		if(!_isImageTextureValid)
		{
			UpdateImageTexture();
		}
		
		if(!_sourceTexture)
		{
			return;
		}

		auto source = _sourceTexture.GetSize();
		float width = _bounds.w;
		float height = _bounds.h;

		switch(_stretch)
		{
			case Stretch::None:
			{
				if(source.Width < width)
				{
					width = source.Width;
				}

				if(source.Height < height)
				{
					height = source.Height;
				}

				break;
			}

			case Stretch::Fill:
			{
				break;
			}

			case Stretch::Uniform:
			{
				if(height > width)
				{
					height = source.Height * width / source.Width;
				}
				else
				{
					width = source.Width * height / source.Height;
				}

				break;
			}

			case Stretch::UniformToFill:
			{
				if(height > width)
				{
					height = source.Height;
					width = _bounds.w * height / _bounds.h;
				}
				else
				{
					width = source.Width;
					height = _bounds.h * width / _bounds.w;
				}

				break;
			}
		}		

		if(_stretch == Stretch::UniformToFill)
		{
			_sourceBounds =
			{
				.x = (source.Width - width) / 2.f,
				.y = (source.Height - height) / 2.f,
				.w = width,
				.h = height
			};
		}
		else
		{
			_sourceBounds =
			{
				.x = _bounds.x + (_bounds.w - width) / 2.f,
				.y = _bounds.y + (_bounds.h - height) / 2.f,
				.w = width,
				.h = height
			};
		}	
	}

	void Image::UpdateImageTexture()
	{
		_sourceTexture = Texture(GetVisualRoot()->GetRenderer(), _source);
		_isImageTextureValid = true;
	}
}
