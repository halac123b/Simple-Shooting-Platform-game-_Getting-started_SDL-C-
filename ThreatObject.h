#ifndef THREAT_OBJECT_H
#define THREAT_OBJECT_H

#include "CommonFunc.h"
#include "BaseObject.h"
#include "Bullet.h"

#define THREAT_FRAME_NUM 8 // Số lượng frame sprite của threat
#define THREAT_GRAVITY_SPEED 0.8
#define THREAT_MAX_FALL_SPEED 10
#define THREAT_SPEED 3
class ThreatObject : public BaseObject
{
public:
  ThreatObject();
  enum MoveType
  {
    STATIC_THREAT = 0, // Đứng im
    MOVE_IN_SPACE = 1  // Di chuyển qua lại trong 1 khu vực
  };
  void setVelX(float _velX) { velX = _velX; }
  void setVelY(float _velY) { velY = _velY; }
  void setPosX(float _posX) { posX = _posX; }
  void setPosY(float _posY) { posY = _posY; }
  float getPosX() { return posX; }
  float getPosY() { return posY; }
  int getFrameWidth() { return frameWidth; }
  int getFrameHeight() { return frameHeight; }
  void setMapXY(int _mapX, int _mapY)
  {
    mapX = _mapX;
    mapY = _mapY;
  }
  void setMoveType(int _moveType) { moveType = _moveType; }
  void setAnimationPos(int posA, int posB)
  {
    animationA = posA;
    animationB = posB;
  }
  void setInputLeft(int inputLeft) { inputType.left = inputLeft; }
  vector<Bullet *> getBulletList() { return bulletList; }
  void setBulletList(vector<Bullet *> list) { bulletList = list; }

  void loadMoveType(SDL_Renderer *screen);
  void clipFrame();
  bool loadImg(string path, SDL_Renderer *screen);
  void show(SDL_Renderer *screen);
  void moveThreat(Map &mapData);
  void checkTouchMap(Map &mapData);
  void respawnThreat();                                  // Respawn nếu threat rơi xuống vực
  void initBullet(Bullet *bullet, SDL_Renderer *screen); // Khởi tạo 1 viên đạn mới
  void handleBullet(SDL_Renderer *screen, int borderX, int borderY);
  void removeBullet(int index);
  SDL_Rect getRectFrame(); // Lấy rect của frame hiện tại

private:
  // Kích thước của sprtie tương tự như Player
  int frameWidth;
  int frameHeight;
  SDL_Rect frameClip[THREAT_FRAME_NUM]; // Array chứa các frame
  int frame;                            // Index của frame hiện tại
  int comebackTime;                     // Thời gian respawn sau khi rơi xuống vực
  float posX;
  float posY;
  bool onGround;
  float velX;
  float velY;
  // Vị trí của map tương ứng với Threat
  int mapX;
  int mapY;
  int moveType; // Hành vi sẽ di chuyển như thế nào
  // Tọa độ để threat đổi hướng di chuyển
  int animationA; // Phải -> trái
  int animationB; // Trái -> phải
  Input inputType;
  vector<Bullet *> bulletList;
};
#endif