#pragma once
#include "MyMemory.hpp"
#include "meow_hash_x64_aesni.h"

namespace RedFoxEngine
{
    struct Material
    {
        vec3 ambient;
        float Opaqueness;

        vec3 diffuse;
        float Shininess;

        vec3 specular;
        int diffuseMap;

        vec3 emissive;
        int normalMap;
    };

    typedef struct MaterialKey
    {
        u64 key;
        u32 index;
        MaterialKey *next;
    } MaterialKey;

    typedef struct HashKey
    {
        u64 key; //TODO: replace key with the hash ?
        u32 index;
        HashKey *next;
    } HashKey;
    struct Allocators
    {
        Memory arena;
        Memory temp;
    };
    class ResourcesManager
    {
        public:
        MaterialKey *materialHash = 0;
        MaterialKey *materialList = 0;
        int listCount = 0;
        int materialCount = 0;

        Material *materials = nullptr;
        public:
        int m_sceneUsedMemory;
        Allocators m_memory;
        ResourcesManager();
        void *PersistentAllocation(u64 size);
        void *TemporaryAllocation(u64 size);
        int AddMaterial(Material newMtl)
        {
            u64 hashSlot = MeowU64From(MeowHash(MeowDefaultSeed, sizeof(Material), &newMtl), 0);
            if (materials == nullptr)
            {
                materials = (Material *)PersistentAllocation(sizeof(Material) * 100);
                materialHash = (MaterialKey *)PersistentAllocation(sizeof(MaterialKey) * 100);
                materialList = (MaterialKey *)PersistentAllocation(sizeof(MaterialKey) * 100);
                memset(materials, 0, sizeof(Material) * 100);
                memset(materialHash, 0, sizeof(MaterialKey) * 100);
                memset(materialList, 0, sizeof(MaterialKey) * 100);
            }

            MaterialKey *key = materialHash + (hashSlot % 100);
            if (key->key == 0)
            {
                key->key = hashSlot;
                key->index = materialCount;
                materials[materialCount] = newMtl;
                materialCount++;
                return (key->index);
            }
            else
            {
                MaterialKey *current = key;
                while (current)
                {
                    if (hashSlot == current->key)
                        return (current->index);
                    else if (current->next == nullptr)
                    {
                        current->next = materialList + listCount;
                        current->next->key = hashSlot;
                        current->next->index = materialCount;
                        materials[materialCount] = newMtl;
                        materialCount++;
                        listCount++;
                        return (current->next->index); 
                    }
                    current = current->next;
                }
            }
            FatalExit(-1);
            return (0);
        }
    };
}
