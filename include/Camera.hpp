#pragma once

#include "GameObject.hpp"

namespace RedFoxEngine
{
    enum projectionType
    {
        ORTHOGRAPHIC,
        PERSPECTIVE,
    };

    struct CameraParameters
    {
        float right, left;
        float top, bottom;
        float _far, _near; // The underscore is too avoid conflict with standard library
        float FOV, aspect;
    };
    
    class Camera
    {
    public:
        union
        {
            Transform transform;
            struct
            {
              RedFoxMaths::Float3 position;
              RedFoxMaths::Float3 scale;
              RedFoxMaths::Quaternion orientation;
            };
        };
        RedFoxMaths::Mat4 m_projection;
        CameraParameters m_parameters;
        
    public:
        Camera(projectionType projection, float aspect);

        void SetProjection(projectionType projectionType);

        void SetParameters(CameraParameters parameters)
        {
            m_parameters = parameters;
        }
        void SetViewLookAt(RedFoxMaths::Float3 target, RedFoxMaths::Float3 up);
        
        RedFoxMaths::Mat4 GetViewMatrix();
        RedFoxMaths::Mat4 GetVP();
    };
}