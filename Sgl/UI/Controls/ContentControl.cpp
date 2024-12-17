#include "ContentControl.h"

namespace Sgl
{
	void ControlContent::OnRender(RenderContext& renderContext)
	{
		Control::OnRender(renderContext);

		if(ContentVisualizer)
		{
			ContentVisualizer->OnRender(renderContext);
		}
	}
}

