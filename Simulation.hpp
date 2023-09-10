#pragma once

#include <SDL.h>

class Simulation{
	public:
		Simulation();
		~Simulation();
		void GetCommands();
		void FirstGeneration();
		void Glider(int Position);
		void CalculateNeighbours(int Position);
		void EraseNeighbours(int Position);
		void SetNextGeneration();
		void ResetNeighbours();
		void CreateActualGeneration();
		void Draw();
		void Cycle();
	private:
		const int Columns, Rows;
		const int CellDimension;
		int WindowWidth, WindowHeight;
		int FPS;
		SDL_Window *Window = nullptr;
		SDL_Surface *Surface = nullptr;
		SDL_Event Event;
		bool Quit;
		bool *ActualGeneration;
		bool *NextGeneration;
		char *Neighbours;
};
