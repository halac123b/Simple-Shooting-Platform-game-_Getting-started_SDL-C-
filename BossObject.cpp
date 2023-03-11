#include "BossObject.h"

BossObject::BossObject()
{
  frame = 0;
  velX = velY = 0.0;
  posX = posY = 0;
  frameWidth = frameHeight = 0;
  thinkTime = 0;
  mapX = mapY = 0;
  initBoss();
}

bool BossObject::loadImg(string path, SDL_Renderer *screen)
{
  bool ret = BaseObject::loadImg(path, screen);
  if (ret == true)
  {
    frameWidth = rect.w / BOSS_FRAME_NUM; // Chia làm 32 frame từ sprite lớn
    frameHeight = rect.h;
  }
  return ret;
}

void BossObject::clipFrame()
{
  if (frameWidth > 0 && frameHeight > 0)
  {
    for (int i = 0; i < BOSS_FRAME_NUM; i++)
    {
      frameClip[i].x = i * frameWidth;
      frameClip[i].y = 0;
      frameClip[i].w = frameWidth;
      frameClip[i].h = frameHeight;
    }
  }
}

void BossObject::show(SDL_Renderer *screen)
{
  if (thinkTime == 0)
  {
    rect.x = posX - mapX;
    rect.y = posY - mapY;
    frame++;
    if (frame >= 32)
    {
      frame = 0;
    }
    SDL_Rect *currentClip = &frameClip[frame];
    SDL_Rect renderQuad = {rect.x, rect.y, frameWidth, frameHeight};
    if (currentClip != NULL)
    {
      renderQuad.w = currentClip->w;
      renderQuad.h = currentClip->h;
    }
    SDL_RenderCopy(screen, p_object, currentClip, &renderQuad);
  }
}

void BossObject::moveBoss(Map &mapData)
{
  if (thinkTime == 0)
  {
    velX = 0;
    velY += GRAVITY_SPEED;
    if (velY >= MAX_FALL_SPEED)
    {
      velY = MAX_FALL_SPEED;
    }
    if (inputType.left == 1)
    {
      velX -= PLAYER_SPPED;
    }
    else if (inputType.right == 1)
    {
      velX += PLAYER_SPPED;
    }
    if (inputType.jump == 1)
    {
      if (onGround == 1)
      {
        velY = -PLAYER_HIGH_VAL;
      }
      inputType.jump = 0;
    }
    checkTouchMap(mapData);
  }
  if (thinkTime > 0)
  {
    thinkTime--;
    if (thinkTime == 0)
    {
      initBoss();
    }
  }
}

void BossObject::initBoss()
{
  velX = velY = 0;
  if (posX > 256)
  {
    posX -= 256;
  }
  else
  {
    posX = 0;
  }
  posY = 0;
  thinkTime = 0;
  inputType.left = 1;
}

// Copy từ player
void BossObject::checkTouchMap(Map &mapData)
{
  int x1 = 0, x2 = 0;
  int y1 = 0, y2 = 0;

  // Check theo chiều ngang (horizontal)
  int heightMin = frameHeight;

  x1 = (posX + velX) / TILE_SIZE;
  x2 = (posX + velX + frameWidth - 1) / TILE_SIZE; // Điểm cuối, đặt sai số 1
  y1 = posY / TILE_SIZE;
  y2 = (posY + heightMin - 2) / TILE_SIZE;

  //  Chỉ check khi player còn đang nằm trong index hợp lệ của map
  if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
  {
    if (velX > 0) // Player go forward
    {
      int val1 = mapData.tiles[y1][x2];
      int val2 = mapData.tiles[y2][x2];

      // Check ô tiếp theo di chuyển tới là bức tường
      if (val1 != 0 || val2 != 0)
      {
        posX = x2 * TILE_SIZE - frameWidth - 1; // Giới hạn vị trí ở chân tường
        velX = 0;
      }
    }
    else if (velX < 0)
    {
      int val1 = mapData.tiles[y1][x1];
      int val2 = mapData.tiles[y2][x1];
      if (val1 != 0 || val2 != 0)
      {
        posX = (x1 + 1) * TILE_SIZE;
        velX = 0;
      }
    }
  }

  // Check theo chiều dọc (vertical)
  int widthMin = frameWidth;
  x1 = posX / TILE_SIZE;
  x2 = (posX + widthMin) / TILE_SIZE;
  y1 = (posY + velY) / TILE_SIZE;
  y2 = (posY + velY + frameHeight) / TILE_SIZE;

  if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
  {
    if (velY > 0)
    { // Player rơi từ trên xuống
      int val1 = mapData.tiles[y2][x1];
      int val2 = mapData.tiles[y2][x2];
      if (val1 != 0 || val2 != 0)
      {
        posY = y2 * TILE_SIZE - frameHeight;
        velY = 0;
        onGround = 1;
      }
    }
    else if (velY < 0)
    {
      int val1 = mapData.tiles[y1][x1];
      int val2 = mapData.tiles[y1][x2];
      if (val1 != 0 || val2 != 0)
      {
        posY = (y1 + 1) * TILE_SIZE;
        velY = 0;
      }
    }
  }
  posX += velX;
  posY += velY;
  if (posX < 0)
  {
    posX = 0;
  }
  else if (posX + frameWidth > mapData.maxX)
  {
    posX = mapData.maxX - frameWidth - 1;
  }
  if (posY > mapData.maxY)
  {
    thinkTime = 60;
  }
}

void BossObject::initBullet(SDL_Renderer *screen)
{
  Bullet *bullet = new Bullet;
  bool ret = bullet->loadImg("img/boss bullet.png", screen);
  if (ret)
  {
    bullet->setDirection(Bullet::DIR_LEFT);
    bullet->setIsMove(true);
    bullet->setRect(rect.x - 50, rect.y + frameHeight - 30);
    bullet->setVelX(15);
    bulletList.push_back(bullet);
  }
}

void BossObject::handleBullet(SDL_Renderer *screen, int borderX, int borderY)
{
  if (frame == 18)
  {
    initBullet(screen);
  }
  for (int i = 0; i < bulletList.size(); i++)
  {
    Bullet *instance = bulletList.at(i);
    if (instance != NULL)
    {
      if (instance->getIsMove())
      {
        instance->handleMove(borderX, borderY);
        instance->render(screen);
      }
      else
      {
        instance->free();
        bulletList.erase(bulletList.begin() + i);
      }
    }
  }
}