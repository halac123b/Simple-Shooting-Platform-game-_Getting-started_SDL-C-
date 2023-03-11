#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "CommonFunc.h"
#include "BaseObject.h"

#define NUM_FRAME_EXP 8 // Số frame của hiệu ứng nổ

class ExplosionObject : public BaseObject
{
public:
  ExplosionObject();

  void clipFrame(); // Cắt frame từ sprite lớn
  void setFrame(int _frame) { frame = _frame; }
  virtual bool loadImg(string path, SDL_Renderer *screen);
  void show(SDL_Renderer *screen);
  int getFrameWidth() { return frameWidth; }
  int getFrameHeight() { return frameHeight; }

public:
  int frameWidth;
  int frameHeight;
  int frame;
  SDL_Rect frameClip[NUM_FRAME_EXP];
};

#endif