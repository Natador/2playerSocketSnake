/*
 * Implementation for SnakeBoard class. Interfaces with Curses and calls
 *  necessary functions to use the library.
 */

#include <time.h>
#include "SnakeBoard.h"

// Initialize curses environment, draw border, initialize player positions based
//  on initial values
SnakeBoard::SnakeBoard() : currentPlayer(1) {
	player1.num = 1;
	player2.num = 2;

	// TODO: remove
	internalState.player_2.setDir(0);
	internalState.player_1.setDir(0);

}

// Destroy curses board
SnakeBoard::~SnakeBoard() {
	endwin();
}

// Initializes the Curses screen
void SnakeBoard::initScreen(void) {
	// Initialize curses environment
	initscr();
	cbreak();
	noecho();
	timeout(1);
	curs_set(0);

	// Top row border
	mvaddch(0, 0, BORDER_CORNER);
	mvaddch(0, SCREEN_WIDTH-1, BORDER_CORNER);
	for (int i = 1; i < SCREEN_WIDTH-1; i++) {
		mvaddch(0, i, BORDER_EDGE);
	}

	// Bottom border
	mvaddch(SCREEN_HEIGHT-1, 0, BORDER_CORNER);
	mvaddch(SCREEN_HEIGHT-1, SCREEN_WIDTH-1, BORDER_CORNER);
	for (int i = 1; i < SCREEN_WIDTH-1; i++) {
		mvaddch(SCREEN_HEIGHT-1, i, BORDER_EDGE);
	}

	// Sides border
	for (int i = 1; i < SCREEN_HEIGHT-1; i++) {
		mvaddch(i, 0, BORDER_SIDE);
		mvaddch(i, SCREEN_WIDTH-1, BORDER_SIDE);
	}

	// Initial refresh with borders
	refresh();
}

// Initialize a connection to the server
void SnakeBoard::initConnection(char* hostname, int port) {
	struct sockaddr_in  servadd;        /* the number to call */
	struct hostent      *hp;            /* used to get number */
	int    sock_fd;            /* the socket and fd  */
	int    messlen;                     /* length of received message*/

	int i;
	char clientid[4];

	// Make a random ID for this client
	srand(time(NULL));
	sprintf(clientid, "%d", rand()%1000);

	//
    // Step 1: Get a socket as member variable and error-check
	//
	sock_id = socket( AF_INET, SOCK_STREAM, 0 );    
	if ( sock_id == -1 ) {
		perror("socket");
		exit(1);
	}
	printf("Obtained socket\n");

	//
    // Step 2: Connect to the server after building server's address (host, port)
	//
	bzero( &servadd, sizeof( servadd ) );   /* zero the address     */

	// Obtain host IP and error-check
	hp = gethostbyname( hostname );
	if (hp == NULL) {
		perror(hostname);
		exit(1);
	}
	printf("Obtained host from hostname: %s\n", hostname);

	// Copy server address into servadd's address
	bcopy(hp->h_addr, (struct sockaddr *)&servadd.sin_addr, hp->h_length);
	printf("Copied hp->haddr to serveradd.sin_addr\n");
	servadd.sin_port = htons(port);  		/* fill in port number  */
	printf("Filled in port: %d\n", port);
	servadd.sin_family = AF_INET ;          /* fill in socket type  */

	// Dial connection to server
	printf("Connecting to server...\n");
	if ( connect(sock_id, (struct sockaddr *)&servadd, sizeof(servadd)) != 0) {
		perror("connect");
		exit(1);
	}
	printf("Connected to server.\n");

	//
	// Step 3: Receive player id (1 or 2) from server
	//
	messlen = read(sock_id, &currentPlayer, sizeof(int));
	printf("Current player is %d\n", currentPlayer);
	if (messlen <= 0) {
		perror("connection read");
	}
}

// Quit the game. Happens upon control C?
void SnakeBoard::quit(void) {
	close(sock_id);
	endwin();
	exit(1);
}

// Receive a new BoardState from the server and update internally
void SnakeBoard::getState(void) {
	int messlen;
	int request = 1;

	// Receive from server
	BoardState tempState;

	// Request new state
	if (send(sock_id, &request, sizeof(request), 0) == -1) {
		quit();
		perror("request");
		exit(1);
	}

	// Read call to receive data
	messlen = read(sock_id, &tempState, sizeof(BoardState));
	if (messlen < 0) {
		quit();
		perror("state update read");
		exit(1);
	}

	// Update internal state
	internalState = tempState;
}

// Send updated state to the server
void SnakeBoard::sendState(void) {
	// Send the current state to the server
	if (send(sock_id, &internalState, sizeof(BoardState), 0) == -1) {
		quit();
		perror("reply");
		exit(1);
	}

}

// Update the local board given a BoardState (from the server)
bool SnakeBoard::update() {
	// Check for game over
	if (internalState.game_on) {
		// Update player 1 and 2's snake heads
		player1.newHead(internalState.player_1);
		player2.newHead(internalState.player_2);

		// New food posititon
		food = internalState.snake_food;

	// Else, win has occurred by one player and game is over
	} else {
		// Print win message depending on player
		endwin();
		if (internalState.winner == currentPlayer) {
			won = true;
		} else {
			won = false;
		}

		// Do not keep playing
		return false;
	}

	// Keep playing (true to keep looping)
	return true;
}

// Draw the board on the screen according to the board's state
void SnakeBoard::draw(void) {
	// Draw both players
	player1.draw();
	player2.draw();

	// Draw the food
	mvaddch(food.getYPos(), food.getXPos(), FOODCHAR);
	refresh();
}

// Collect nonblocking terminal input from the user and return BoardState based
//  on this input. Also modifies the current player's head.
void SnakeBoard::collectInput(void) {
	int newdir = -1;
	char ch;

	// Perform a nonblocking read from the terminal (crmode set)
	ch = getch();

	// Determine direction/action depending on character
	switch(ch) {
		case 'w': case 'W': case 'k': case 'K':
			newdir = Up;
			break;
		case 's': case 'S': case 'j': case 'J':
			newdir = Down;
			break;
		case 'a': case 'A': case 'h': case 'H':
			newdir = Left;
			break;
		case 'd': case 'D': case 'l': case 'L':
			newdir = Right;
			break;
		case 'q': case 'Q': case EOF:
			// Quit game
		default: // If unrecognized key or ERR, do nothing.
			newdir = -1;
	}

	// If direction was changed
	if (newdir != -1) {
		// Set direction for current player
		if (currentPlayer == 1)
			internalState.player_1.setDir(newdir);
		else
			internalState.player_2.setDir(newdir);
	}
}

// Print game ending information
void SnakeBoard::endGame(void) {
	endwin();
	if (currentPlayer == internalState.winner) {
		printf("Winner!\n");
	} else {
		printf("Loser!!!!!\n");
	}
}
