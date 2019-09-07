/*
 * Implementation file for Snake class. Responsible for keeping track of
 *  the snake, its body/tail, and its life/death.
 */

#include "Snake.h"

// Default constructor
Snake::Snake() : alive(true), growflag(false), last_x(-1), last_y(-1), tail_head(0), tail_tail(0) {

}

// Destructor
Snake::~Snake() {

}

// Sets the head position of the snake
void Snake::initPos(int xpos, int ypos) {
	head.setPos(xpos, ypos);
}

// Draws the snake on the screen according to the position
void Snake::draw(void) {
	// Replace old head position with tail char.
	if (last_x != -1 && last_y != -1) {
		//mvaddch(last_y, last_x, TAILCHAR);
		if (head.getLength() == 0) {
			mvaddch(last_y, last_x, ' ');
		} else {
			mvaddch(last_y, last_x, TAILCHAR);
		}
	}

	// Draw new head
	char ch = num + '0';
	mvaddch(head.getYPos(), head.getXPos(), ch);

	// Undraw the last position of the tail and update the queue pointers
	if (head.getLength() != 0) {
		// Update tail_head for written position
		tail[++tail_head].x_pos = last_x;
		tail[tail_head].y_pos = last_y;

		// Undraw tail_tail
		ch = ' ';
		mvaddch(tail[tail_tail].y_pos, tail[tail_tail].x_pos, ch);
		tail_tail++;
	}

	// Update last position
	last_x = head.getXPos();
	last_y = head.getYPos();
}

// Update the head of the snake given a head
void Snake::newHead(SnakeHead& newSnakeHead) {
	if (newSnakeHead.getLength() > head.getLength()) {
		tail[++tail_head].x_pos = last_x;
		tail[tail_head].y_pos = last_y;
		mvaddch(last_y, last_x, 'O');
	}
	head = newSnakeHead;
}
