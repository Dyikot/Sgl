#include "LanguageInfo.h"

namespace Sgl
{
	void LanguageManager::SetCurrent(const LanguageInfo& languageInfo)
	{
		if(_current.Name != languageInfo.Name)
		{
			_current = languageInfo;
			CurrentChanged.Invoke(*this);
		}
	}

	const LanguageInfo& LanguageManager::GetCurrent() const
	{
		return _current;
	}
}

