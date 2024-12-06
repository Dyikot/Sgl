#pragma once

#include "../Object.h"

namespace Sgl
{
	class Control;

	class Panel: public Object
	{
	public:
		Control* MouseOverControl = nullptr;
		std::vector<Control*> Children;
	public:

	};
}