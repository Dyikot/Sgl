#pragma once

#include "Control.h"


namespace Sgl
{
	class ContentVisualizer: public IVisual
	{
	public:
		const Any& Content;
	public:
		ContentVisualizer(const Any& content):
			Content(content)
		{}
	};

	class StringVisualizer: public ContentVisualizer
	{
	public:
		StringVisualizer(const Any& content):
			ContentVisualizer(content)
		{
			if(!content.HasValue() || (content.HasValue() && !content.Is<std::string>()))
			{
				throw std::invalid_argument("Content type is not std::string!\n");
			}
		}

		void OnRender(RenderContext& renderContext) override
		{
			const auto& string = Content.As<std::string>();

		}
	};

	class ControlContent: public Control
	{
	public:
		Any Content;
	private:
		std::unique_ptr<ContentVisualizer> _contentVisualizer;
	public:
		void OnRender(RenderContext& renderContext) override;

		template<typename T> requires std::derived_from<T, ContentVisualizer>
		void SetContentVisualizer()
		{
			_contentVisualizer = std::make_unique<T>(Content);
		}
	};
}