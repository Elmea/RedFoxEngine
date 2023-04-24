#pragma once
#include "RedfoxMaths.hpp"
#include "MyMemory.hpp"
#include "OpenGLGraphics.hpp"
namespace RedFoxEngine
{
    struct GameUI
    {
        MyString name;
        int parent;

        RedFoxMaths::Float2 screenPosition;
        RedFoxMaths::Float2 scale;

        MyString text;
/*
        GLuint image;
        void* callbackFunction;
*/
        bool isPressed = false;

    };
}