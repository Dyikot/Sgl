#include "LocalizationStorage.h"

#include <vector>
#include <unordered_map>

#include "../Tools/CSVParser.h"
#include "../Logger.h"
#include "../Exceptions.h"

namespace Sgl::Localization
{
    class LocalizationStorage : public ILocalizationStorage
    {
    private:
        std::vector<std::string> _headers;
        std::vector<std::string> _records;
        size_t _headersCount = 0;
        size_t _cultureIndex = 0;
        std::unordered_map<std::string, size_t> _keyIndex;
    public:
        LocalizationStorage(std::string csvFilePath, char delimeter = ',')
        {
            CSVParser csvParser(std::move(csvFilePath), delimeter);

            if(!csvParser.ParseTo(_headers, _records))
            {
                Logger::LogError("Unable to parse cvs file");
            }

            _headersCount = _headers.size();

            if(_headers.size() < 2 || _records.size() < 2)
            {
                throw Exception("Storage is empty");
            }

            CreateKeyIndex();
        }
        LocalizationStorage(const LocalizationStorage&) = default;
        LocalizationStorage(LocalizationStorage&&) = default;

        void SetCulture(const std::string& culture) override
        {
            for(size_t i = 1; i < _headers.size(); i++)
            {
                if(_headers[i] == culture)
                {
                    _cultureIndex = i;
                    return;
                }
            }

            throw Exception("The current culture does not exist in the storage");
        }

        const std::string& GetCulture() const override
        {
            return _headers[_cultureIndex];
        }

        const std::string& GetLocalizedString(const std::string& key) const override
        {
            if(auto it = _keyIndex.find(key); it != _keyIndex.end())
            {
                size_t keyIndex = it->second;
                return _records[keyIndex * _headersCount + _cultureIndex];
            }
            else
            {
                return key;
            }
        }
    private:
        void CreateKeyIndex()
        {
            size_t index = 0;
            for(size_t i = 0; i < _records.size(); i += _headersCount)
            {
                _keyIndex.emplace(_records[i], index++);
            }
        }
    };

    static std::unique_ptr<ILocalizationStorage> _storage = nullptr;

    void SetLocalizationStorage(std::unique_ptr<ILocalizationStorage> storage)
    {
        _storage = std::move(_storage);
    }

    void SetLocalizationStorage(std::string csvFilePath, char delimeter)
	{
        _storage = std::make_unique<LocalizationStorage>(std::move(csvFilePath), delimeter);
    }

    const std::unique_ptr<ILocalizationStorage>& GetLocalizationStorage()
    {
        return _storage;
    }
}