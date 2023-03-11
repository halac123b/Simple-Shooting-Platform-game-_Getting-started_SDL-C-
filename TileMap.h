#ifndef TILE_MAP_H
#define TILE_MAP_H

#include "CommonFunc.h"
#include "BaseObject.h"

#define MAX_TILE 10  // Số loại sprite tile có trong asset
#define COIN_INDEX 4 // Index của tile Coin

// Object ô tile nhỏ trong map
class Tile : public BaseObject
{
public:
  Tile() {}
  ~Tile() {}
};

class TileMap
{
public:
  TileMap() {}
  ~TileMap() {}
  void loadMap(char *file);            // Load data của map từ file array
  void loadTile(SDL_Renderer *screen); // Load các file sprite tương ứng với từng tile
  void drawMap(SDL_Renderer *screen);
  Map getMap() const { return map; }
  void setMap(Map &mapData) { map = mapData; }

private:
  Map map;
  Tile tiles[MAX_TILE];
};
#endif