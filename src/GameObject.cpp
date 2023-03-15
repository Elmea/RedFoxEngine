#include "GameObject.hpp"

#define MEMORY_IMPLEMENTATION
#include "MyMemory.hpp"

namespace RedFoxEngine
{
        RedFoxMaths::Mat4 GameObject::GetWorldMatrix()
        {
            RedFoxMaths::Mat4 local = RedFoxMaths::Mat4::CreateTransformMatrix(position, orientation, {scale, scale, scale});
            if (parent)
                return local * parent->GetWorldMatrix();

            return local;
        }

        GameObject **GameObject::GetChildren(GameObject *models, int modelCount, Memory *temp, int* childrenCount)
        {
            GameObject **result = (GameObject **)MyMalloc(temp, sizeof(GameObject **));
            int count = 0;
            for (int i = 0; i < modelCount; i++)
            {
                if (models[i].parent == this)
                {
                    result[count] = (GameObject *)MyMalloc(temp, sizeof(GameObject*));
                    result[count] = &models[i];
                    count++;
                }
            }
            if (childrenCount) *childrenCount = count;
            result[count] = nullptr;
            return(result);
        }
}
