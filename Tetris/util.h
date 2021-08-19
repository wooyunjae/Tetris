#pragma once
#pragma warning (disable : 4244)
#include <d2d1.h>
#include <cmath>
typedef struct HSV
{
    float H;
    float S;
    float V;
} HSV;

HSV RGBtoHSV(float r, float g, float b)
{
    // h, s, v = hue, saturation, value
    float cmax = max(r, max(g, b)); // maximum of r, g, b
    float cmin = min(r, min(g, b)); // minimum of r, g, b
    float diff = cmax - cmin; // diff of cmax and cmin.
    float h = -1;
    float s = -1;

    // if cmax and cmax are equal then h = 0
    if (cmax == cmin)
    {
        h = 0;
    }
    // if cmax equal r then compute h
    else if (cmax == r)
    {
        h = (static_cast<int>(60.f * ((g - b) / diff) + 360.f) % 360);
    }
    // if cmax equal g then compute h
    else if (cmax == g)
    {
        h = (static_cast<int>(60 * ((b - r) / diff) + 120) % 360);
    }
    // if cmax equal b then compute h
    else if (cmax == b)
    {
        h = (static_cast<int>(60 * ((r - g) / diff) + 240) % 360);
    }

    // if cmax equal zero
    if (cmax == 0)
    {
        s = 0;
    }
    else
    {
        s = (diff / cmax) * 100;
    }

    // compute v
    float v = cmax * 100;
    HSV hsv;
    hsv.H = h;
    hsv.S = s;
    hsv.V = v;
    return hsv;
}

D2D1::ColorF HSVtoRGB(float H, float S, float V) {
    float s = S / 100;
    float v = V / 100;
    float C = s * v;
    float X = static_cast<float>(C * (1 - abs(fmod(H / 60.0, 2) - 1)));
    float m = v - C;
    float r, g, b;
    if (H >= 0 && H < 60) {
        r = C, g = X, b = 0;
    }
    else if (H >= 60 && H < 120) {
        r = X, g = C, b = 0;
    }
    else if (H >= 120 && H < 180) {
        r = 0, g = C, b = X;
    }
    else if (H >= 180 && H < 240) {
        r = 0, g = X, b = C;
    }
    else if (H >= 240 && H < 300) {
        r = X, g = 0, b = C;
    }
    else {
        r = C, g = 0, b = X;
    }
    return D2D1::ColorF(r + m, g + m, b + m);
}

D2D1::ColorF BrightenColor(D2D1::ColorF color, float v)
{
	HSV hsv;
	hsv = RGBtoHSV(color.r, color.g, color.b);
	hsv.V += v;
	hsv.V = min(hsv.V, 100);
	return HSVtoRGB(hsv.H,hsv.S,hsv.V);
}

D2D1::ColorF DarkenColor(D2D1::ColorF color, float v)
{
	HSV hsv;
	hsv = RGBtoHSV(color.r, color.g, color.b);
	if (v >= hsv.V)
	{
		hsv.V = 0;
	}
	else
	{
		hsv.V -= v;
	}
	return HSVtoRGB(hsv.H, hsv.S, hsv.V);
}