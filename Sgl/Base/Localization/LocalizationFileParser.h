#pragma once

#include <string_view>
#include <string>
#include <stdexcept>
#include <unordered_map>

namespace Sgl
{
    class LocalizationFileParser
    {
    private:
        std::string _source;
        size_t _position;
    public:
        explicit LocalizationFileParser(const std::string& filePath);
        LocalizationFileParser(const LocalizationFileParser&) = delete;
        LocalizationFileParser(LocalizationFileParser&&) = delete;
        ~LocalizationFileParser() = default;

        std::unordered_map<std::string, std::string> Parse();
    private:
        void SkipWhitespace();
        char Peek() const;
        char GetChar();
        void ExpectChar(char expected);
        std::string ParseString();
        std::pair<std::string, std::string> ParsePair();
    };
}