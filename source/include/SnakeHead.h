/*
 * Data structucture to encode information about the head of the snake.
 *
 */

#ifndef _SNAKEHEAD_H
#define _SNAKEHEAD_H

#include "SnakeFood.h"

#define PLAYER_1_X 15
#define PLAYER_1_Y 7
#define PLAYER_2_X 64
#define PLAYER_2_Y 7

#define SNAKECHAR 'S'

enum Direction{
	Up,
	Right,
	Down,
	Left
};

class SnakeHead {
	private:
		int x_pos, y_pos; 	// The position of the snake head on the screen
		int dir; 			// Direction of the snake's movement (up, down, left, right)
		int length; 		// Length of the snake

	public:
		SnakeHead();
		SnakeHead(const SnakeHead& rhs);
		~SnakeHead();

		SnakeHead(int xpos, int ypos);

		void setPos(int xpos, int ypos);
		void setDir(int newdir);
		int getDir(void) const;
		int getXPos(void) const;
		int getYPos(void) const;
		int getLength(void) const;
		void addLength(void);
		bool updatePos(void);
};

#endif
