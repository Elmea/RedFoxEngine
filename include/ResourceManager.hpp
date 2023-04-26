#pragma once
#include "MyMemory.hpp"
#include "meow_hash_x64_aesni.h"

namespace RedFoxEngine
{
    struct Allocators
    {
        Memory arena;
        Memory temp;
    };

    class ResourcesManager
    {
        public:
        public:
        int m_sceneUsedMemory;
        Allocators m_memory;
        ResourcesManager();
        void *PersistentAllocation(u64 size);
        void *TemporaryAllocation(u64 size);
    };
}
