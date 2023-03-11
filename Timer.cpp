#include "Timer.h"

Timer::Timer()
{
  startTick = 0;
  pauseTick = 0;
  isStarted = false;
  isPaused = false;
}

void Timer::start()
{
  isStarted = true;
  isPaused = false;
  startTick = SDL_GetTicks();
}

void Timer::stop()
{
  isStarted = false;
  isPaused = false;
}

void Timer::pause()
{
  if (isStarted == true && isPaused == false)
  {
    isPaused = true;
    pauseTick = SDL_GetTicks() - startTick;
  }
}

void Timer::resume()
{
  if (isPaused == true)
  {
    isPaused = false;
    startTick = SDL_GetTicks() - pauseTick;
    pauseTick = 0;
  }
}

int Timer::getTick()
{
  if (isStarted == true)
  {
    if (isPaused == true)
    {
      return pauseTick;
    }
    else
    {
      return SDL_GetTicks() - startTick;
    }
  }
  return 0;
}

bool Timer::isGameStarted()
{
  return isStarted;
}