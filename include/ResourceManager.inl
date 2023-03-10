#pragma once

#include <type_traits>
#include <string>
#include <unordered_map>

#include "ThreadManager.hpp"

namespace Resource
{
    template<typename T>
    inline IResource* ResourceManager::Create(const char* pFileName)
    {
        std::string tag = pFileName;
        if (!std::is_base_of<IResource, T>())
            return nullptr;

        if (resources[tag] != nullptr)
            delete resources[tag];

        T* resource = new T();
        resources[tag] = (IResource*)resource;
        resources[tag]->id = resources.size();

        Core::threadManager.AddThread(Core::ThreadType::LOAD, new std::thread(&IResource::Load, resource, pFileName));

        return resources[tag];
    }

    template<typename T>
    inline IResource* ResourceManager::Create(const char* pFileName, const char* pTag)
    {
        std::string tag = pTag;
        if (!std::is_base_of<IResource, T>())
            return nullptr;

        if (resources[tag] != nullptr)
            delete resources[tag];

        T* resource = new T();
        resources[tag] = (IResource*)resource;
        resources[tag]->id = resources.size();

        Core::threadManager.AddThread(Core::ThreadType::LOAD, new std::thread(&IResource::Load, resource, pFileName));

        return resources[tag];

    }
    template<typename T>
    inline IResource* ResourceManager::Get(const char* pTag)
    {
        std::string tag = pTag;
        if (!resources[tag])
            return nullptr;

        return resources[tag];
    }

    template<typename T>
    inline void ResourceManager::Delete(const char* pTag)
    {
        std::string tag = pTag;
        if (!resources[tag])
            return;

        delete resources[tag];
        resources.erase(tag);
    }

    inline void ResourceManager::Reset()
    {
        for (auto resource : resources)
        {
            delete resource.second;
        }
        resources.clear();
    }

    inline ResourceManager::~ResourceManager()
    {
        for (auto resource : resources)
        {
            resource.second->Unload();
            delete resource.second;
        }
        resources.clear();
    }
}