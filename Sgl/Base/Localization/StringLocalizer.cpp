#include "StringLocalizer.h"
#include "../Tools/CSVParser.h"
#include "../Logger.h"
#include "../Exceptions.h"

namespace Sgl
{
	StringLocalizer::StringLocalizer(std::vector<std::string> headers,
									 std::vector<std::string> records) :
		_headers(std::move(headers)),
		_records(std::move(records)),
		_headersCount(_headers.size())
	{
		if(_headers.size() < 2 || _records.size() < 2)
		{
			throw Exception("Storage is empty");
		}

		CreateRecordIndex();
	}

	StringLocalizer::StringLocalizer(std::string csvFilePath, char delimeter)
	{
		CSVParser csvParser(std::move(csvFilePath), delimeter);

		if(csvParser.ParseTo(_headers, _records))
		{
			Logger::LogError("Unable to parse cvs file");
		}

		_headersCount = _headers.size();

		if(_headers.size() < 2 || _records.size() < 2)
		{
			throw Exception("Storage is empty");
		}

		CreateRecordIndex();
	}

	const std::string& StringLocalizer::operator()(const std::string& name) const
	{
		size_t recordIndex = GetRecordIndex(name);
		return _records[recordIndex * _headersCount + _cultureIndex];
	}

	void StringLocalizer::SetCulture(const std::string& culture)
	{
		for(size_t i = 1; i < _headers.size(); i++)
		{
			if(_headers[i] == culture)
			{
				_cultureIndex = i;
				return;
			}
		}

		throw Exception("The current culture does not exist in the storage");
	}

	const std::string& StringLocalizer::GetCulture() const
	{
		return _headers[_cultureIndex];
	}

	void StringLocalizer::CreateRecordIndex()
	{
		int index = 0;
		for(size_t i = 0; i < _records.size(); i += _headersCount)
		{
			_recordIndex.emplace(_records[i], index++);
		}
	}

	size_t StringLocalizer::GetRecordIndex(const std::string& record) const
	{
		if(auto it = _recordIndex.find(record); it != _recordIndex.end())	
		{
			return it->second;
		}

		throw Exception("The record '{}' does not exist", record);
	}
}


