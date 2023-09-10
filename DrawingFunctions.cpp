#include "DrawingFunctions.hpp"

bool IsSquareOfGivenColor(int x, int y, int Size, SDL_Surface *MySurface, int WindowWidth, SDL_Color Color){
	//Here I am only checking the top-left pixel of the square
	uint8_t* PixelPtr = (uint8_t*)MySurface->pixels + (y * Size * WindowWidth + x * Size) * 4;
	if(*(PixelPtr + 2) == Color.r && *(PixelPtr + 1) == Color.g && *(PixelPtr) == Color.b)
    return true;
  return false;
}

void DrawSquare(int x, int y, int Size, SDL_Surface *MySurface, int WindowWidth, SDL_Color Color){
	uint8_t* PixelPtr = (uint8_t*)MySurface->pixels + (y * Size * WindowWidth + x * Size) * 4;
	for (int i = 0; i < Size; i++){
		for (int j = 0; j < Size; j++){
			*(PixelPtr + j * 4) = Color.b;
			*(PixelPtr + j * 4 + 1) = Color.g;
			*(PixelPtr + j * 4 + 2) = Color.r;
		}
		PixelPtr += WindowWidth * 4;
	}
}
