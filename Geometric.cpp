#include "Geometric.h"

void Geometric::renderRectangle(GeometricFormat geoSize, ColorData &colorData, SDL_Renderer *screen)
{
  SDL_Rect fillRect = {geoSize.left, geoSize.top, geoSize.width, geoSize.height};
  SDL_SetRenderDrawColor(screen, colorData.red, colorData.green, colorData.blue, 255);
  SDL_RenderFillRect(screen, &fillRect);
}

void Geometric::renderOutline(GeometricFormat geoSize, ColorData colorData, SDL_Renderer *screen)
{
  SDL_Rect outlineRect = {geoSize.left, geoSize.top, geoSize.width, geoSize.height};
  SDL_SetRenderDrawColor(screen, colorData.red, colorData.green, colorData.blue, 255);
  SDL_RenderDrawRect(screen, &outlineRect);
}