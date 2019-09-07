#ifndef SERVER_H 
#define SERVER_H

#include "../include/BoardState.h"

bool looper(int master_socket, int max_clients, int client_socket[2], sockaddr_in address, int addrlen, BoardState& currentState, int &pInt);

#endif
