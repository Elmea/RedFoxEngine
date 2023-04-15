#include "ResourceManager.hpp"

#define MEMORY_IMPLEMENTATION
#include "MyMemory.hpp"

RedFoxEngine::ResourcesManager::ResourcesManager()
{
    m_memory.arena = InitVirtualMemory(1 * GigaByte);
    m_memory.temp = InitVirtualMemory(1 * GigaByte); 
    IncreaseTotalCapacity(&m_memory.arena, 1 * MegaByte);
}

void *RedFoxEngine::ResourcesManager::PersistentAllocation(u64 size)
{
    return(MyMalloc(&m_memory.arena, size));
}

void *RedFoxEngine::ResourcesManager::TemporaryAllocation(u64 size)
{
    return(MyMalloc(&m_memory.temp, size));
}