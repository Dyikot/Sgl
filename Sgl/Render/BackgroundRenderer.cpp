#include "BackgroundRenderer.h"

namespace Sgl
{
	WindowBackgroundRenderer::WindowBackgroundRenderer(RenderContext _context):
		_context(_context)
	{}

	void WindowBackgroundRenderer::operator()(Color color)
	{
		_context.DrawBackground(color);
	}

	void WindowBackgroundRenderer::operator()(const TextureSource& textureSource)
	{
		const auto& texture = textureSource.CreateTexture(_context).GetValue();
		_context.DrawTexture(texture);		
	}

	UIElementBackgroundRenderer::UIElementBackgroundRenderer(RenderContext _context, FRect rect):
		_context(_context),
		_rect(rect)
	{}

	void UIElementBackgroundRenderer::operator()(Color color)
	{
		if(!color.IsTransparent())
		{
			_context.DrawRectangleFill(_rect, color);
		}
	}

	void UIElementBackgroundRenderer::operator()(const TextureSource& textureSource)
	{
		const auto& texture = textureSource.CreateTexture(_context).GetValue();
		_context.DrawTexture(texture, _rect);
	}
}

