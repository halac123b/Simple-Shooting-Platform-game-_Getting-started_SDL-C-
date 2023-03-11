#ifndef TEXT_OBJECT_H
#define TEXT_OBJECT_H

#include "CommonFunc.h"

class TextObject
{
public:
  TextObject();
  ~TextObject();
  enum TextColor
  {
    RED = 1,
    WHITE = 2,
    BLACK = 3
  };

  bool loadFromRenderText(TTF_Font *font, SDL_Renderer *screen);
  void free();

  void setTextColor(Uint8 red, Uint8 green, Uint8 blue); // Set cụ thể màu RGB
  void setTextColor(int type);                           // Set theo các bôj màu cơ bản có  sẵn
  void renderText(SDL_Renderer *screen,
                  int posX, int posY, SDL_Rect *clip = NULL,
                  double angle = 0.0, SDL_Point *center = NULL,
                  SDL_RendererFlip flip = SDL_FLIP_NONE);
  int getWidth() { return width; }
  int getHeight() { return height; }
  void setContent(string text) { content = text; }
  string getContent() { return content; }

private:
  string content;
  SDL_Color textColor;
  SDL_Texture *texture;
  int width;
  int height;
};

#endif