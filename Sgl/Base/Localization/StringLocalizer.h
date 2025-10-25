#pragma once

#include "IStringLocalizer.h"
#include <vector>
#include <unordered_map>

namespace Sgl
{
    class StringLocalizer : public IStringLocalizer
    {
    private:
        std::vector<std::string> _headers;
        std::vector<std::string> _records;
        size_t _headersCount = 0;
        size_t _cultureIndex = 0;
        std::unordered_map<std::string, size_t> _recordIndex;
    public:
        StringLocalizer(std::vector<std::string> headers, std::vector<std::string> records);
        StringLocalizer(const StringLocalizer&) = default;
        StringLocalizer(StringLocalizer&&) = default;
        ~StringLocalizer() = default;

        void SetCulture(const std::string& culture) override;
        const std::string& GetCulture() const override;

        const std::string& operator()(const std::string& name) const override;
    private:
        size_t GetRecordIndex(const std::string& record) const;
    };
}