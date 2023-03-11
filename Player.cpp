#include "Player.h"
#include <iostream>
#include "TileMap.h"

Player::Player()
{
  frame = 0;
  posX = 700;
  posY = 0;
  velX = velY = 0;
  frameWidth = frameHeight = 0;
  status = WALK_NONE;
  onGround = false;
  // Init các gt input về 0
  inputType.left = inputType.right = inputType.down = inputType.up = inputType.jump = 0;
  mapX = mapY = 0;
  comebackTime = 0;
  coinBadge = 0;
}

bool Player::loadImg(string path, SDL_Renderer *screen)
{
  bool ret = BaseObject::loadImg(path, screen);
  if (ret == true)
  {
    frameWidth = rect.w / 8; // Chia làm 8 frame từ sprite lớn
    frameHeight = rect.h;
  }
  return ret;
}

void Player::clipFrame()
{
  if (frameWidth > 0 && frameHeight > 0)
  {
    for (int i = 0; i < 8; i++)
    {
      frameClip[i].x = i * frameWidth;
      frameClip[i].y = 0;
      frameClip[i].w = frameWidth;
      frameClip[i].h = frameHeight;
    }
  }
}

void Player::show(SDL_Renderer *des)
{
  // Tuỳ vào hướng di chuyển mà phải load lại sprite khác
  updateImage(des);

  if (inputType.left == 1 || inputType.right == 1)
  {
    frame++;
  }
  else
  {
    frame = 0;
  }
  if (frame >= 8)
  {
    frame = 0;
  }

  // Không load ảnh khi player rơi xuống hố
  if (comebackTime == 0)
  {
    // Trừ đi phần mà map di chuyển theo
    rect.x = posX - mapX;
    rect.y = posY - mapY;

    SDL_Rect *currentFrame = &frameClip[frame];
    SDL_Rect renderQuad = {rect.x, rect.y, frameWidth, frameHeight};
    SDL_RenderCopy(des, p_object, currentFrame, &renderQuad);
  }
}

void Player::handleInputAction(SDL_Event event, SDL_Renderer *screen)
{
  if (event.type == SDL_KEYDOWN)
  {
    switch (event.key.keysym.sym)
    {
    case SDLK_RIGHT:
      status = WALK_RIGHT;
      inputType.right = 1;
      inputType.left = 0; // Tránh user nhấn 2 phím cùng lúc
      updateImage(screen);
      break;
    case SDLK_LEFT:
      status = WALK_LEFT;
      inputType.left = 1;
      inputType.right = 0;
      updateImage(screen);
      break;
    }
  }
  else if (event.type == SDL_KEYUP)
  {
    switch (event.key.keysym.sym)
    {
    case SDLK_RIGHT:
      inputType.right = 0;
      break;
    case SDLK_LEFT:
      inputType.left = 0;
      break;
    }
  }
  if (event.type == SDL_MOUSEBUTTONDOWN)
  {
    if (event.button.button == SDL_BUTTON_RIGHT)
    {
      inputType.jump = 1;
    }
    else if (event.button.button == SDL_BUTTON_LEFT) // Shooting bullet
    {
      // Tạo 1 bullet mới
      Bullet *newBullet = new Bullet;
      newBullet->setBulletType(Bullet::LASER_BULLET);
      newBullet->loadBulletImg(screen);
      if (status == WALK_LEFT)
      {
        newBullet->setDirection(Bullet::DIR_LEFT);
        newBullet->setRect(this->rect.x, rect.y + frameHeight * 0.25);
      }
      else
      {
        newBullet->setDirection(Bullet::DIR_RIGHT);
        newBullet->setRect(this->rect.x + frameWidth - 20, rect.y + frameHeight * 0.25);
      }
      newBullet->setVelX(20);
      newBullet->setIsMove(true);

      bulletList.push_back(newBullet);
    }
  }
}

void Player::movePlayer(Map &mapData)
{
  // Nếu player đang rơi xuống vực thì không di chuyển đc gì
  if (comebackTime == 0)
  {
    velX = 0;
    velY += GRAVITY_ACCLERATION;
    if (velY > MAX_FALL_SPEED)
    {
      velY = MAX_FALL_SPEED;
    }
    if (inputType.right == 1)
    {
      velX += PLAYER_SPEED;
    }
    else if (inputType.left == 1)
    {
      velX -= PLAYER_SPEED;
    }

    if (inputType.jump == 1)
    {
      if (onGround)
      {
        velY = PLAYER_JUMP_SPEED;
        onGround = false;
      }
      inputType.jump = 0;
    }

    checkTouchMap(mapData);
    centerPlayerOnMap(mapData);
  }
  if (comebackTime > 0)
  {
    comebackTime--;
    if (comebackTime <= 0) // Respawn lại player trên không
    {
      posY = 0;
      velX = 0;
      velY = 0;
      onGround = false;
      // Lùi lại 1 tí so với vị trí lọt hố
      if (posX > 256)
      {
        posX -= 256;
      }
    }
  }
}

