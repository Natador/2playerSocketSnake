/*
 * Implementation file for the TailSeg class, which stores information
 *  about a tail segment of the snake from its history
 */

#include "TailSeg.h"

// Default constructor
TailSeg::TailSeg() {
	x_pos = 0;
	y_pos = 0;
}

// Destructor
TailSeg::~TailSeg() {

}

// Parameterized constructor
TailSeg::TailSeg(int xpos, int ypos) {
	x_pos = xpos;
	y_pos = ypos;
}
