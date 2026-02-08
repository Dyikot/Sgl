#include "LanguageInfo.h"

namespace Sgl
{
	static LanguageInfo _languageInfo("en-US");

	LanguageInfo::LanguageInfo(std::string name):
		_name(std::move(name))
	{}

	void LanguageInfo::SetCurrent(const LanguageInfo& languageInfo)
	{
		_languageInfo = languageInfo;
		CurrentChanged(_languageInfo);
	}

	const LanguageInfo& LanguageInfo::GetCurrent()
	{
		return _languageInfo;
	}
}

