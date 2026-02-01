#pragma once

#include <string>
#include <vector>

namespace Sgl
{
    /// <summary>
    /// A simple parser for reading comma-separated values (CSV) files.
    /// Supports custom delimiters and provides basic parsing of headers and records into string vectors.
    /// </summary>
    class CSVParser
    {
    public:
        /// <summary>
        /// Constructs a CSV parser for the specified file path and delimiter character.
        /// </summary>
        /// <param name="path"> - path to the CSV file to parse.</param>
        /// <param name="delimeter"> - character used to separate fields (e.g., ',' or ';').</param>
        CSVParser(std::string path, char delimeter);

        CSVParser(const CSVParser&) = default;
        CSVParser(CSVParser&&) = default;

        /// <summary>
        /// The file path associated with this parser.
        /// </summary>
        std::string FilePath;

        /// <summary>
        /// The delimiter character used to separate fields in the CSV file.
        /// </summary>
        char Delimeter;

        /// <summary>
        /// Parses the CSV file into two output vectors:
        /// - The first row is treated as column headers.
        /// - All subsequent rows are concatenated into a flat list of field values in row-major order.
        /// Returns true on success, false if the file cannot be opened or read.
        /// </summary>
        /// <param name="headers"> - output vector that will contain the header field names.</param>
        /// <param name="records"> - output vector that will contain all data fields from all rows after the header.</param>
        /// <returns>True if parsing succeeded; otherwise, false.</returns>
        bool ParseTo(std::vector<std::string>& headers, std::vector<std::string>& records);
    };
}