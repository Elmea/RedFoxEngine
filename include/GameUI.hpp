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
        RedFoxMaths::Float2 size;
        RedFoxMaths::Float3 textColor;
        RedFoxMaths::Float3 hoverColor;
        RedFoxMaths::Float3 selectedColor;

        //TODO Function callback



        MyString text;
        u32 image = 0;
  
        bool isPressed = false;
        bool isHovered = false;

    };
}