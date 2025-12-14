#pragma once

#include <string>
#include <vector>

namespace Sgl
{
	class CSVParser
	{
	public:
		std::string FilePath;
		char Delimeter;
	public:
		CSVParser(std::string path, char delimeter);
		CSVParser(const CSVParser&) = default;
		CSVParser(CSVParser&&) = default;

		bool ParseTo(std::vector<std::string>& headers, std::vector<std::string>& records);
	};
}