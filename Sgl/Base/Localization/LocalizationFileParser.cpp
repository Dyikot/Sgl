#include "LocalizationFileParser.h"
#include <filesystem>
#include <fstream>

namespace Sgl
{
    static std::string ReadAllLines(const std::string& filePath)
    {
        auto file = std::ifstream(filePath);
        auto buffer = std::ostringstream();
        buffer << file.rdbuf();
        return buffer.str();
    }

    LocalizationFileParser::LocalizationFileParser(const std::string& filePath)
    {
        if(!std::filesystem::exists(filePath))
        {
            auto message = std::format("File with path: '{}' does not exist", filePath);
            throw std::runtime_error(message);
        }

        _source = ReadAllLines(filePath);
    }

    std::unordered_map<std::string, std::string> LocalizationFileParser::Parse()
    {
        std::unordered_map<std::string, std::string> result;

        SkipWhitespace();
        ExpectChar('{');
        SkipWhitespace();

        // Handle empty object
        if(Peek() == '}')
        {
            GetChar();
            return result;
        }

        // Parse first pair
        auto [key, value] = ParsePair();
        result.emplace(std::move(key), std::move(value));

        // Parse remaining pairs
        while(true)
        {
            SkipWhitespace();
            if(Peek() == '}')
            {
                GetChar();
                break;
            }

            ExpectChar(',');
            auto [nextKey, nextValue] = ParsePair();
            result.emplace(std::move(nextKey), std::move(nextValue));
        }

        return result;
    }

    void LocalizationFileParser::SkipWhitespace()
    {
        while(_position < _source.length() && std::isspace(_source[_position]))
        {
            ++_position;
        }
    }

    char LocalizationFileParser::Peek() const
    {
        return (_position < _source.length()) ? _source[_position] : '\0';
    }

    char LocalizationFileParser::GetChar()
    {
        return (_position < _source.length()) ? _source[_position++] : '\0';
    }

    void LocalizationFileParser::ExpectChar(char expected)
    {
        if(_position >= _source.length() || _source[_position] != expected)
        {
            throw std::runtime_error("Expected '" + std::string(1, expected) + "'");
        }
        ++_position;
    }

    std::string LocalizationFileParser::ParseString()
    {
        ExpectChar('"');

        std::string result;
        while(_position < _source.length() && _source[_position] != '"')
        {
            if(_source[_position] == '\\')
            {
                ++_position;
                if(_position >= _source.length())
                {
                    throw std::runtime_error("Incomplete escape sequence");
                }

                switch(_source[_position])
                {
                    case '"':  result += '"'; break;
                    case '\\': result += '\\'; break;
                    case '/':  result += '/'; break;
                    case 'b':  result += '\b'; break;
                    case 'f':  result += '\f'; break;
                    case 'n':  result += '\n'; break;
                    case 'r':  result += '\r'; break;
                    case 't':  result += '\t'; break;
                    default:   result += _source[_position]; break;
                }
            }
            else
            {
                result += _source[_position];
            }
            ++_position;
        }

        ExpectChar('"');
        return result;
    }

    std::pair<std::string, std::string> LocalizationFileParser::ParsePair()
    {
        SkipWhitespace();

        // Parse key
        std::string key = ParseString();

        SkipWhitespace();
        ExpectChar(':');
        SkipWhitespace();

        // Parse value
        std::string value = ParseString();

        return { std::move(key), std::move(value) };
    }
}
