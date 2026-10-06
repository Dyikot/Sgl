#include "Selector.h"
#include "Styleable.h"

namespace Sgl
{
    Selector::Selector():
        _data(new Data())
    {}

    Selector::Selector(const Selector & other):
        _data(other._data)
    {
        if(_data)
        {
            _data->references++;
        }
    }

    Selector::Selector(Selector&& other) noexcept:
        _data(other._data)
    {
        other._data = nullptr;
    }

    Selector::~Selector()
    {
        Release();
    }

    Selector& Selector::Name(std::string name)
    {
        auto& _name = _data->name;

        if(!_name)
        {
            _name = new std::string();
        }

        _name->swap(name);

        return *this;
    }

    Selector& Selector::Class(std::string className)
    {
        auto& _classes = _data->classes;

        if(!_classes)
        {
            _classes = new std::vector<std::string>();
        }

        _classes->push_back(std::move(className));
        return *this;
    }

    bool Selector::Match(const Styleable& element) const
    {
        return _data->Match(element);
    }

    Selector& Selector::operator=(const Selector& other)
    {
        Release();
        _data = other._data;
        return *this;
    }

    Selector& Selector::operator=(Selector&& other) noexcept
    {
        std::swap(_data, other._data);
        return *this;
    }

    void Selector::Release()
    {
        if(_data && (--_data->references == 0))
        {
            delete _data;
        }
    }

    Selector::Data::~Data()
    {
        delete name;
        delete classes;
    }

    bool Selector::Data::Match(const Styleable& element) const
    {
        if(typeComparer && !typeComparer(element))
        {
            return false;
        }

        if(name && element.Name != *name)
        {
            return false;
        }

        if(classes)
        {
            auto& targetClasses = element.GetClasses();

            for(auto& className : *classes)
            {
                if(std::ranges::find(targetClasses, className) == targetClasses.end())
                {
                    return false;
                }
            }
        }

        return true;
    }
}

