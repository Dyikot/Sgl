#pragma once

#include <string_view>
#include <string>
#include <vector>

namespace Sgl
{
	class CSVParser
	{
	public:
		std::string_view FilePath;
		char Delimeter;
	public:
		CSVParser(std::string_view filePath, char delimeter);
		CSVParser(const CSVParser&) = default;
		CSVParser(CSVParser&&) = default;
		~CSVParser() = default;

		bool ParseTo(std::vector<std::string>& headers, std::vector<std::string>& records);

	};
}