#pragma once

#include "../Base/Delegate.h"
#include "../Base/Any.h"

namespace Sgl
{
	using Command = Action<const Any&>;
}