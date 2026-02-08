#include "LocalizationStorage.h"
#include "../Tools/CSVParser.h"
#include "../Logging.h"
#include "../Exceptions.h"


namespace Sgl
{
    static constexpr int NotFoundLanguage = -1;

	LocalizationStorage::LocalizationStorage(std::string csvFilePath, char delimeter)
	{
        CSVParser csvParser(std::move(csvFilePath), delimeter);

        if(!csvParser.ParseTo(_headers, _records))
        {
            Logging::LogError("Unable to parse cvs file");
        }

        _headersCount = _headers.size();

        if(_headers.size() < 2 || _records.size() < 2)
        {
            throw Exception("Storage is empty");
        }

        CreateKeyIndex();
        
        LanguageInfo::CurrentChanged += MethodEventHandler(&LocalizationStorage::OnLanguageChanged, this);
        SetLanguage(LanguageInfo::GetCurrent());
	}

    LocalizationStorage::~LocalizationStorage()
    {
        LanguageInfo::CurrentChanged -= MethodEventHandler(&LocalizationStorage::OnLanguageChanged, this);
    }

    void LocalizationStorage::SetLanguage(const LanguageInfo& language)
    {
        for(size_t i = 1; i < _headers.size(); i++)
        {
            if(_headers[i] == language.GetName())
            {
                _languageIndex = i;
                return;
            }
        }

        _languageIndex = NotFoundLanguage;
    }

    const std::string& LocalizationStorage::GetLocalizedString(const std::string& key) const
    {
        if(_languageIndex == NotFoundLanguage)
        {
            return key;
        }

        if(auto it = _keyIndex.find(key); it != _keyIndex.end())
        {
            size_t keyIndex = it->second;
            return _records[keyIndex * _headersCount + _languageIndex];
        }
        else
        {
            return key;
        }
    }

    void LocalizationStorage::OnLanguageChanged(LanguageInfo& sender, EventArgs e)
    {
        SetLanguage(sender);
    }

    void LocalizationStorage::CreateKeyIndex()
    {
        size_t index = 0;
        for(size_t i = 0; i < _records.size(); i += _headersCount)
        {
            _keyIndex.emplace(_records[i], index++);
        }
    }
}