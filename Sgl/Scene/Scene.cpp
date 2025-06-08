#include "Scene.h"

namespace Sgl
{
	void Scene::OnRender(RenderContext rc) const
	{
		if(BackgroundTexture)
		{
			rc.FillBackground(*BackgroundTexture, BackgroundColor);
		}
		else
		{
			rc.FillBackground(BackgroundColor);
		}

		_layout->OnRender(rc);
	}

	void Scene::SetLayout(UI::Layout& layout)
	{
		_layout = &layout;
	}
}