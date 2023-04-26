#pragma once
#include "RedfoxMaths.hpp"
#include "MyMemory.hpp"

namespace RedFoxEngine
{
    struct GameUI
    {
        MyString name;
        int parent;

        RedFoxMaths::Float2 screenPosition;
        RedFoxMaths::Float2 scale;

        MyString text;
        MyString imagePath;
        MyString buttonName;
        u32 image;
/*
        void* callbackFunction;
*/
        bool isPressed = false;

    };
}