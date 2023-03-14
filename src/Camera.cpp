#include "Camera.hpp"

using namespace RedFoxEngine;
using namespace RedFoxMaths;

Camera::Camera(projectionType projection, float aspect)
{
    m_parameters.FOV = 90;
    m_parameters.bottom = -10;
    m_parameters.top = 10;
    m_parameters.left = -10;
    m_parameters.right = 10;
    m_parameters._near = 0.1f;
    m_parameters._far = 100;
    m_parameters.aspect = aspect;
    SetProjection(projection);
}    


void Camera::SetProjection(projectionType projectionType)
{
    switch (projectionType)
    {
    case (projectionType::PERSPECTIVE):
        m_projection = Mat4::GetPerspectiveMatrix(m_parameters.aspect, m_parameters.FOV, m_parameters._far, m_parameters._near);
        break;

    case (projectionType::ORTHOGRAPHIC):
        m_projection = Mat4::GetOrthographicMatrix(m_parameters.right, m_parameters.left, m_parameters.top, m_parameters.bottom, m_parameters._far, m_parameters._near);
        break;

    default:
        break;
    }
}

RedFoxMaths::Mat4 Camera::GetVP()
{
    return m_projection * Mat4::CreateTransformMatrix(position, orientation, {1.0f, 1.0f, 1.0f}).GetInverseMatrix();
}

