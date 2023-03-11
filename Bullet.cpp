#include "Bullet.h"

Bullet::Bullet()
{
  velX = velY = 0;
  isMove = false;
  bulletType = SPHERE_BULLET;
}

void Bullet::handleMove(int borderX, int borderY)
{
  if (direction == DIR_RIGHT)
  {
    rect.x += velX;
    if (rect.x > borderX)
    {
      isMove = false;
    }
  }
  else if (direction == DIR_LEFT)
  {
    rect.x -= velX;
    if (rect.x < 0)
    {
      isMove = false;
    }
  }
  else if (direction == DIR_UP)
  {
    rect.y -= velY;
    if (rect.y < 0)
    {
      isMove = false;
    }
  }
  else if (direction == DIR_UP_LEFT)
  {
    rect.x -= velX;
    if (rect.x < 0)
    {
      isMove = false;
    }
    rect.y -= velY;
    if (rect.y < 0)
    {
      isMove = false;
    }
  }
  else if (direction == DIR_UP_RIGHT)
  {
    rect.x += velX;
    if (rect.x > borderX)
    {
      isMove = false;
    }
    rect.y -= velY;
    if (rect.y < 0)
    {
      isMove = false;
    }
  }
  else if (direction == DIR_DOWN_LEFT)
  {
    rect.x -= velX;
    if (rect.x < 0)
    {
      isMove = false;
    }
    rect.y += velY;
    if (rect.y > borderY)
    {
      isMove = false;
    }
  }
  else if (direction == DIR_DOWN_RIGHT)
  {
    rect.x += velX;
    if (rect.x > borderX)
    {
      isMove = false;
    }
    rect.y += velY;
    if (rect.y > borderY)
    {
      isMove = false;
    }
  }
}

bool Bullet::loadBulletImg(SDL_Renderer *screen)
{
  bool ret = false;
  if (bulletType == SPHERE_BULLET)
  {
    ret = loadImg("img/sphere_bullet.png", screen);
  }
  else if (bulletType == LASER_BULLET)
  {
    ret = loadImg("img/laser_bullet.png", screen);
  }
  return ret;
}