#include "ThreatObject.h"
#include "TileMap.h"

ThreatObject::ThreatObject()
{
  frameWidth = 0;
  frameHeight = 0;
  velX = velY = 0.0;
  posX = posY = 0.0;
  onGround = false;
  comebackTime = 0;
  frame = 0;
  animationA = animationB = 0;
  inputType.left = 0;
  moveType = STATIC_THREAT;
}

bool ThreatObject::loadImg(string path, SDL_Renderer *screen)
{
  bool ret = BaseObject::loadImg(path, screen);
  if (ret)
  {
    frameWidth = rect.w / THREAT_FRAME_NUM; // 1 sprite lớn gồm 8 sprite nhỏ
    frameHeight = rect.h;
  }
  return ret;
}

void ThreatObject::clipFrame()
{
  if (frameWidth > 0 && frameHeight > 0)
  {
    for (int i = 0; i < THREAT_FRAME_NUM; i++)
    {
      frameClip[i].x = i * frameWidth;
      frameClip[i].y = 0;
      frameClip[i].w = frameWidth;
      frameClip[i].h = frameHeight;
    }
  }
}

void ThreatObject::show(SDL_Renderer *screen)
{
  if (comebackTime == 0)
  {
    rect.x = posX - mapX;
    rect.y = posY - mapY;
    frame++;
    if (frame >= THREAT_FRAME_NUM)
    {
      frame = 0;
    }
    SDL_Rect *currentClip = &frameClip[frame];
    SDL_Rect renderQuad = {rect.x, rect.y, frameWidth, frameHeight};
    SDL_RenderCopy(screen, p_object, currentClip, &renderQuad);
  }
}

void ThreatObject::moveThreat(Map &mapData)
{
  if (comebackTime == 0)
  {
    velX = 0;
    velY += THREAT_GRAVITY_SPEED;
    if (velY >= THREAT_MAX_FALL_SPEED)
    {
      velY = THREAT_MAX_FALL_SPEED;
    }
    if (inputType.left == 1)
    {
      velX = -THREAT_SPEED;
    }
    else if (inputType.right == 1)
    {
      velX = THREAT_SPEED;
    }
    checkTouchMap(mapData);
  }
  else if (comebackTime > 0)
  {
    comebackTime--;
    if (comebackTime == 0)
    {
      respawnThreat();
    }
  }
}

