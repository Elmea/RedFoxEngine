#include "ObjParser.hpp"
#include "RedfoxMaths.hpp"

namespace RedFoxEngine
{
    struct Model
    {
        Model* parent;
        u32 vao;
        ObjModel obj;
        RedFoxMaths::Float3 position;
        RedFoxMaths::Quaternion orientation;
        f32 scale;

        RedFoxMaths::Mat4 GetWorldMatrix()
        {
            RedFoxMaths::Mat4 local = RedFoxMaths::Mat4::CreateTransformMatrix(position, orientation, {scale, scale, scale});

            if (parent)
                return local * parent->GetWorldMatrix();

            return local;
        }

        Model **GetChildren(Model *models, int modelCount, Memory *temp)
        {
            Model **result = (Model**)MyMalloc(temp, sizeof(Model**));
            int count = 0;
            for (int i = 0; i < modelCount; i++)
            {
                if (models[i].parent == this)
                {
                    result[count] = (Model *)MyMalloc(temp, sizeof(Model*));
                    result[count] = models[i].parent;
                    count++;
                }
            }
            result[count] = nullptr;
            return(result);
        }
        //TODO do we need this?
        Model *GetParent()
        {
            return(parent);
        }

        void SetParent(Model *model)
        {
            parent = model;
        }

    };
}