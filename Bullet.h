#ifndef BULLET_H
#define BULLET_H

#include "CommonFunc.h"
#include "BaseObject.h"

class Bullet : public BaseObject
{
public:
  Bullet();
  enum BulletDirection // Ở đây có định nghĩa nhiều hướng băn bắn nhưng chỉ mới hiện thực 2 hướng trái, phải, các hướng khác cần catch vị trí chuột để chia trường hợp
  {
    DIR_LEFT = 20,
    DIR_RIGHT = 21,
    DIR_UP = 22,
    DIR_UP_LEFT = 23,
    DIR_UP_RIGHT = 24,
    DIR_DOWN_LEFT = 25,
    DIR_DOWN_RIGHT = 26
  };

  enum typeBullet
  {
    SPHERE_BULLET = 50,
    LASER_BULLET = 51
  };
  void setVelX(const int _velX) { velX = _velX; }
  void setVelY(const int _velY) { velY = _velY; }
  int getVelX() const { return velX; }
  int getVelY() const { return velY; }
  void setIsMove(const bool _isMove) { isMove = _isMove; }
  bool getIsMove() const { return isMove; }
  void setDirection(int _direction) { direction = _direction; }
  int getDirection() { return direction; }
  void setBulletType(int type) { bulletType = type; }
  int getBulletType() { return bulletType; }
  void handleMove(int borderX, int borderY);
  bool loadBulletImg(SDL_Renderer *screen);

private:
  // Vận tốc di chuyển của đạn
  int velX;
  int velY;
  bool isMove;    // Check đạn có đang đc bắn ra và di chuyển trong màn hình hay không
  int direction;  // Hướng viên đạn đc bắn ra
  int bulletType; // loại đạn đc bắn ra
};

#endif