#ifndef PLAYER_STAT_H
#define PLAYER_STAT_H

#include "CommonFunc.h"
#include "BaseObject.h"

class PlayerStat : public BaseObject
{
public:
  PlayerStat();
  void setNumber(int num) { numberLife = num; }
  void addPos(int posX);
  // Show các icon số mạng ra screen
  void show(SDL_Renderer *screen);
  // Init game với 3 mạng
  void init(SDL_Renderer *screeen);
  // Tăng giảm mạng
  void increaseLife();
  void decreaseLife();

private:
  int numberLife;      // Số mạng của player
  vector<int> posList; // Vị trí đặt các icon của các mạng
};

class PlayerMoney : public BaseObject
{
public:
  PlayerMoney();
  void init(SDL_Renderer *screen);
  void show(SDL_Renderer *screen);
  void setPos(int _posX, int _posY)
  {
    posX = _posX;
    posY = _posY;
  }

private:
  int posX;
  int posY;
};
#endif