#include "GameObject.hpp"

#define MEMORY_IMPLEMENTATION
#include "MyMemory.hpp"

namespace RedFoxEngine
{
        RedFoxMaths::Mat4 GameObject::GetLocalMatrix()
        {
            return (RedFoxMaths::Mat4::CreateTransformMatrix(position, orientation, scale));
        }
       
        RedFoxMaths::Mat4 GameObject::GetWorldMatrix()
        {
            if (parent)
                return GetLocalMatrix() * parent->GetWorldMatrix();

            return GetLocalMatrix();
        }

        GameObject **GameObject::GetChildren(GameObject *objects, int objectCount, Memory *temp)
        {
            GameObject **result = (GameObject **)MyMalloc(temp, sizeof(GameObject **));
            int count = 0;
            for (int i = 0; i < objectCount; i++)
            {
                if (objects[i].parent == this)
                    result[count++] = &objects[i];
            }
            result[count] = nullptr;
            return(result);
        }

        int GameObject::GetChildrenCount(GameObject *objects, int objectCount)
        {
            int count = 0;
            for (int i = 0; i < objectCount; i++)
            {
                if (objects[i].parent == this)
                    count++;
            }
            return (count);
        }

        void GameObject::Test()
        {
            RedFoxEngine::Transform tr;

            tr.orientation = RedFoxMaths::Quaternion();
            tr.position = RedFoxMaths::Float3(0, 0, 0);
            tr.scale = RedFoxMaths::Float3(0, 0, 0);


           
            PxTransform test = tr.toPxTransform();

        }
}
