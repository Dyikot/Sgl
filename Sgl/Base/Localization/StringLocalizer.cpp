#include "StringLocalizer.h"

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
			throw std::length_error("Storage is empty");
		}

		int index = 0;
		for(size_t i = 0; i < _records.size(); i += _headersCount)
		{
			_recordIndex.emplace(_records[i], index++);
		}
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

		throw std::out_of_range("The current culture does not exist in the storage");
	}

	const std::string& StringLocalizer::GetCulture() const
	{
		return _headers[_cultureIndex];
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


