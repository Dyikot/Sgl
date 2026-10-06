#include "SetterCollection.h"

namespace Sgl
{
	SetterCollection::SetterCollection():
		_data(new Data(1, {}))
	{}

	SetterCollection::SetterCollection(std::initializer_list<ISetter*> setters):
		SetterCollection()
	{
		_data->setters.reserve(setters.size());

		for(auto setter : setters)
		{
			_data->setters.emplace_back(setter);
		}
	}

	SetterCollection::SetterCollection(std::vector<Ref<ISetter>> setters):
		SetterCollection()
	{
		_data->setters = std::move(setters);
	}

	SetterCollection::SetterCollection(const SetterCollection& other):
		_data(other._data)
	{
		if(_data)
		{
			_data->references++;
		}
	}

	SetterCollection::SetterCollection(SetterCollection&& other) noexcept:
		_data(other._data)
	{
		other._data = nullptr;
	}

	SetterCollection::~SetterCollection()
	{
		Release();
	}

	void SetterCollection::Release()
	{
		if(_data && (--_data->references == 0))
		{
			delete _data;
		}
	}

	void SetterCollection::Add(Ref<ISetter> setter)
	{
		_data->setters.push_back(std::move(setter));
	}

	void SetterCollection::Clear()
	{
		_data->setters.clear();
	}

	size_t SetterCollection::Count() const
	{
		return _data->setters.size();
	}

	SetterCollection& SetterCollection::operator=(const SetterCollection& other)
	{
		Release();
		_data = other._data;

		if(_data)
		{
			_data->references++;
		}

		return *this;
	}

	SetterCollection& SetterCollection::operator=(SetterCollection&& other) noexcept
	{
		std::swap(_data, other._data);
		return *this;
	}
}