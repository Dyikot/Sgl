#include "CSVParser.h"

#include <fstream>
#include <format>

#include "../Logger.h"

namespace Sgl
{
	static std::vector<std::string> ParseLine(const std::string& line, char delimeter)
	{
		std::vector<std::string> records;
		std::string record;
		bool inQuotes = false;		

		record.reserve(line.size() / 4);

		for(size_t i = 0; i < line.length(); i++)
		{
			const char current = line[i];

			if(current == '"')
			{
				inQuotes = !inQuotes;
			}
			else if(current == delimeter && !inQuotes)
			{
				records.push_back(std::move(record));
				record.clear();
			}
			else if(current == '\\' && (i + 1) < line.length())
			{
				switch(line[i + 1])
				{
					case 't': record.push_back('\t'); i++; break;
					case 'n': record.push_back('\n'); i++; break;
					case '"': record.push_back('"');  i++; break;
					case '\\': record.push_back('\\'); i++; break;
					default:
						record.push_back(current);
						break;
				}
			}
			else
			{
				record.push_back(current);
			}
		}

		records.push_back(record);
		return records;
	}

	CSVParser::CSVParser(std::string path, char delimeter):
		FilePath(std::move(path)),
		Delimeter(delimeter)
	{}

	bool CSVParser::ParseTo(std::vector<std::string>& headers, std::vector<std::string>& records)
	{
		bool success = false;

		if(auto stream = std::ifstream(FilePath))
		{
			std::string line;

			if(std::getline(stream, line))
			{
				headers = ParseLine(line, Delimeter);
			}

			while(std::getline(stream, line))
			{
				auto rec = ParseLine(line, Delimeter);
				records.insert(records.end(), rec.begin(), rec.end());
			}

			success = true;
		}
		else
		{
			Logger::LogWarning("Unbale to open the file: {}", FilePath);
		}

		return success;
	}
}