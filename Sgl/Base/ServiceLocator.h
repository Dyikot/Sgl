#pragma once

#include <unordered_map>
#include <memory>
#include <typeindex>
#include "Exceptions.h"

namespace Sgl
{
    /// <summary>
    /// Service Locator container
    /// </summary>
    class ServiceLocator
    {
    public:
        /// <summary>
        /// Registers a service using a std::shared_ptr
        /// </summary>
        template<typename TInterface, typename TImpl = TInterface>
        void Add(std::shared_ptr<TImpl> service)
        {
            _services[typeid(TInterface)] = std::static_pointer_cast<TInterface>(std::move(service));
        }

        /// <summary>
        /// Registers a service using a std::unique_ptr
        /// </summary>
        template<typename TInterface, typename TImpl = TInterface>
        void Add(std::unique_ptr<TImpl> service)
        {
            _services[typeid(TInterface)] = std::shared_ptr<TInterface>(std::move(service));
        }

        /// <summary>
        /// Register a default initializable service
        /// </summary>
        template<typename TInterface, typename TImpl = TInterface>
        void Add() requires std::default_initializable<TImpl>
        {
            Add<TInterface, TImpl>(std::make_shared<TImpl>());
        }

        /// <summary>
        /// Checks if a service has been registered
        /// </summary>
        /// <returns>true if registered, false if not</returns>
        template<typename T>
        bool Has() const
        {
            return _services.find(typeid(T)) != _services.end();
        }

        /// <summary>
        /// Retrieves a service
        /// </summary>
        /// <returns>Pointer to service or nullptr if not found</returns>
        template<typename T>
        T* Get() const
        {
            if(auto it = _services.find(typeid(T)); it != _services.end())
            {
                return static_cast<T*>(it->second.get());
            }

            return nullptr;
        }

        /// <summary>
        /// Retrieves a service
        /// </summary>
        /// <returns>Reference to service or throws an exception if it's missing</returns>
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