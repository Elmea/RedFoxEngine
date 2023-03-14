#pragma once

#include "GameObject.hpp"

namespace RedFoxEngine
{
    enum projectionType
    {
        ORTHOGRAPHIC,
        PERSPECTIVE
    };

    struct CameraParameters
    {
        float right, left;
        float top, bottom;
        float _far, _near; // The underscore is too avoid conflict with standard library
        float FOV, aspect;
    };
    
    class Camera : public GameObject
    {
        CameraParameters m_parameters;

        RedFoxMaths::Mat4 m_projection;
        
    public:
        Camera(projectionType projection, float aspect);
        
        void SetProjection(projectionType projectionType);

        void SetParameters(CameraParameters parameters)
        {
            m_parameters = parameters;
        }
        
        RedFoxMaths::Mat4 GetVP();
    };
}