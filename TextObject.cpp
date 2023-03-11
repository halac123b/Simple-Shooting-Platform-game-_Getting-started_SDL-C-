#include "TextObject.h"

TextObject::TextObject()
{
  textColor.r = 255;
  textColor.g = 255;
  textColor.b = 255;
}

TextObject::~TextObject()
{
  free();
}

bool TextObject::loadFromRenderText(TTF_Font *font, SDL_Renderer *screen)
{
  SDL_Surface *textSurface = TTF_RenderText_Solid(font, content.c_str(), textColor);
  if (textSurface)
  {
    texture = SDL_CreateTextureFromSurface(screen, textSurface);
    width = textSurface->w;
    height = textSurface->h;
    SDL_FreeSurface(textSurface);
  }
  return texture != NULL;
}

void TextObject::free()
{
  if (texture != NULL)
  {
    SDL_DestroyTexture(texture);
    texture = NULL;
  }
}

void TextObject::setTextColor(Uint8 red, Uint8 green, Uint8 blue)
{
  textColor.r = red;
  textColor.g = green;
  textColor.b = blue;
}

void TextObject::setTextColor(int type)
{
  if (type == RED)
  {
    SDL_Color color = {255, 0, 0};
    textColor = color;
  }
  else if (type == WHITE)
  {
    SDL_Color color = {255, 255, 255};
    textColor = color;
  }
  else if (type == BLACK)
  {
    SDL_Color color = {0, 0, 0};
    textColor = color;
  }
}

void TextObject::renderText(SDL_Renderer *screen,
                            int posX, int posY, SDL_Rect *clip,
                            double angle, SDL_Point *center,
                            SDL_RendererFlip flip)
{
  SDL_Rect renderQuad = {posX, posY, width, height};
  if (clip != NULL)
  {
    renderQuad.w = clip->w;
    renderQuad.h = clip->h;
  }
  SDL_RenderCopyEx(screen, texture, clip, &renderQuad, angle, center, flip);
}
