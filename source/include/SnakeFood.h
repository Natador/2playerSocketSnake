/*
 * Data structure to define the information about the snake food
 */

#ifndef _SNAKEFOOD_H
#define _SNAKEFOOD_H

#define X_SIZE 80
#define Y_SIZE 24

#include <cstdlib>

#define FOODCHAR '*'


#include <math.h>

class SnakeFood {
	private:
		int x_pos, y_pos;

	public:
		SnakeFood();
		~SnakeFood();
		SnakeFood(int xpos, int ypos);

		int getXPos();
		int getYPos();
		void randPos(void);
};

#endif
