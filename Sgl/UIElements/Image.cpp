#include "Image.h"

namespace Sgl::UIElements
{
	Image::Image(Image&& other) noexcept:
		UIElement(std::move(other)),
		_sourceBounds(other._sourceBounds),
		_source(std::move(other._source)),
		_sourceTexture(std::move(other._sourceTexture)),
		_stretch(other._stretch)
	{}

	void Image::SetSource(const ImageSource& value, ValueSource source)
	{
		if(SetProperty(SourceProperty, _source, value, _sourceSource, source))
		{
			InvalidateImageTexture();
			InvalidateArrange();
		}
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
		UIElement::Render(context);

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

					context.DrawTexture(_sourceTexture, &_sourceBounds, &clip);
					break;
				}

				case Stretch::Fill:
				case Stretch::Uniform:
				{
					context.DrawTexture(_sourceTexture, &_sourceBounds, nullptr);
					break;
				}

				case Stretch::UniformToFill:
				{
					auto bounds = GetBounds();
					context.DrawTexture(_sourceTexture, &bounds, &_sourceBounds);
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
		auto [x, y, w, h] = GetBounds();
		float width = w;
		float height = h;

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
					width = w * height / h;
				}
				else
				{
					width = sourceWidth;
					height = h * width / w;
				}

				break;
			}
		}		

		if(_stretch == Stretch::UniformToFill)
		{
			_sourceBounds =
			{
				.x = (sourceWidth - width) * 0.5f,
				.y = (sourceHeight - height) * 0.5f,
				.w = width,
				.h = height
			};
		}
		else
		{
			_sourceBounds =
			{
				.x = x + (w - width)  * 0.5f,
				.y = y + (h - height) * 0.5f,
				.w = width,
				.h = height
			};
		}	
	}

	void Image::UpdateTexture()
	{
		_sourceTexture = GetVisualRoot()->GetTextureFactory().Create(_source, false);
		_isImageTextureValid = true;
	}
}
