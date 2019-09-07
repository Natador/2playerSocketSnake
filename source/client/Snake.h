/*
 * Snake class. Stores the snake's head and keeps track of the tail. Also checks
 *  for collisions with the wall.
 */

#ifndef _SNAKE_H
#define _SNAKE_H

#include "TailSeg.h"
#include "../include/SnakeHead.h"
#include <curses.h>

class Snake {
	private:
		bool alive;
		bool growflag;
		SnakeHead head;
		int last_x, last_y;
		int tail_head, tail_tail;
		TailSeg tail[X_SIZE*Y_SIZE];

	public:
		int num;

		Snake();
		~Snake();

		void initPos(int xpos, int ypos);
		bool isAlive(void);
		void undraw(void);
		void draw(void);
		void newHead(SnakeHead& newHead);
		//grow(void);
};

#endif
