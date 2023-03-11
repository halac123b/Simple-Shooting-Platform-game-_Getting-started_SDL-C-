#include "PlayerStat.h"

PlayerStat::PlayerStat()
{
  numberLife = 0;
}

void PlayerStat::addPos(int posX)
{
  posList.push_back(posX);
}

void PlayerStat::init(SDL_Renderer *screen)
{
  loadImg("img/player_pw.png", screen);
  numberLife = 3;
  if (posList.size() > 0)
  {
    posList.clear();
  }
  addPos(20);
  addPos(60);
  addPos(100);
}

void PlayerStat::show(SDL_Renderer *screen)
{
  for (int i = 0; i < posList.size(); i++)
  {
    rect.x = posList.at(i);
    rect.y = 0;
    render(screen);
  }
}

void PlayerStat::decreaseLife()
{
  numberLife--;
  posList.pop_back();
}

void PlayerStat::increaseLife()
{
  numberLife++;
  int lastPos = posList.back();
  lastPos += 40;
  posList.push_back(lastPos);
}

PlayerMoney::PlayerMoney()
{
  posX = posY = 0;
}

void PlayerMoney::init(SDL_Renderer *screen)
{
  bool ret = loadImg("img/money_img.png", screen);
}

void PlayerMoney::show(SDL_Renderer *screen)
{
  rect.x = posX;
  rect.y = posY;
  render(screen);
}