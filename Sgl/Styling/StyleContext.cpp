#include "StyleContext.h"
#include "../Application.h"

namespace Sgl
{
	StyleContext::StyleContext(Styleable& target):
		_target(target)
	{
	}

	Color StyleContext::GetColor(const std::string& key) const
	{
		return App->Resources.GetColor(key);
	}

	Brush StyleContext::GetBrush(const std::string& key) const
	{
		return App->Resources.GetBrush(key);
	}
}