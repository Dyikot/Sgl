#include "Drawing.h"
#include "../Application.h"

namespace Sgl
{
	Drawing::Drawing() noexcept:
		Renderer(Application::Current()->GetWindow()->Renderer) {}
}
