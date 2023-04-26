#pragma once
#include "RedfoxMaths.hpp"
#include "MyMemory.hpp"

namespace RedFoxEngine
{
    struct GameUI
    {
        MyString name;
        int      parent;

        RedFoxMaths::Float2 screenPosition;
        RedFoxMaths::Float2 scale;

        MyString text;
        u32 image = 0;
/*
        void* callbackFunction;
*/
        bool isPressed = false;
        bool isHovered = false;

    };
}