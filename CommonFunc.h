#ifndef COMMON_FUNCTION_H
#define COMMON_FUNCTION_H

#include <tchar.h>
#include <Windows.h>
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

using namespace std;

static SDL_Window *g_window = NULL;
static SDL_Renderer *g_screen = NULL;
static SDL_Event g_event;

// Screen
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 640;
const int SCREEN_BPP = 32; // Bit by pixel

const int COLOR_KEY_R = 167;
const int COLOR_KEY_G = 175;
const int COLOR_KEY_B = 180;
const int RENDER_DRAW_COLOR = 0xff;

#define TILE_SIZE 64 // Kích thước của mỗi Tile
// Số lượng tile mỗi chiều
#define MAX_MAP_X 400
#define MAX_MAP_Y 10

struct Map
{
  // Tọa độ của các tile đầu tiên trên window
  int startX;
  int startY;
  // Tọa độ của các tile cuối cùng trên map
  int maxX;
  int maxY;

  int tiles[MAX_MAP_Y][MAX_MAP_X]; // Array tổng lưu thông tin các tile
  char *fileName;                  // File chứa array
};

struct Input
{
  int up;
  int down;
  int left;
  int right;
  int jump;
};

namespace SDLCommonFunc
{
  bool checkCollison(const SDL_Rect &object1, const SDL_Rect &object2);
}
#endif