#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h> // gcc pthreads_demo.c -lpthread -o pthreads_demo


/**
 * Parses a "host:port"-formatted address into its parts
 */
void parseaddress(char *address, char **host, int *port);


/**
 * Connects to hostname:portno and returns the socket
 */
int client(char *hostname, int portno);


/**
 * Listens for socket connections to portno and passes the file
 * socket file descriptor to the handler in a thread
 */
int server(int portno, void *(*handler)(void *));

