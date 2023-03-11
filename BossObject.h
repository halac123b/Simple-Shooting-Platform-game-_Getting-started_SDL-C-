#ifndef BOSS_OBJECT_H
#define BOSS_OBJECT_H

#include "BaseObject.h"
#include "CommonFunc.h"
#include "Bullet.h"

#define GRAVITY_SPEED 0.8
#define MAX_FALL_SPEED 10

#define PLAYER_SPPED 2
#define PLAYER_HIGH_VAL 18

#define BOSS_FRAME_NUM 32

class BossObject : public BaseObject
{
public:
  BossObject();
  void setVelX(float _velX) { velX = _velX; }
  void setVelY(float _velY) { velY = _velY; }
  void setPosX(int _posX) { posX = _posX; };
  void setPosY(int _posY) { posY = _posY; }
  int getPosX() { return posX; }
  int getPosY() { return posY; }
  int getFrameWidth() { return frameWidth; }
  int getFrameHeight() { return frameHeight; }
  void setMapXY(int _mapX, int _mapY)
  {
    mapX = _mapX;
    mapY = _mapY;
  }
  vector<Bullet *> getBulletList() { return bulletList; }
  void setBulletList(vector<Bullet *> list) { bulletList = list; }

  void show(SDL_Renderer *screen);
  bool loadImg(string path, SDL_Renderer *screen);
  void clipFrame();
  void checkTouchMap(Map &mapData);
  void moveBoss(Map &mapData);
  void initBoss();
  void initBullet(SDL_Renderer *screen);
  void handleBullet(SDL_Renderer *screen, int borderX, int borderY);

private:
  int mapX;
  int mapY;
  int onGround;
  int thinkTime;
  Input inputType;
  int posX;
  int posY;
  float velX;
  float velY;
  SDL_Rect frameClip[BOSS_FRAME_NUM];
  int frame;
  int frameWidth;
  int frameHeight;
  vector<Bullet *> bulletList;
};

#endif