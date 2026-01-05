#include "Image.h"

namespace Sgl::UIElements
{
	Image::Image(const Image& other):
		UIElement(other),
		_sourceBounds(other._sourceBounds),
		_source(other._source),
		_sourceTexture(nullptr),
		_stretch(other._stretch)
	{}

	Image::Image(Image&& other) noexcept:
		UIElement(std::move(other)),
		_sourceBounds(other._sourceBounds),
		_source(std::move(other._source)),
		_sourceTexture(std::move(other._sourceTexture)),
		_stretch(other._stretch)
	{}

	void Image::SetSource(const std::string& value)
	{
		SetSource(value, ValueSource::Local);
	}

	void Image::SetSource(const std::string& value, ValueSource source)
	{
		if(SetProperty(SourceProperty, _source, value, _sourceSource, source))
		{
			InvalidateImageTexture();
			InvalidateArrange();
		}
	}

	void Image::SetStretch(Stretch value)
	{
		SetStretch(value, ValueSource::Local);
	}

	void Image::SetStretch(Stretch value, ValueSource source)
	{
		if(SetProperty(StretchProperty, _stretch, value, _stretchSource, source))
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
			UpdateTexture();
		}

		if(!_sourceTexture)
		{
			return;
		}

		auto sourceWidth = _sourceTexture.GetWidth();
		auto sourceHeight = _sourceTexture.GetHeight();
		float width = _bounds.w;
		float height = _bounds.h;

		switch(_stretch)
		{
			case Stretch::None:
			{
				if(sourceWidth < width)
				{
					width = sourceWidth;
				}

				if(sourceHeight < height)
				{
					height = sourceHeight;
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
					height = sourceHeight * width / sourceWidth;
				}
				else
				{
					width = sourceWidth * height / sourceHeight;
				}

				break;
			}

			case Stretch::UniformToFill:
			{
				if(height > width)
				{
					height = sourceHeight;
					width = _bounds.w * height / _bounds.h;
				}
				else
				{
					width = sourceWidth;
					height = _bounds.h * width / _bounds.w;
				}

				break;
			}
		}		

		if(_stretch == Stretch::UniformToFill)
		{
			_sourceBounds =
			{
				.x = (sourceWidth - width) / 2.f,
				.y = (sourceHeight - height) / 2.f,
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

	void Image::UpdateTexture()
	{
		_sourceTexture = Texture(GetVisualRoot()->GetRenderer(), _source);
		_isImageTextureValid = true;
	}
}
