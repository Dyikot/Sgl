#include "Image.h"

namespace Sgl::UIElements
{
	Image::Image()
	{
		Name = "Image";
	}

	Image::Image(Image&& other) noexcept:
		UIElement(std::move(other)),
		_source(std::move(other._source)),
		_stretch(other._stretch),
		_sourceBounds(other._sourceBounds),
		_sourceTexture(std::move(other._sourceTexture))
	{}

	void Image::SetSource(const std::filesystem::path& imagePath, ValueSource source)
	{
		SetSource(ImageSource(imagePath), source);
	}

	void Image::SetSource(const ImageSource& value, ValueSource source)
	{
		if(SetProperty(SourceProperty, _source, value, _sourceSource, source))
		{
			InvalidateArrange();
			UpdateImageTexture();
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
				case Stretch::UniformToFill:
				{
					context.DrawTexture(_sourceTexture, &_sourceBounds, &_sourceClip);
					break;
				}

				case Stretch::Fill:
				case Stretch::Uniform:
				{
					context.DrawTexture(_sourceTexture, &_sourceBounds, nullptr);
					break;
				}
			}
		}
	}	

	void Image::OnAttachedToLogicalTree()
	{
		UIElement::OnAttachedToLogicalTree();
		UpdateImageTexture();
	}

	void Image::OnDetachedFromLogicalTree()
	{
		UIElement::OnDetachedFromLogicalTree();
		_sourceTexture = nullptr;
	}

	void Image::ArrangeContent(FRect rect)
	{		
		if(!_sourceTexture)
		{
			return;
		}

		float sourceWidth = _sourceTexture.GetWidth();
		float sourceHeight = _sourceTexture.GetHeight();
		auto [x, y, w, h] = rect;

		if(sourceWidth <= 0 || sourceHeight <= 0 || w <= 0 || h <= 0)
		{
			return;
		}

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

				_sourceClip = { 0, 0, width, height };

				break;
			}

			case Stretch::Fill:
			{
				break;
			}

			case Stretch::Uniform:
			{
				if(w * sourceHeight < h * sourceWidth)
				{
					height = w * sourceHeight / sourceWidth;
				}
				else
				{
					width = h * sourceWidth / sourceHeight;
				}

				break;
			}

			case Stretch::UniformToFill:
			{
				if(w * sourceHeight > h * sourceWidth)
				{
					width = sourceWidth;
					height = h * sourceWidth / w;
				}
				else
				{
					height = sourceHeight;
					width = w * sourceHeight / h;
				}

				_sourceClip =
				{
					.x = (sourceWidth - width) * 0.5f,
					.y = (sourceHeight - height) * 0.5f,
					.w = width,
					.h = height
				};

				break;
			}
		}		

		if(_stretch == Stretch::UniformToFill)
		{
			_sourceBounds = rect;
		}
		else
		{
			_sourceBounds =
			{
				.x = x + (w - width) * 0.5f,
				.y = y + (h - height) * 0.5f,
				.w = width,
				.h = height
			};
		}
	}

	void Image::UpdateImageTexture()
	{
		_sourceTexture = IsAttachedToLogicalTree() && _source
			? GetVisualRoot()->GetTextureFactory().Create(_source, false)
			: nullptr;
	}
}
