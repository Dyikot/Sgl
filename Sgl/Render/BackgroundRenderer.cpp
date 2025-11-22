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

	void WindowBackgroundRenderer::operator()(const ImageBrush& imageBrush)
	{
		const auto& texture = imageBrush.GetTexture(_context.GetRenderer());
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

	void UIElementBackgroundRenderer::operator()(const ImageBrush& imageBrush)
	{
		const auto& texture = imageBrush.GetTexture(_context.GetRenderer());
		_context.DrawTexture(texture, _rect);
	}
}

