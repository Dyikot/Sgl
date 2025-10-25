#include "CSVParser.h"
#include <fstream>
#include <format>

namespace Sgl
{
	static std::vector<std::string> ParseLine(const std::string& line, char delimeter)
	{
		std::vector<std::string> records;
		std::string record;
		bool inQuotes = false;		

		for(size_t i = 0; i < line.length(); )
		{
			if(line[i] == '"')
			{
				if(inQuotes && i + 1 < line.length() && line[i + 1] == '"')
				{
					record += '"';
					i += 2;
				}
				else
				{
					inQuotes = !inQuotes;
					i++;
				}
			}
			else if(line[i] == delimeter && !inQuotes)
			{
				records.push_back(record);
				record.clear();
				i++;
			}
			else
			{
				record += line[i++];
			}
		}

		records.push_back(record);
		return records;
	}

	CSVParser::CSVParser(std::string_view filePath, char delimeter)
		: FilePath(filePath), Delimeter(delimeter)
	{}

	bool CSVParser::ParseTo(std::vector<std::string>& headers, std::vector<std::string>& records)
	{
		bool success = false;

		if(std::ifstream stream(FilePath.data()); stream.is_open())
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
			auto message = std::format("Cannot open file: {}", FilePath);
			throw std::runtime_error(message);
		}

		return success;
	}
}

