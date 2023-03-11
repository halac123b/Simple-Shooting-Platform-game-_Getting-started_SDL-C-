#ifndef PLAYER_H
#define PLAYER_H

#include "CommonFunc.h"
#include "BaseObject.h"
#include "Bullet.h"

#define GRAVITY_ACCLERATION 1
#define MAX_FALL_SPEED 10
#define PLAYER_SPEED 8
#define PLAYER_JUMP_SPEED -15

class Player : public BaseObject
{
public:
  Player();
  ~Player() {}

  enum WalkType
  {
    WALK_NONE = 0,
    WALK_RIGHT = 1,
    WALK_LEFT = 2
  };

  // Override lại hàm của BaseObject, vì ảnh của sprite Player gồm nhiều frame
  bool loadImg(string path, SDL_Renderer *screen);
  void show(SDL_Renderer *des);
  void handleInputAction(SDL_Event event, SDL_Renderer *screen);
  void clipFrame();                                // Cắt sprtie thành các frame
  void movePlayer(Map &mapData);                   // Handle việc di chuyển của player
  void checkTouchMap(Map &mapData);                // Check player có chạm với địa hình hay không
  void setMapXY(const int _mapX, const int _mapY); // Set tọa độ của window đang hiển thị phần nào của map
  void centerPlayerOnMap(Map &mapData);            // Chỉnh vị trí map để player nằm ở giữa
  void updateImage(SDL_Renderer *screen);          // Update image mỗi khi player thay đổi trạng thái, hướng di chuyển
  int getPosY() { return posY; }
  int getFrameWidth() { return frameWidth; }
  int getFrameHeight() { return frameHeight; }
  void setComebackTime(int time) { comebackTime = time; }

  void setBulletList(vector<Bullet *> list) { bulletList = list; }
  vector<Bullet *> getBulletList() { return bulletList; }
  void handleBullet(SDL_Renderer *screen);
  void increaseCoin();
  int getCoinBadge() { return coinBadge; }
  void removeBullet(int index); // Xóa 1 viên đạn
  SDL_Rect getRectFrame();      // Lấy rect của frame hiện tại

private:
  vector<Bullet *> bulletList;
  int velX, velY; // Vận tốc di chuyển
  int posX, posY; // Tọa độ trên window

  int frameWidth, frameHeight; // Kích thước các frame sprite

  SDL_Rect frameClip[8]; // Array chứa các frame
  Input inputType;
  int frame;     // Index của frame hiện tại
  int status;    // Gồm 2 bộ sprite khác nhau khi player di chuyển sang phải, trái
  bool onGround; // Player có đang đứng trên mặt đất không
  // Vị trí của map để di chuyển theo khi player di chuyển
  int mapX;
  int mapY;
  int comebackTime; // Thời gian player respawn sau khi rơi xuống vực
  int coinBadge;    // Số coin nhận được
};
#endif