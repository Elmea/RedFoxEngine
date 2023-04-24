#pragma once
#include "RedfoxMaths.hpp"
#include "MyMemory.hpp"
#include "OpenGLGraphics.hpp"
namespace RedFoxEngine
{
    struct GameUI
    {
        MyString name;
        GameUI* parent;

        RedFoxMaths::Float2 screenPosition;
        RedFoxMaths::Float2 scale;

        MyString text;
        GLuint image;
        void* callbackFunction;
        bool isPressed = false;


        GameUI** GetChildren(GameUI* objects, int objectCount, Memory* temp);
        int GetChildrenCount(GameUI* objects, int objectCount);
    };
}