void Player::checkTouchMap(Map &mapData)
{
  // Index của tile mà player sẽ di chuyển tới (phải check trước khi player di chuyển)
  int x1 = 0, x2 = 0;
  int y1 = 0, y2 = 0;

  // Check theo chiều ngang (horizontal)
  int heightMin = frameHeight < TILE_SIZE ? frameHeight : TILE_SIZE;

  x1 = (posX + velX) / TILE_SIZE;                  // Điểm đầu của player
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

      if (val1 == COIN_INDEX || val2 == COIN_INDEX)
      {
        mapData.tiles[y1][x2] = 0;
        mapData.tiles[y2][x2] = 0;
        increaseCoin();
      }
      else
      {
        // Check ô tiếp theo di chuyển tới là bức tường
        if (val1 != 0 || val2 != 0)
        {
          posX = x2 * TILE_SIZE - frameWidth + 1; // Giới hạn vị trí ở chân tường
          velX = 0;
        }
      }
    }
    else if (velX < 0)
    {
      int val1 = mapData.tiles[y1][x1];
      int val2 = mapData.tiles[y2][x1];
      if (val1 == COIN_INDEX || val2 == COIN_INDEX)
      {
        mapData.tiles[y1][x1] = 0;
        mapData.tiles[y2][x1] = 0;
        increaseCoin();
      }
      else
      {
        if (val1 != 0 || val2 != 0)
        {
          posX = (x1 + 1) * TILE_SIZE;
          velX = 0;
        }
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
      if (val1 == COIN_INDEX || val2 == COIN_INDEX)
      {
        mapData.tiles[y2][x1] = 0;
        mapData.tiles[y2][x2] = 0;
        increaseCoin();
      }
      else
      {
        if (val1 != 0 || val2 != 0)
        {
          posY = y2 * TILE_SIZE - frameHeight + 1;
          velY = 0;
          onGround = true;

          if (status == WALK_NONE)
          {
            status = WALK_RIGHT;
          }
        }
      }
    }
    else if (velY < 0)
    {
      int val1 = mapData.tiles[y1][x1];
      int val2 = mapData.tiles[y1][x2];
      if (val1 == COIN_INDEX || val2 == COIN_INDEX)
      {
        mapData.tiles[y1][x1] = 0;
        mapData.tiles[y1][x2] = 0;
        increaseCoin();
      }
      else
      {
        if (val1 != 0 || val2 != 0)
        {
          posY = (y1 + 1) * TILE_SIZE;
          velY = 0;
        }
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

void Player::setMapXY(const int _mapX, const int _mapY)
{
  mapX = _mapX;
  mapY = _mapY;
}

void Player::centerPlayerOnMap(Map &mapData)
{
  mapData.startX = posX - (SCREEN_WIDTH / 2);
  if (mapData.startX < 0)
  {
    mapData.startX = 0;
  }
  else if (mapData.startX + SCREEN_WIDTH >= mapData.maxX)
  {
    mapData.startX = mapData.maxX - SCREEN_WIDTH;
  }

  mapData.startY = posY - (SCREEN_HEIGHT / 2);
  if (mapData.startY < 0)
  {
    mapData.startY = 0;
  }
  else if (mapData.startX + SCREEN_HEIGHT >= mapData.maxY)
  {
    mapData.startY = mapData.maxY - SCREEN_HEIGHT;
  }
}

void Player::updateImage(SDL_Renderer *screen)
{
  if (onGround)
  {
    if (status == WALK_LEFT)
    {
      loadImg("img/player_left.png", screen);
    }
    else
    {
      loadImg("img/player_right.png", screen);
    }
  }
  else
  {
    if (status == WALK_LEFT)
    {
      loadImg("img/jump_left.png", screen);
    }
    else
    {
      loadImg("img/jump_right.png", screen);
    }
  }
}

void Player::handleBullet(SDL_Renderer *screen)
{
  for (int i = 0; i < bulletList.size(); i++)
  {
    Bullet *temp = bulletList.at(i);
    if (temp != NULL)
    {
      if (temp->getIsMove() == true)
      {
        temp->handleMove(SCREEN_WIDTH, SCREEN_HEIGHT);
        temp->render(screen);
      }
      else
      {
        bulletList.erase(bulletList.begin() + i);
        if (temp != NULL)
        {
          delete temp;
          temp = NULL;
        }
      }
    }
  }
}

void Player::increaseCoin()
{
  coinBadge++;
}

void Player::removeBullet(int index)
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

SDL_Rect Player::getRectFrame()
{
  SDL_Rect ans;
  ans.x = rect.x;
  ans.y = rect.y;
  ans.w = frameWidth;
  ans.h = frameHeight;
  return ans;
}