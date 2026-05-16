#include "LanguageInfo.h"

namespace Sgl
{
	static LanguageInfo current("en-US");

	LanguageInfo::LanguageInfo(std::string name):
		_name(std::move(name))
	{}

	void LanguageInfo::SetCurrent(const LanguageInfo& languageInfo)
	{
		if(current != languageInfo)
		{
			current = languageInfo;
			CurrentChanged.Invoke(current);
		}
	}

	const LanguageInfo& LanguageInfo::GetCurrent()
	{
		return current;
	}
}

