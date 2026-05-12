#include "LanguageInfo.h"

namespace Sgl
{
	LanguageInfo _languageInfo("en-US");

	LanguageInfo::LanguageInfo(std::string name):
		_name(std::move(name))
	{}

	void LanguageInfo::SetCurrent(const LanguageInfo& languageInfo)
	{
		if(_languageInfo != languageInfo)
		{
			_languageInfo = languageInfo;
			CurrentChanged.Invoke(_languageInfo);
		}
	}

	const LanguageInfo& LanguageInfo::GetCurrent()
	{
		return _languageInfo;
	}
}

