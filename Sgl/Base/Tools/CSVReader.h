#pragma once

#include <string>
#include <vector>
#include <unordered_map>

namespace Sgl
{
    /// <summary>
    /// A reader for comma-separated values (CSV) files.
    /// Supports custom delimiters and provides methods to retrieve all records, specific rows, or columns.
    /// Handles quoted fields and escaped quotes within values.
    /// </summary>
    class CSVReader
    {
    public:
        /// <summary>
        /// Constructs a CSV reader for the specified file path and delimiter character.
        /// </summary>
        /// <param name="path">The path to the CSV file to parse.</param>
        /// <param name="delimiter">The character used to separate fields (e.g., ',' or ';').</param>
        CSVReader(std::string path, char delimiter);
        CSVReader(const CSVReader&) = default;
        CSVReader(CSVReader&&) noexcept = default;

        /// <summary>
        /// The file path associated with this parser.
        /// </summary>
        std::string FilePath;

        /// <summary>
        /// The delimiter character used to separate fields in the CSV file.
        /// </summary>
        char Delimiter;

        /// <summary>
        /// Returns all records from the CSV file as a 2D vector of strings.
        /// </summary>
        /// <returns>A vector of rows, where each row is a vector of field values.</returns>
        std::vector<std::vector<std::string>> GetRecords() const;

        /// <summary>
        /// Returns a specific row from the CSV file by index.
        /// </summary>
        /// <param name="number"> - the zero-based row index to retrieve.</param>
        /// <returns>A vector of field values for the specified row, or an empty vector if the row does not exist.</returns>
        std::vector<std::string> GetRow(uint32_t number) const;

        /// <summary>
        /// Returns a specific column from the CSV file by index.
        /// </summary>
        /// <param name="number"> - the zero-based column index to retrieve.</param>
        /// <returns>A vector of field values for the specified column. Rows with insufficient fields are skipped.</returns>
        std::vector<std::string> GetColumn(uint32_t number) const;

        /// <summary>
        /// Returns a specific column from the CSV file by its header name.
        /// Assumes the first row contains column headers.
        /// </summary>
        /// <param name="name"> - the name of the column to retrieve.</param>
        /// <returns>A vector of field values for the specified column (excluding the header row). Returns an empty vector if the column is not found.</returns>
        std::vector<std::string> GetColumnByName(std::string_view name) const;

        /// <summary>
        /// Returns a localization map for the specified language.
        /// Assumes the first row contains column headers and the first column contains localization keys.
        /// </summary>
        /// <param name="languageName"> - the name of the language column to retrieve.</param>
        /// <returns>A map where keys are values from the first column and values are from the specified language column.</returns>
        std::unordered_map<std::string, std::string> GetLocalization(std::string_view languageName) const;
    };
}