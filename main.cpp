#include "CommonFunc.h"
#include "BaseObject.h"
#include "TileMap.h"
#include "Player.h"
#include "Timer.h"
#include "ThreatObject.h"
#include "ExplosionObject.h"
#include "TextObject.h"
#include "PlayerStat.h"
#include "Geometric.h"
#include "BossObject.h"

#define FPS 25

BaseObject g_background;
TTF_Font *fontTime = NULL;

bool initData()
{
  bool success = true;
  int ret = SDL_Init(SDL_INIT_VIDEO);
  if (ret < 0)
  {
    return false;
  }
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

  g_window = SDL_CreateWindow("Learn SDL",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              SCREEN_WIDTH, SCREEN_HEIGHT,
                              SDL_WINDOW_SHOWN);
  if (g_window == NULL)
  {
    success = false;
  }
  else
  {
    g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
    if (g_screen == NULL)
    {
      success = false;
    }
    else
    {
      SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
      int imgFlag = IMG_INIT_PNG;
      if (IMG_Init(imgFlag) && imgFlag == false)
      {
        success = false;
      }
    }
    if (TTF_Init() == -1)
    {
      success = false;
    }
    fontTime = TTF_OpenFont("font/dlxfont_.ttf", 15);
    if (fontTime == NULL)
    {
      success = false;
    }
  }
  return success;
}

bool loadBackground()
{
  bool ret = g_background.loadImg("img/background.png", g_screen);
  if (ret == false)
    return false;
  return true;
}

void close()
{
  g_background.free();
  SDL_DestroyRenderer(g_screen);
  g_screen = NULL;

  SDL_DestroyWindow(g_window);
  g_window = NULL;

  IMG_Quit();
  SDL_Quit();
}

vector<ThreatObject *> makeThreadList()
{
  vector<ThreatObject *> listThreat;
  ThreatObject *threatObj = new ThreatObject[20];
  for (int i = 0; i < 20; i++)
  {
    ThreatObject *temp = (threatObj + i);
    if (temp != NULL)
    {
      if (i % 2 == 0)
      {
        temp->loadImg("img/threat_level.png", g_screen);
        temp->clipFrame();
        temp->setPosX(700 + 1200 * i);
        temp->setPosY(250);
        Bullet *threatBullet = new Bullet;
        temp->initBullet(threatBullet, g_screen);
      }
      else if (i % 2 == 1)
      {
        temp->loadImg("img/threat_left.png", g_screen);
        temp->clipFrame();
        temp->setMoveType(ThreatObject::MOVE_IN_SPACE);
        temp->setInputLeft(1);
        temp->setPosX(700 + 1200 * i);
        temp->setPosY(250);
        temp->setAnimationPos(temp->getPosX() - 60, temp->getPosX() + 60);
      }
      listThreat.push_back(temp);
    }
  }
  return listThreat;
}

