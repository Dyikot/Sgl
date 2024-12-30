#include "Audio.h"
#include <Random/Random.h>

namespace Sgl
{
	void PlayList::Shuffle()
	{
		Random().Shuffle(*this);
		_current = begin();
	}
}
