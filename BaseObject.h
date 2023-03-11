#ifndef BASE_OBJECT_H
#define BASE_OBJECT_H

#include "CommonFunc.h"

using namespace std;

class BaseObject
{
public:
  BaseObject();
  ~BaseObject();

  void setRect(const int &x, const int &y)
  {
    rect.x = x;
    rect.y = y;
  }
  SDL_Rect getRect() const { return rect; }
  SDL_Texture *getObject() { return p_object; }

  bool loadImg(string path, SDL_Renderer *screen);
  void render(SDL_Renderer *des, const SDL_Rect *clip = NULL);
  void free();

protected:
  SDL_Texture *p_object;
  SDL_Rect rect;
};

#endif