#include "StringLocalizer.h"


namespace Sgl
{
	StringLocalizer::StringLocalizer(std::vector<std::string> headers,
									 std::vector<std::string> records, 
									 const std::string& culture) :
		_headers(std::move(headers)),
		_records(std::move(records)),
		_culture(culture),
		_headersCount(_headers.size())
	{
		int index = 0;
		for(size_t i = 0; i < _records.size(); i += _headersCount)
		{
			_recordIndex.emplace(_records[i], index++);
		}
	}

	const std::string& StringLocalizer::operator()(const std::string& name) const
	{
		if(_headers.size() < 2 || _records.size() < 2)
		{
			throw std::length_error("Storage is empty");
		}

		size_t cultureIndex = GetCurrentCultureIndex();
		size_t recordIndex = GetRecordIndex(name);

		return _records[recordIndex * _headersCount + cultureIndex];
	}

	size_t StringLocalizer::GetCurrentCultureIndex() const
	{
		for(size_t i = 1; i < _headers.size(); i++)
		{
			if(_headers[i] == _culture)
			{
				return i;
			}
		}

		throw std::out_of_range("The current culture does not exist in the storage");
	}

	size_t StringLocalizer::GetRecordIndex(const std::string& record) const
	{
		if(auto it = _recordIndex.find(record); it != _recordIndex.end())	
		{
			return it->second;
		}

		auto message = std::format("The record '{}' does not exist", record);
		throw std::out_of_range(message);
	}
}


