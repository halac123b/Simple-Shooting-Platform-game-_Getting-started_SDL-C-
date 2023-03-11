#ifndef TIMER_H
#define TIMER_H

#include "CommonFunc.h"

class Timer
{
public:
  Timer();
  ~Timer() {}

  void start();
  void stop();
  void pause();
  void resume();

  int getTick(); // Get thời play ingame kể từ khi start
  bool isGameStarted();
  // bool isPaused();

private:
  // Đếm thời gian từ lúc SDL được khởi tạo cho tới game đc start/resume, trừ khoảng pause ra
  int startTick;
  bool isStarted;

  int pauseTick; // Thời điểm game pause so với thời điểm game bắt đầu
  bool isPaused;
};
#endif