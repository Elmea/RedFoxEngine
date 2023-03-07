#include "ObjParser.hpp"
#include "RedfoxMaths.hpp"

namespace RedFoxEngine
{
struct Model
{
    u32 vao;
    ObjModel obj;
    RedFoxMaths::Float3 position;
    RedFoxMaths::Quaternion orientation;
    f32 scale;
};
} // namespace RedFoxEngine