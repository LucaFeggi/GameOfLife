#include "Simulation.hpp"

// -o main Simulation.cpp DrawingFunctions.cpp

/*
	Commands:
	- left mouse to draw cells
	- right mouse to erase cells
	- esc to quit

	Advice:
	For the first generation use either the glider, or the casual generation.
	Using both might cause problems, since I'm not checking for any already living cells.
*/

int main(int argc, char** args){

	Simulation GameOfLife;

	GameOfLife.Cycle();

	return 0;

}
