/*
 * Implementation file for SnakeHead class
 */

#include "include/SnakeHead.h"
#include <stdio.h>

// Default constructor
SnakeHead::SnakeHead() : x_pos(0), y_pos(0), dir(Up), length(0) {

}

// Copy constructor
SnakeHead::SnakeHead(const SnakeHead& rhs) {
	this->x_pos = rhs.getXPos();
	this->y_pos = rhs.getYPos();
	this->length = rhs.getLength();
	this->dir = rhs.getDir();
}

// Destructor
SnakeHead::~SnakeHead() {

}

// Parameterized constructor
SnakeHead::SnakeHead(int xpos, int ypos) : x_pos(xpos), y_pos(ypos), dir(-1), length(0) {

}

// Returns the snake's direction of travel
int SnakeHead::getDir(void) const {
	return dir;
}

// Returns the X position of the head
int SnakeHead::getXPos(void) const {
	return x_pos;
}

// Returns the Y position of the head
int SnakeHead::getYPos(void) const {
	return y_pos;
}

// Returns the length of the snake
int SnakeHead::getLength(void) const {
	return length;
}

// Update the position according to the direction
bool SnakeHead::updatePos() {
	char noCollision = true;
	// Update position based on current direction
	switch (dir) {
		case Up:
			y_pos -= 1;
			break;
		case Down:
			y_pos += 1;
			break;
		case Left:
			x_pos -= 1;
			break;
		case Right:
			x_pos += 1;
			break;
		default:
			// Do nothing
			break;
	}

	// Check x coordinates for collision with walls
	if (x_pos <= 0) {
		x_pos = 0;
		noCollision = false;
	} else if (x_pos >= X_SIZE-1) {
		x_pos = X_SIZE-1;
		noCollision = false;
	}

	// Check y coordinates for collision
	if (y_pos <= 0) {
		y_pos = 0;
		noCollision = false;
	} else if (y_pos >= Y_SIZE-1) {
		y_pos = Y_SIZE-1;
		noCollision = false;
	}

	// Return collision state
	return noCollision;
}

// Sets the direction of the snake
void SnakeHead::setDir(int newdir) {
	if (newdir == Up || newdir == Down || newdir == Left || newdir == Right)
		dir = newdir;
}

// Increases the length of the snake by 1
void SnakeHead::addLength(void) {
	length++;
}

// Sets the position of the snake's head
void SnakeHead::setPos(int xpos, int ypos) {
	x_pos = xpos;
	y_pos = ypos;
}
