#include "Simulation.hpp"
#include "DrawingFunctions.hpp"

#include <ctime>
#include <cmath>
#include <iostream>

inline int SDL_GetWindowRefreshRate(SDL_Window *Window){
    SDL_DisplayMode Mode;
    int DisplayIndex = SDL_GetWindowDisplayIndex(Window);
    int DefaultRefreshRate = 60;
    if(SDL_GetDesktopDisplayMode(DisplayIndex, &Mode) != 0){
        return DefaultRefreshRate;
    }
    if(Mode.refresh_rate == 0){
        return DefaultRefreshRate;
    }
    return Mode.refresh_rate;
}

Simulation::Simulation() : Columns(1280), Rows(720), CellDimension(1){
	WindowWidth = Columns * CellDimension;
	WindowHeight = Rows * CellDimension;
	SDL_Init(SDL_INIT_VIDEO);
	Window = SDL_CreateWindow("Conway's Game of Life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WindowWidth, WindowHeight, SDL_WINDOW_SHOWN);
	Surface = SDL_GetWindowSurface(Window);
	FPS = SDL_GetWindowRefreshRate(Window);
	Quit = false;
	ActualGeneration = new bool[Columns*Rows];
	NextGeneration = new bool[Columns*Rows];
	Neighbours = new char[Columns*Rows];
	for(int i = 0; i < Columns*Rows; i++){
		ActualGeneration[i] = false;
		NextGeneration[i] = false;
		Neighbours[i] = 0;
	}
	this->FirstGeneration();
//	this->Glider((Columns * Rows)/2 + Columns/2);
}

Simulation::~Simulation(){
  delete[] ActualGeneration;
  delete[] NextGeneration;
  delete[] Neighbours;
	SDL_FreeSurface(Surface);
	SDL_DestroyWindow(Window);
	SDL_Quit();
}

void Simulation::GetCommands(){
	while(SDL_PollEvent(&Event)){
		if(Event.type == SDL_QUIT){
			Quit = true;
			break;
		}
		if(Event.key.state == SDL_PRESSED){
			if(Event.key.keysym.sym == SDLK_ESCAPE){
				Quit = true;
				break;
			}
		}
		if(Event.button.state == SDL_PRESSED || Event.button.state == SDL_RELEASED){
			int x, y;
			SDL_GetMouseState(&x, &y);
			x /= CellDimension;
			y /= CellDimension;
			if(Event.button.button == SDL_BUTTON_LEFT){
				if(ActualGeneration[y*Columns + x] == false){
					ActualGeneration[y*Columns + x] = true;
					this->CalculateNeighbours(y*Columns + x);
				}
			}
			else if(Event.button.button == SDL_BUTTON_RIGHT || Event.button.button == SDL_BUTTON_RMASK){
				if(ActualGeneration[y*Columns + x] == true){
					ActualGeneration[y*Columns + x] = false;
					this->EraseNeighbours(y*Columns + x);
				}
			}
		}
	}
}

void Simulation::CalculateNeighbours(int Position){
	int LeftX, RightX, AboveY, BelowY;  //these are for the wrap around world
	LeftX = (Position%Columns == 0) ? Columns-1 : -1;
	RightX = (Position%Columns == (Columns-1)) ? -(Columns-1) : 1;
	AboveY = (Position/Columns == 0) ? Columns*Rows - Columns : -Columns;
	BelowY = (Position/Columns == (Rows-1)) ? -(Columns*Rows - Columns) : Columns;
	Neighbours[Position+AboveY+LeftX]++;
	Neighbours[Position+AboveY]++;
	Neighbours[Position+AboveY+RightX]++;
	Neighbours[Position+LeftX]++;
	Neighbours[Position+RightX]++;
	Neighbours[Position+BelowY+LeftX]++;
	Neighbours[Position+BelowY]++;
	Neighbours[Position+BelowY+RightX]++;
}

void Simulation::EraseNeighbours(int Position){
	int LeftX, RightX, AboveY, BelowY;
	LeftX = (Position%Columns == 0) ? Columns - 1 : -1;
	RightX = (Position%Columns == (Columns - 1)) ? -(Columns - 1) : 1;
	AboveY = (Position/Columns == 0) ? Columns*Rows - Columns : -Columns;
	BelowY = (Position/Columns == (Rows - 1)) ? -(Columns*Rows - Columns) : Columns;
	Neighbours[Position+AboveY+LeftX]--;
	Neighbours[Position+AboveY]--;
	Neighbours[Position+AboveY+RightX]--;
	Neighbours[Position+LeftX]--;
	Neighbours[Position+RightX]--;
	Neighbours[Position+BelowY+LeftX]--;
	Neighbours[Position+BelowY]--;
	Neighbours[Position+BelowY+RightX]--;
}

void Simulation::Glider(int Position){
	ActualGeneration[Position-Columns] = true;
	ActualGeneration[Position+1] = true;
	ActualGeneration[Position+Columns-1] = true;
	ActualGeneration[Position+Columns] = true;
	ActualGeneration[Position+Columns+1] = true;
	this->CalculateNeighbours(Position-Columns);
	this->CalculateNeighbours(Position+1);
	this->CalculateNeighbours(Position+Columns-1);
	this->CalculateNeighbours(Position+Columns);
	this->CalculateNeighbours(Position+Columns+1);
}

void Simulation::FirstGeneration(){
	for(int i = 0; i < Columns*Rows; i++){
		if(rand()%5 != 0){  //living cell probability set to ~20%
			continue;
		}
		ActualGeneration[i] = true;
		this->CalculateNeighbours(i);
	}
}

void Simulation::SetNextGeneration(){
	for(int i = 0; i < Columns*Rows; i++){
		if(Neighbours[i] == 0 && ActualGeneration[i] == false)
			continue;
		if(ActualGeneration[i] == true){
			if(Neighbours[i] < 2 || Neighbours[i] > 3)
				NextGeneration[i] = false;
			else
				NextGeneration[i] = true;
			continue;
 		}
 		if(Neighbours[i] == 3)
 			NextGeneration[i] = true;
	}
}

void Simulation::ResetNeighbours(){
	for(int i = 0; i < Columns*Rows; i++){
		Neighbours[i] = 0;
	}
}

void Simulation::CreateActualGeneration(){
	for(int i = 0; i < Columns*Rows; i++){
		ActualGeneration[i] = NextGeneration[i];
		if(ActualGeneration[i] == true)
			this->CalculateNeighbours(i);
	}
}

void Simulation::Draw(){
	for(int i = 0; i < Columns*Rows; i++){
		int x = i%Columns;
		int y = i/Columns;
		if(ActualGeneration[i] == true){
			if(IsSquareOfGivenColor(x, y, CellDimension, Surface, WindowWidth, Black))
				DrawSquare(x, y, CellDimension, Surface, WindowWidth, White);
			continue;
		}
		if(IsSquareOfGivenColor(x, y, CellDimension, Surface, WindowWidth, White))
			DrawSquare(x, y, CellDimension, Surface, WindowWidth, Black);
	}
}

void Simulation::Cycle(){
	unsigned int Seed = (unsigned)time(NULL);
	srand(Seed);
	unsigned long Generations = 0;
	std::cout << "Initializing...\n";
	while (!Quit){
		double IterationStart = SDL_GetPerformanceCounter();
		this->GetCommands();
		if(Quit)
			break;
		this->Draw();
		this->SetNextGeneration();
		this->ResetNeighbours();
		this->CreateActualGeneration();
		Generations++;
		SDL_UpdateWindowSurface(Window);
		double IterationEnd = SDL_GetPerformanceCounter();
		double ElapsedSeconds = (IterationEnd-IterationStart) / (double)SDL_GetPerformanceFrequency();
		double Delay =  (1000.0/(double)FPS) - (ElapsedSeconds*1000.0);
		if(Delay > 0)
			SDL_Delay(std::max(0, (int)round(Delay)));
	}
	std::cout << "Seed: " << Seed << ".\n";
	std::cout << "Total generations: " << Generations << ".\n";
}
