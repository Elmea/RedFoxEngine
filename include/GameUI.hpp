#pragma once
#include "RedfoxMaths.hpp"
#include "MyMemory.hpp"
#include "OpenGLGraphics.hpp"
namespace RedFoxEngine
{
    struct GameUI
    {
        bool buttonPressed = false;
        int parent;
        RedFoxMaths::Float2 screenPosition;
        RedFoxMaths::Float2 scale;
        GLuint image;
        MyString name;
        MyString text;
    };
}