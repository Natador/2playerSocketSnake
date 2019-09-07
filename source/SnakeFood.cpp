/*
 * Implementation file for the SnakeFood class, which encodes the position
 *  of the food for the snakes.
 */

#include "include/SnakeFood.h"

// Default constructor
SnakeFood::SnakeFood() {
	randPos();
}

// Destructor
SnakeFood::~SnakeFood() {

}

// Parameterized constructor with X/Y position
SnakeFood::SnakeFood(int xpos, int ypos) : x_pos(xpos), y_pos(ypos) {

}

// Return X position
int SnakeFood::getXPos(void) {
	return x_pos;
}

// Return Y position
int SnakeFood::getYPos(void) {
	return y_pos;
}

// Assigns a random value to the x and y position
void SnakeFood::randPos() {
	x_pos = rand() % (X_SIZE-2) + 1;
	y_pos = rand() % (Y_SIZE-2) + 1;
}
