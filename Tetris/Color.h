#pragma once
#include <d2d1.h>

namespace Color
{
    typedef struct HSV
    {
        float H;
        float S;
        float V;
    } HSV;

    HSV RGBtoHSV(float r, float g, float b);
    D2D1::ColorF HSVtoRGB(float H, float S, float V);
    D2D1::ColorF BrightenColor(D2D1::ColorF color, float v);
    D2D1::ColorF DarkenColor(D2D1::ColorF color, float v);
}