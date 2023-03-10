#pragma once
#include <unordered_map>

namespace Resource
{
    class IResource
    {
    public:
        int id;

        virtual void Load(const char* pFileName) = 0;

        virtual void Unload() = 0;
    };

    class ResourceManager
    {
    public:
        template<typename T>
        inline IResource* Create(const char* pFileName);

        template<typename T>
        IResource* Create(const char* pFileName, const char* pTag);

        template<typename T>
        IResource* Get(const char* pTag);

        template<typename T>
        void Delete(const char* pTag);

        void Reset();

        ~ResourceManager();

    private:
        std::unordered_map<std::string, IResource*> resources;
    };
}

#include "ResourceManager.inl"
