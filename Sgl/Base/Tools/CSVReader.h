#pragma once

#include <string>
#include <vector>

namespace Sgl
{
	//! @brief A reader for comma-separated values (CSV) files. 
	//! Supports custom delimiters and provides methods to retrieve all records, specific rows, or columns.
	//! Handles quoted fields and escaped quotes within values.
	class CSVReader
	{
	public:
		//! @brief Constructs a CSV reader for the specified file path and delimiter character
		//! @param path The path to the CSV file to parse
		//! @param delimiter The character used to separate fields (e.g., ',' or ';')
		CSVReader(std::string path, char delimiter);

		CSVReader(const CSVReader&) = default;
		CSVReader(CSVReader&&) noexcept = default;

		//! @brief The file path associated with this parser
		const std::string FilePath;

		//! @brief The delimiter character used to separate fields in the CSV file
		const char Delimiter;

		//! @brief Returns all records from the CSV file as a 2D vector of strings
		//! @return A vector of rows, where each row is a vector of field values
		std::vector<std::vector<std::string>> GetRecords() const;

		//! @brief Returns a specific row from the CSV file by index
		//! @param number The zero-based row index to retrieve
		//! @return A vector of field values for the specified row, or an empty vector if the row does not exist
		std::vector<std::string> GetRow(uint32_t number) const;

		//! @brief Returns a specific column from the CSV file by index
		//! @param number The zero-based column index to retrieve
		//! @return A vector of field values for the specified column. Rows with insufficient fields are skipped
		std::vector<std::string> GetColumn(uint32_t number) const;

		//! @brief Returns a specific column from the CSV file by its header name. Assumes the first row contains column headers
		//! @param name The name of the column to retrieve
		//! @return A vector of field values for the specified column (excluding the header row). Returns an empty vector if the column is not found
		std::vector<std::string> GetColumnByName(std::string_view name) const;

	protected:
		std::vector<std::string> ParseLine(const std::string& line, char delimiter) const;
	};
}