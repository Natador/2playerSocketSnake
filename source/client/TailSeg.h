/*
 * Data structure to encode the tail segments of the snake on the board.
 */

#ifndef _TAILSEG_H
#define _TAILSEG_H

#define TAILCHAR 'O'

class TailSeg {
	public:
		int x_pos, y_pos;
		TailSeg();
		~TailSeg();
		TailSeg(int xpos, int ypos);
};

#endif
