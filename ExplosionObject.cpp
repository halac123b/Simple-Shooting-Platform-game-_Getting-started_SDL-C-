#include "ExplosionObject.h"

ExplosionObject::ExplosionObject()
{
  frameHeight = frameWidth = 0;
  frame = 0;
}

bool ExplosionObject::loadImg(string path, SDL_Renderer *screen)
{
  bool ret = BaseObject::loadImg(path, screen);
  if (ret)
  {
    frameWidth = rect.w / NUM_FRAME_EXP;
    frameHeight = rect.h;
  }
  return ret;
}

void ExplosionObject::clipFrame()
{
  if (frameWidth > 0 && frameHeight > 0)
  {
    for (int i = 0; i < NUM_FRAME_EXP; i++)
    {
      frameClip[i].x = i * frameWidth;
      frameClip[i].y = 0;
      frameClip[i].w = frameWidth;
      frameClip[i].h = frameHeight;
    }
  }
}

void ExplosionObject::show(SDL_Renderer *screen)
{
  SDL_Rect *currentFrame = &frameClip[frame];
  SDL_Rect renderQuad = {rect.x, rect.y, frameWidth, frameHeight};
  SDL_RenderCopy(screen, p_object, currentFrame, &renderQuad);
}
