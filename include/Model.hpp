#include "ObjParser.hpp"
#include "RedfoxMaths.hpp"

namespace RedFoxEngine
{
    struct Model
    {
        int parentId;
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
    };
}