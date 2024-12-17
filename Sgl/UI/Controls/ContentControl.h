#pragma once

#include "Control.h"
#include "../../Render/ContentVisualizer.h"

namespace Sgl
{
	class ControlContent: public Control
	{
	public:
		Any Content;
		std::unique_ptr<ContentVisualizer> ContentVisualizer = nullptr;
	public:
		void OnRender(RenderContext& renderContext) override;
	};
}