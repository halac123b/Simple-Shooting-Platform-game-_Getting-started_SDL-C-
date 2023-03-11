#include "BaseObject.h"

BaseObject::BaseObject()
{
  p_object = NULL;
  rect.x = rect.y = rect.w = rect.h = 0;
}

BaseObject::~BaseObject()
{
  free();
}

bool BaseObject::loadImg(string path, SDL_Renderer *screen)
{
  free();
  SDL_Texture *newTexture = NULL;
  SDL_Surface *loadSurface = IMG_Load(path.c_str());
  if (loadSurface != NULL)
  {
    SDL_SetColorKey(loadSurface, SDL_TRUE, SDL_MapRGB(loadSurface->format, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B));
    newTexture = SDL_CreateTextureFromSurface(screen, loadSurface);
    if (newTexture != NULL)
    {
      rect.w = loadSurface->w;
      rect.h = loadSurface->h;
    }
    SDL_FreeSurface(loadSurface);
  }
  p_object = newTexture;
  return newTexture != NULL;
}

void BaseObject::render(SDL_Renderer *des, const SDL_Rect *clip)
{
  SDL_Rect renderQuad = {rect.x, rect.y, rect.w, rect.h};
  SDL_RenderCopy(des, p_object, clip, &renderQuad);
}

void BaseObject::free()
{
  if (p_object != NULL)
  {
    SDL_DestroyTexture(p_object);
    p_object = NULL;
    rect.h = rect.w = 0;
  }
}