// Copy từ hàm của Player
void ThreatObject::checkTouchMap(Map &mapData)
{
  int x1 = 0, x2 = 0;
  int y1 = 0, y2 = 0;

  // Check theo chiều ngang (horizontal)
  int heightMin = frameHeight < TILE_SIZE ? frameHeight : TILE_SIZE;

  x1 = (posX + velX) / TILE_SIZE;
  x2 = (posX + velX + frameWidth - 1) / TILE_SIZE; // Điểm cuối, đặt sai số 1
  y1 = posY / TILE_SIZE;
  y2 = (posY + heightMin - 2) / TILE_SIZE;

  if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
  {
    if (velX > 0) // Player go forward
    {
      int val1 = mapData.tiles[y1][x2];
      int val2 = mapData.tiles[y2][x2];
      // Check ô tiếp theo di chuyển tới là bức tường
      if ((val1 != 0 && val1 != COIN_INDEX) || (val2 != 0 && val2 != COIN_INDEX))
      {
        posX = x2 * TILE_SIZE - frameWidth + 1; // Giới hạn vị trí ở chân tường
        velX = 0;
      }
    }
    else if (velX < 0)
    {
      int val1 = mapData.tiles[y1][x1];
      int val2 = mapData.tiles[y2][x1];

      if ((val1 != 0 && val1 != COIN_INDEX) || (val2 != 0 && val2 != COIN_INDEX))
      {
        posX = (x1 + 1) * TILE_SIZE;
        velX = 0;
      }
    }
  }
  if (posY > mapData.maxY)
  {
    comebackTime = 60;
  }

  // Check theo chiều dọc (vertical)
  int widthMin = frameWidth < TILE_SIZE ? frameWidth : TILE_SIZE;
  x1 = posX / TILE_SIZE;
  x2 = (posX + widthMin - 1) / TILE_SIZE;
  y1 = (posY + velY) / TILE_SIZE;
  y2 = (posY + velY + frameHeight - 1) / TILE_SIZE;

  if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
  {
    if (velY > 0)
    { // Player rơi từ trên xuống
      int val1 = mapData.tiles[y2][x1];
      int val2 = mapData.tiles[y2][x2];

      if ((val1 != 0 && val1 != COIN_INDEX) || (val2 != 0 && val2 != COIN_INDEX))
      {
        posY = y2 * TILE_SIZE - frameHeight + 1;
        velY = 0;
        onGround = true;
      }
    }
    else if (velY < 0)
    {
      int val1 = mapData.tiles[y1][x1];
      int val2 = mapData.tiles[y1][x2];
      if ((val1 != 0 && val1 != COIN_INDEX) || (val2 != 0 && val2 != COIN_INDEX))
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
}

void ThreatObject::loadMoveType(SDL_Renderer *screen)
{
  if (moveType == MOVE_IN_SPACE)
  {
    if (onGround == true)
    {
      if (posX > animationB)
      {
        inputType.left = 1;
        inputType.right = 0;
        loadImg("img/threat_left.png", screen);
      }
      else if (posX < animationA)
      {
        inputType.left = 0;
        inputType.right = 1;
        loadImg("img/threat_right.png", screen);
      }
    }
    else
    {
      if (inputType.left == 1)
      {
        loadImg("img/threat_left.png", screen);
      }
    }
  }
}

void ThreatObject::respawnThreat()
{
  velX = 0;
  velY = 0;
  if (posX > 256)
  {
    posX -= 256;
    animationA -= 256;
    animationB -= 256;
  }
  else
  {
    posX = 0;
  }
  posY = 0;
  inputType.left = 1;
}

void ThreatObject::initBullet(Bullet *bullet, SDL_Renderer *screen)
{
  if (bullet != NULL)
  {
    bullet->setBulletType(Bullet::LASER_BULLET);
    bool ret = bullet->loadBulletImg(screen);
    if (ret)
    {
      bullet->setIsMove(true);
      bullet->setDirection(Bullet::DIR_LEFT); // Threat luôn bắn về phía Player
      bullet->setRect(rect.x + 5, rect.y + 5);
      bullet->setVelX(15);
      bulletList.push_back(bullet);
    }
  }
}
void ThreatObject::handleBullet(SDL_Renderer *screen, int borderX, int borderY)
{
  for (int i = 0; i < bulletList.size(); i++)
  {
    Bullet *instance = bulletList.at(i);
    if (instance != NULL)
    {
      if (instance->getIsMove())
      {
        int bulletDistance = rect.x + frameWidth - instance->getRect().x;
        if (bulletDistance < 400 && bulletDistance > 0)
        {
          instance->handleMove(borderX, borderY);
          instance->render(screen);
        }
        else
        {
          instance->setIsMove(false);
        }
      }
      else
      { // Threat sẽ chỉ có 1 bullet bắn đi bắn lại, nếu đạn ra ngoài giới hạn màn hình sẽ reset như bắn 1 viên khác
        instance->setIsMove(true);
        instance->setRect(rect.x + 5, rect.y + 5);
      }
    }
  }
}

void ThreatObject::removeBullet(int index)
{
  int size = bulletList.size();
  if (size > 0 && index < size)
  {
    Bullet *temp = bulletList.at(index);
    bulletList.erase(bulletList.begin() + index);
    if (temp != NULL)
    {
      delete temp;
      temp = NULL;
    }
  }
}

SDL_Rect ThreatObject::getRectFrame()
{
  SDL_Rect ans;
  ans.x = rect.x;
  ans.y = rect.y;
  ans.w = frameWidth;
  ans.h = frameHeight;
  return ans;
}