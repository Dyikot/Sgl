#pragma once

#include "../Object.h"

namespace Sgl
{
	class Control;

	class Panel: public Object
	{
	protected:
		std::vector<Control*> _children;
	public:
		Control* MouseOverControl = nullptr;
		std::vector<Control*>* const Children = &_children;
	public:
	};
}