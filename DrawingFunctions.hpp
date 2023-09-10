#pragma once

#include <SDL.h>

const SDL_Color White = {0xff, 0xff, 0xff};
const SDL_Color Black = {0x00, 0x00, 0x00};

bool IsSquareOfGivenColor(int x, int y, int Size, SDL_Surface *MySurface, int WindowWidth, SDL_Color Color);
void DrawSquare(int x, int y, int Size, SDL_Surface *MySurface, int WindowWidth, SDL_Color Color);
