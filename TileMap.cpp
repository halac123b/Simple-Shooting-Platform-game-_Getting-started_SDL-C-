#include "TileMap.h"
#include <fstream>

using namespace std;

void TileMap::loadMap(char *file)
{
  fstream fp;
  fp.open(file, ios::in);
  if (!fp.is_open())
  {
    return;
  }
  map.maxX = 0;
  map.maxY = 0;

  for (int i = 0; i < MAX_MAP_Y; i++)
  {
    for (int j = 0; j < MAX_MAP_X; j++)
    {
      fp >> map.tiles[i][j];
      if (map.tiles[i][j] > 0)
      {
        // Nếu các index vượt qua giới hạn hiện tại, thì tăng giới hạn của map
        if (j > map.maxX)
        {
          map.maxX = j;
        }
        if (i > map.maxY)
        {
          map.maxY = i;
        }
      }
    }
  }
  // Nhân với kích thước tile để ra tọa độ
  map.maxX = (map.maxX + 1) * TILE_SIZE;
  map.maxY = (map.maxY + 1) * TILE_SIZE;

  map.startX = 0;
  map.startY = 0;
  map.fileName = file;

  fp.close();
}

void TileMap::loadTile(SDL_Renderer *screen)
{
  char fileImg[15];
  fstream fp;
  for (int i = 0; i < MAX_TILE; i++)
  {
    sprintf(fileImg, "map/%d.png", i);
    fp.open(fileImg, ios::in);
    if (!fp.is_open())
    {
      continue;
    }
    tiles[i].loadImg(fileImg, screen);
    fp.close();
  }
}

void TileMap::drawMap(SDL_Renderer *screen)
{
  int x1 = 0, x2 = 0;
  int y1 = 0, y2 = 0;
  int mapX = 0, mapY = 0; // Index của tile được render trong array tổng

  mapX = map.startX / TILE_SIZE;
  x1 = (map.startX % TILE_SIZE) * -1;
  x2 = x1 + SCREEN_WIDTH + (x1 == 0 ? 0 : TILE_SIZE);

  mapY = map.startY / TILE_SIZE;
  y1 = (map.startY % TILE_SIZE) * -1;
  y2 = y1 + SCREEN_HEIGHT + (y1 == 0 ? 0 : TILE_SIZE);
  for (int i = y1; i < y2; i += TILE_SIZE)
  {
    mapX = map.startX / TILE_SIZE; // Reset lại X sau mỗi hàng
    for (int j = x1; j < x2; j += TILE_SIZE)
    {
      int val = map.tiles[mapY][mapX];
      if (val > 0)
      {                            // Nếu val > 0 thì mới có image tương ứng
        tiles[val].setRect(j, i);  // Set tọa độ của tile đó theo i, j
        tiles[val].render(screen); // Render sprite của tile
      }
      mapX++;
    }
    mapY++;
  }
}