int main(int argc, char *argv[])
{
  if (initData() == false)
    return -1;
  if (loadBackground() == false)
    return -1;

  Timer timerFPS;
  TileMap tileMap;
  char mapFile[] = "map/map01.txt";
  tileMap.loadMap(mapFile);
  tileMap.loadTile(g_screen);

  Player player;
  player.loadImg("img/player_left.png", g_screen);
  player.clipFrame();

  PlayerStat playerLife;
  playerLife.init(g_screen);

  PlayerMoney playerMoney;
  playerMoney.init(g_screen);
  playerMoney.setPos(SCREEN_WIDTH * 0.5 - 300, 8);

  vector<ThreatObject *> listThreat = makeThreadList();

  // Boss object
  BossObject bossObject;
  bool ret = bossObject.loadImg("img/boss_object.png", g_screen);
  bossObject.clipFrame();
  int bossPosX = MAX_MAP_X * TILE_SIZE - SCREEN_WIDTH * 0.6;
  bossObject.setPosX(bossPosX);
  bossObject.setPosY(10);

  ExplosionObject threatExplo;
  bool eRet = threatExplo.loadImg("img/explosion.png", g_screen);
  if (eRet == false)
    return -1;
  threatExplo.clipFrame();

  int frameExpWidth = threatExplo.getFrameWidth();
  int frameExpHeight = threatExplo.getFrameHeight();

  int numDie = 0; // Số lần đã die của player

  // Time text
  TextObject timeGame;
  timeGame.setTextColor(TextObject::WHITE);
  // Point text
  TextObject pointText;
  pointText.setTextColor(TextObject::BLACK);
  UINT pointValue = 0;
  // Coin text
  TextObject coinText;
  coinText.setTextColor(TextObject::RED);

  bool isQuit = false;

  while (isQuit == false)
  {
    timerFPS.start();
    while (SDL_PollEvent(&g_event) != 0)
    {
      if (g_event.type == SDL_QUIT)
      {
        isQuit = true;
      }
      player.handleInputAction(g_event, g_screen);
    }
    SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
    SDL_RenderClear(g_screen);
    g_background.render(g_screen, NULL);

    Map mapData = tileMap.getMap();

    player.handleBullet(g_screen);
    player.setMapXY(mapData.startX, mapData.startY);
    player.movePlayer(mapData);

    player.show(g_screen);

    tileMap.setMap(mapData);
    tileMap.drawMap(g_screen);

    // Draw geometric
    GeometricFormat rectangleSize(0, 0, SCREEN_WIDTH, 40);
    ColorData colorData(36, 36, 36);
    Geometric::renderRectangle(rectangleSize, colorData, g_screen);

    GeometricFormat outlineSize(1, 1, SCREEN_WIDTH - 1, 38);
    ColorData outlineColor(255, 255, 255);
    Geometric::renderOutline(outlineSize, outlineColor, g_screen);

    playerLife.show(g_screen);
    playerMoney.show(g_screen);

    for (int i = 0; i < listThreat.size(); i++)
    {
      ThreatObject *threatInstance = listThreat.at(i);
      if (threatInstance != NULL)
      {
        threatInstance->setMapXY(mapData.startX, mapData.startY);
        threatInstance->loadMoveType(g_screen);
        threatInstance->moveThreat(mapData);
        threatInstance->handleBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);
        threatInstance->show(g_screen);

        // Check Player có bị đạn của Threat bắn trúng không
        SDL_Rect rectPlayer = player.getRectFrame();
        bool bulletCollision = false;
        vector<Bullet *> threatBullet = threatInstance->getBulletList();
        for (int j = 0; j < threatBullet.size(); j++)
        {
          Bullet *bulletInstance = threatBullet.at(j);
          if (bulletInstance != NULL)
          {
            bulletCollision = SDLCommonFunc::checkCollison(bulletInstance->getRect(), rectPlayer);
            if (bulletCollision) // Player bị bắn trúng
            {
              threatInstance->removeBullet(j);
              break;
            }
          }
        }

        // Check Player có va chạm với Threat không
        SDL_Rect rectThreat = threatInstance->getRectFrame();
        bool threatCollision = SDLCommonFunc::checkCollison(rectPlayer, rectThreat);
        if (bulletCollision || threatCollision)
        {
          for (int i = 0; i < NUM_FRAME_EXP; i++)
          {
            int posX = (player.getRect().x + player.getFrameWidth() * 0.5) - frameExpWidth * 0.5;
            int posY = (player.getRect().y + player.getFrameHeight() * 0.5) - frameExpHeight * 0.5;
            threatExplo.setFrame(i);
            threatExplo.setRect(posX, posY);
            threatExplo.show(g_screen);
            SDL_RenderPresent(g_screen);
          }
          // Tăng số lần chết và tính xem game over chưa
          numDie++;
          if (numDie <= 3)
          {
            player.setRect(0, 0);
            player.setComebackTime(60);
            SDL_Delay(1000);
            playerLife.decreaseLife();
            playerLife.render(g_screen);
            continue;
          }
          else
          {
            if (MessageBox(NULL, _T("GAME OVER"), _T("Info"), MB_OK | MB_ICONSTOP) == IDOK)
            {
              threatInstance->free();
              close();
              return 0;
            }
          }
        }
      }
    }

    vector<Bullet *> playerBullet = player.getBulletList();
    for (int r = 0; r < playerBullet.size(); r++)
    {
      Bullet *instance = playerBullet.at(r);
      // Check từng viên đạn với từng Threat xem chúng có va chạm với nhau hay không
      if (instance != NULL)
      {
        for (int t = 0; t < listThreat.size(); t++)
        {
          ThreatObject *threat = listThreat.at(t);
          if (threat != NULL)
          {
            // Rect của Threat
            SDL_Rect tRect = threat->getRectFrame();

            SDL_Rect bRect = instance->getRect();

            bool bCol = SDLCommonFunc::checkCollison(bRect, tRect);
            if (bCol) // Nếu đạn có va chạm với Threat
            {
              pointValue++;
              for (int ex = 0; ex < NUM_FRAME_EXP; ex++)
              {
                int posX = instance->getRect().x - frameExpWidth * 0.5;
                int posY = instance->getRect().y - frameExpHeight * 0.5;

                threatExplo.setFrame(ex);
                threatExplo.setRect(posX, posY);
                threatExplo.show(g_screen);
              }
              player.removeBullet(r);                   // Xóa viên đạn
              threat->free();                           // Xóa Threat
              listThreat.erase(listThreat.begin() + t); // Xoá Threat khỏi list
            }
          }
        }
      }
    }

    // Show game time
    string strTime = "Time: ";
    Uint32 timeValue = SDL_GetTicks() / 1000;
    Uint32 timeRemain = 300 - timeValue;
    if (timeRemain <= 0)
    {
      if (MessageBox(NULL, _T("GAME OVER"), _T("Info"), MB_OK | MB_ICONSTOP) == IDOK)
      {
        isQuit = true;
        break;
      }
    }
    else
    {
      string strValue = to_string(timeRemain);
      strTime += strValue;

      timeGame.setContent(strTime);
      timeGame.loadFromRenderText(fontTime, g_screen);
      timeGame.renderText(g_screen, SCREEN_WIDTH - 200, 15);
    }

    // Show point text
    string strPointValue = to_string(pointValue);
    string strPoint = "Point: ";
    strPoint += strPointValue;
    pointText.setContent(strPoint);
    pointText.loadFromRenderText(fontTime, g_screen);
    pointText.renderText(g_screen, SCREEN_WIDTH * 0.5 - 50, 15);

    // Show coin text
    int coinValue = player.getCoinBadge();
    string strCoinValue = to_string(coinValue);
    coinText.setContent(strCoinValue);
    coinText.loadFromRenderText(fontTime, g_screen);
    coinText.renderText(g_screen, SCREEN_WIDTH * 0.5 - 250, 15);

    // Show boss
    int val = MAX_MAP_X * TILE_SIZE - (mapData.startX + player.getRect().x);
    // // Chỉ khi player đến đủ gần thì boss mới đc render
    if (val <= SCREEN_WIDTH)
    {
      bossObject.setMapXY(mapData.startX, mapData.startY);
      bossObject.moveBoss(mapData);
      bossObject.handleBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);
      bossObject.show(g_screen);
    }

    SDL_RenderPresent(g_screen);

    int realTime = timerFPS.getTick();
    int timePerFrame = 1000 / FPS; // ms

    // Nếu time in game chạy nhanh hơn time quy định của số frame trong 1s của game, delay chương trình để đảm bảo FPS đúng
    if (realTime < timePerFrame)
    {
      int delayTime = timePerFrame - realTime;
      SDL_Delay(delayTime);
    }
  }

  for (int i = 0; i < listThreat.size(); i++)
  {
    ThreatObject *instance = listThreat.at(i);
    if (instance != NULL)
    {
      instance->free();
      instance = NULL;
    }
  }
  listThreat.clear();
  close();
  return 0;
}