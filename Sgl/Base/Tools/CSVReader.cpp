#include "CSVReader.h"

#include <fstream>
#include <format>

#include "../Logging.h"

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
				if(i + 1 < line.length() && line[i + 1] == '"')
				{
					record.push_back('\"');
					i++;
				}
				else
				{
					inQuotes = !inQuotes;
				}
			}
			else if(current == delimeter && !inQuotes)
			{
				records.push_back(std::move(record));
				record.clear();
			}
			else
			{
				record.push_back(current);
			}
		}

		records.push_back(record);
		return records;
	}

	CSVReader::CSVReader(std::string path, char delimiter):
		FilePath(std::move(path)),
		Delimiter(delimiter)
	{}

	std::vector<std::vector<std::string>> CSVReader::GetRecords() const
	{
		std::vector<std::vector<std::string>> records;

		if(auto stream = std::ifstream(FilePath))
		{
			std::string line;
			while(std::getline(stream, line))
			{
				records.push_back(ParseLine(line, Delimiter));
			}
		}
		else
		{
			Logging::LogWarning("Unable to open a csv file: '{}'.", FilePath);
		}

		return records;
	}

	std::vector<std::string> CSVReader::GetRow(uint32_t number) const
	{
		if(auto stream = std::ifstream(FilePath))
		{
			std::string line;
			uint32_t currentRow = 0;
			while(std::getline(stream, line))
			{
				if(currentRow == number)
				{
					return ParseLine(line, Delimiter);
				}
				currentRow++;
			}
		}
		else
		{
			Logging::LogWarning("Unable to open a csv file: '{}'.", FilePath);
		}

		return {};
	}

	std::vector<std::string> CSVReader::GetColumn(uint32_t number) const
	{
		std::vector<std::string> column;

		if(auto stream = std::ifstream(FilePath))
		{
			std::string line;
			while(std::getline(stream, line))
			{
				auto record = ParseLine(line, Delimiter);
				if(number < record.size())
				{
					column.push_back(record[number]);
				}
			}
		}
		else
		{
			Logging::LogWarning("Unable to open a csv file: '{}'.", FilePath);
		}

		return column;
	}

	std::vector<std::string> CSVReader::GetColumnByName(std::string_view name) const
	{
		if(auto stream = std::ifstream(FilePath))
		{
			std::string line;
			if(std::getline(stream, line))
			{
				auto headers = ParseLine(line, Delimiter);

				for(size_t i = 0; i < headers.size(); i++)
				{
					if(headers[i] == name)
					{
						std::vector<std::string> column;
						while(std::getline(stream, line))
						{
							auto record = ParseLine(line, Delimiter);
							if(i < record.size())
							{
								column.push_back(record[i]);
							}
						}

						return column;
					}
				}
			}
		}
		else
		{
			Logging::LogWarning("Unable to open a csv file: '{}'.", FilePath);
		}

		return {};
	}

	std::unordered_map<std::string, std::string> CSVReader::GetLocalization(std::string_view languageName) const
	{
		std::unordered_map<std::string, std::string> localization;

		if(auto stream = std::ifstream(FilePath))
		{
			std::string line;
			if(std::getline(stream, line))
			{
				auto headers = ParseLine(line, Delimiter);

				int languageIndex = -1;
				for(size_t i = 0; i < headers.size(); i++)
				{
					if(headers[i] == languageName)
					{
						languageIndex = static_cast<int>(i);
						break;
					}
				}

				if(languageIndex >= 0)
				{
					while(std::getline(stream, line))
					{
						auto record = ParseLine(line, Delimiter);
						if(!record.empty() && languageIndex < static_cast<int>(record.size()))
						{
							localization.emplace(record[0], record[languageIndex]);
						}
					}
				}
			}
		}
		else
		{
			Logging::LogWarning("Unable to open a csv file: '{}'.", FilePath);
		}

		return localization;
	}
}