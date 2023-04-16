#include "GameObject.hpp"

#define MEMORY_IMPLEMENTATION
#include "MyMemory.hpp"

namespace RedFoxEngine
{
        RedFoxMaths::Mat4 GameObject::GetLocalMatrix()
        {
            return (RedFoxMaths::Mat4::CreateTransformMatrix(position, orientation, scale));
        }
       
        // int GameObject::GetChildrenCount(GameObject *objects, int objectCount)
        // {
        //     int count = 0;
        //     for (int i = 0; i < objectCount; i++)
        //     {
        //         if (objects[i].parent == this)
        //             count++;
        //     }
        //     return (count);
        // }
        // int GameObject::GetChildrenCount(GameObject *objects, int objectCount)
        // {
        //     int count = 0;
        //     for (int i = 0; i < objectCount; i++)
        //     {
        //         if (objects[i].parent == this)
        //             count++;
        //     }
        //     return (count);
        // }
}
