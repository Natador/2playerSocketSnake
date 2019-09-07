#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <iostream>

#include "server.h"
#include "../include/SnakeFood.h"
#include "../include/SnakeHead.h"

bool looper(int master_socket, int max_clients, int client_socket[2], sockaddr_in address, int addrlen, BoardState& currentState, int &pInt)
{
    fd_set readfds;

    int max_sd,
        activity,
        valread,
        sd, // current socket connected to
        new_socket,
        i; // iterators

    char buffer[1025];

    //clear the socket set
    FD_ZERO(&readfds);

    //add master socket to set
    FD_SET(master_socket, &readfds);
    max_sd = master_socket;

    //add child sockets to set
    for ( i = 0 ; i < max_clients ; i++)
    {
        //socket descriptor
        sd = client_socket[i];

        //if valid socket descriptor then add to read list
        if(sd > 0)
            FD_SET( sd , &readfds);

        //highest file descriptor number, need it for the select function
        if(sd > max_sd)
            max_sd = sd;
    }

    //wait for an activity on one of the sockets , timeout is NULL ,
    //so wait indefinitely
	printf("Waiting for connection\n");
    activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
	printf("Got some activity\n");

    if ((activity < 0) && (errno!=EINTR))
    {
        printf("select error");
    }

    // If the activity is on the master socket, then it is a new connection.
	// Accept only if the number of players is less than 2
    if (FD_ISSET(master_socket, &readfds)) {
        if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        //inform user of socket number - used in send and receive commands
        printf("New connection , socket fd is %d , ip is : %s , port : %d\n", new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

        // Send player their player number (1 or 2)
        memcpy(buffer, &pInt, sizeof(int));
        if( send(new_socket, buffer, sizeof(int ), 0) != sizeof(int)) {
			perror("send");
        }

         // increasements player number and displays to the server screen
        std::cout << "Player "<< pInt++ << " has joined.\n";

        //add new socket to array of sockets
        for (i = 0; i < max_clients; i++) {
            //if position is empty
            if( client_socket[i] == 0 ) {
                client_socket[i] = new_socket;
                break;
            }
        }
    }

    //else its some IO operation on some other socket
	bool playFlag = true; // determines if there is a winner or if they should keep playing
	BoardState tempState;
    for (i = 0; i < max_clients && pInt > 2; i++)
    {
        sd = client_socket[i]; // gets a client

        if (FD_ISSET( sd , &readfds))
        {
            // Check for a request or disconnect from the client
			printf("Listening for request\n");
            if ( (valread = read(sd, buffer, sizeof(BoardState))) == 0 )
            {
                //Somebody disconnected , get his details and print
                getpeername(sd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
                printf("Client %d disconnected, ip %s, port %d\n", i, inet_ntoa(address.sin_addr), ntohs(address.sin_port));

                //Close the socket and mark as 0 in list for reuse
                close( sd );
                client_socket[i] = 0;

				// Do not continue playing
				playFlag = false;
            }
            // Send the client the updated board state and receive their new
            else
            {
				printf("Sending state to client\n");
				if (send(sd, &currentState, sizeof(BoardState), 0) != sizeof(BoardState)) {
					perror("BoardState send");
					exit(1);
				}

				// Receive updated state from client
				printf("Reading reply from client\n");
				valread = read(sd, &tempState, sizeof(BoardState));

				// If return value is zero, client disconnected.
				if (valread == 0) {
					//Somebody disconnected , get his details and print
					getpeername(sd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
					printf("Client %d disconnected, ip %s, port %d\n", i, inet_ntoa(address.sin_addr), ntohs(address.sin_port));

					//Close the socket and mark as 0 in list for reuse
					close( sd );
					client_socket[i] = 0;

					// Do not continue playing
					playFlag = false;

					// Set game over
					currentState.game_on = false;
				} else {
					// Update state from client
					currentState = tempState;

					// Check if the player won
					if (!currentState.game_on) {
						// Do not continue playing
						playFlag = false;

						// Send the current state to the other player
						//
						// Get the request and check for disconnect
						if ( (valread = read(sd, buffer, sizeof(BoardState))) == 0 )
						{
							//Somebody disconnected , get his details and print
							getpeername(sd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
							printf("Client %d disconnected, ip %s, port %d\n", i, inet_ntoa(address.sin_addr), ntohs(address.sin_port));

							//Close the socket and mark as 0 in list for reuse
							close( sd );
							client_socket[i] = 0;
						} else {
							if (send(sd, &currentState, sizeof(BoardState), 0) != sizeof(BoardState)) {
								perror("BoardState send");
								exit(1);
							}
						}

						// Exit the loop
						break;


						// Do not continue playing
						playFlag = false;
					}
				}
            }// end of else statement

        }// end of outer if statement
    } // end of for loop, looping over max_clients

	return playFlag;

} //end of looper
