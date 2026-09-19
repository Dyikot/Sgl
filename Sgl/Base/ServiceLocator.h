#pragma once

#include <unordered_map>
#include <memory>
#include <typeindex>

#include "Exceptions.h"

namespace Sgl
{
    //! @brief Service Locator container
    class ServiceLocator
    {
    public:
        //! @brief Registers a service using a std::shared_ptr
        template<typename TInterface, typename TImpl = TInterface>
        void Add(std::shared_ptr<TImpl> service)
        {
            _services[typeid(TInterface)] = std::static_pointer_cast<TInterface>(std::move(service));
        }

        //! @brief Registers a service using a std::unique_ptr
        template<typename TInterface, typename TImpl = TInterface>
        void Add(std::unique_ptr<TImpl> service)
        {
            _services[typeid(TInterface)] = std::shared_ptr<TInterface>(std::move(service));
        }

        //! @brief Register a default initializable service
        template<typename TInterface, typename TImpl = TInterface>
        void Add() requires std::default_initializable<TImpl>
        {
            Add<TInterface, TImpl>(std::make_shared<TImpl>());
        }

        //! @brief Checks if a service has been registered
        //! @tparam T Service type
        //! @return true if registered, false if not
        template<typename T>
        bool Has() const
        {
            return _services.find(typeid(T)) != _services.end();
        }

        //! @brief Retrieves a service
        //! @tparam T Service type
        //! @return Pointer to service or nullptr if not found
        template<typename T>
        T* Get() const
        {
            if(auto it = _services.find(typeid(T)); it != _services.end())
            {
                return static_cast<T*>(it->second.get());
            }

            return nullptr;
        }

        //! @brief Retrieves a service
        //! @tparam T Service type
        //! @return Reference to service or throws an exception if it's missing
        template<typename T>
        T& GetRequired() const
        {
            auto service = Get<T>();
            if(!service)
            {
                throw Exception("Service not found: {}", typeid(T).name());
            }

            return *service;
        }

    private:
        std::unordered_map<std::type_index, std::shared_ptr<void>> _services;
    };
}