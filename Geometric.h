#ifndef GEOMETRIC_H
#define GEOMETRIC_H

#include "CommonFunc.h"

// Struct cho loại hình học
struct GeometricFormat
{
public:
  GeometricFormat(int _left, int _top, int _width, int _height) : left(_left), top(_top), width(_width), height(_height) {}

  int left;
  int top;
  int width;
  int height;
};

struct ColorData
{
  ColorData(Uint8 r, Uint8 g, Uint8 b) : red(r), green(g), blue(b) {}

  Uint8 red;
  Uint8 green;
  Uint8 blue;
};

class Geometric
{
public:
  // Vẽ 1 hình fill cả bên trong
  static void renderRectangle(GeometricFormat geoSize, ColorData &colorData, SDL_Renderer *screen);
  // Vẽ 1 hình chỉ vẽ viền
  static void renderOutline(GeometricFormat geoSize, ColorData colorData, SDL_Renderer *screen);
};

#endif