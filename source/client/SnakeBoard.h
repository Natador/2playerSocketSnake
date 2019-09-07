/*
 * Board class to store the board state and update based on the player
 *  positions and history.
 * 
 * Responsible for interfacing with Curses and actually drawing stuff
 */

#ifndef _BOARD_H
#define _BOARD_H

#include "Snake.h"
#include "../include/SnakeFood.h"
#include "../include/BoardState.h"
#include <curses.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <time.h>

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 24
#define BORDER_EDGE '-'
#define BORDER_CORNER '+'
#define BORDER_SIDE '|'

class SnakeBoard {
	private:
		int sock_id;
		int currentPlayer;
		bool won;
		BoardState internalState;
		Snake player1;
		Snake player2;
		SnakeFood food;

	public:
		SnakeBoard();
		~SnakeBoard();

		void initConnection(char* hostname, int port);
		void initScreen(void);
		void getState(void);
		bool update(void);
		void collectInput(void);
		void sendState(void);
		void draw(void);
		void quit(void);
		void endGame(void);
};

#endif
