#pragma once

#include "GenericPropertyManager.h"
#include "Setters.h"

namespace Sgl
{
	using PropertyManager = GenericPropertyManager<PropertyId>;
	using EventManager = GenericPropertyManager<EventId>;
}