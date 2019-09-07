//Example code: A simple server side code, which echos back the received message.
//Handle multiple socket connections with select and fd_set on Linux
#include <stdio.h>
#include <string.h>   //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>   //close
#include <arpa/inet.h>    //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros

#include <signal.h>

#include "server.h"
#include "../include/BoardState.h"
#include "../include/Port.h"


int main(int argc , char *argv[])
{
    int opt = true;

    int master_socket ,
        addrlen , // length of IP address
        client_socket[2] , // list of all the sockets, we can have
        max_clients = 2 ,
        i;

    struct sockaddr_in address;

    // ** IMPORTANT ** for simplicity, ignore SIGPIPE
    signal(SIGPIPE, SIG_IGN);

    //initialise all client_socket[] to 0 so not checked
    for (i = 0; i < max_clients; i++)
    {
        client_socket[i] = 0;
    }

    //create a master socket
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    //set master socket to allow multiple connections ,
    //this is just a good habit, it will work without this
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
                sizeof(opt)) < 0 )
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    //type of socket created
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    //bind the socket to localhost port 8888
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Listener on port %d \n", PORT);

    //try to specify maximum of 3 pending connections for the master socket
    if (listen(master_socket, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    //accept the incoming connection
    addrlen = sizeof(address);
    puts("Waiting for connections ...");

	//
    // Send to clients until game over or one disconnects
	//
	BoardState currentState; // state for our upcomming game
	bool playing = true;
	int playernum = 1; // how we tell which player is which,
                    // will be intermented, when passed to looper

	// While the game is being played
    while (playing) {
		playing = looper(master_socket, max_clients, client_socket, address, addrlen, currentState, playernum);
		playing = looper(master_socket, max_clients, client_socket, address, addrlen, currentState, playernum);
		if (!playing) continue;
		printf("Sent/received with clients\n");
		if (playernum > 2)
			currentState.update(); // checks if player one/player 2 has won
                            // also check if player one/player 2 has eaten the
                            //food, and then generates a new postion for the snake_food
		printf("Player 1 coords: (%d, %d)\nPlayer 2 coords (%d, %d)\n", currentState.player_1.getXPos(), currentState.player_1.getYPos(), currentState.player_2.getXPos(), currentState.player_2.getYPos());
    }

    return 0;
}
