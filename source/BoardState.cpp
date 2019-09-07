/*
 * Implementation file for the board state. To be passed between server and
 *  client to synchronize board states in clients.
 */

#include <cstring>
#include "include/BoardState.h"
#include <string.h>

// Default constructor
BoardState::BoardState() : game_on(true), winner(-1) {
	player_1.setPos(15, 20);
	player_2.setPos(64, 20);
	snake_food.randPos();
}

// Copy constructor
BoardState::BoardState(const BoardState& lhs) {
	winner = lhs.winner;
	game_on = lhs.game_on;
	player_1 = lhs.player_1;
	player_2 = lhs.player_2;
	snake_food = lhs.snake_food;
}

// Destructor
BoardState::~BoardState() {

}

// Update state based on each snake head's position and direction
void BoardState::update() {
	// Update snake head position and end game if collision with walls occurs
	if (!player_1.updatePos()) {
		game_on = false;
		winner = 2;
		return;
	}
	if (!player_2.updatePos()) {
		game_on = false;
		winner = 1;
		return;
	}

	// Check if a snake ate the food and regenerate if necessary
	if (player_1.getXPos() == snake_food.getXPos() && player_1.getYPos() == snake_food.getYPos()) {
		// Add 1 to player 1's score
		player_1.addLength();

		// Generate new food
		snake_food.randPos();
	} else if (player_2.getXPos() == snake_food.getXPos() && player_2.getYPos() == snake_food.getYPos()) {
		// Add length to player 2
		player_2.addLength();

		// Generate new food
		snake_food.randPos();
	}

}
