/**
 * networking.c 
 * derived from http://www.cs.cmu.edu/afs/cs/academic/class/15213-f99/www/class26/
 * usage: tcpclient <host> <port>
 */


#include "networking.h"

#define BUFSIZE 1024



void parseaddress(char *address, char **host, int *port) {
    char *c;
    for (c = address; *c != ':'; c++) {}
    *c = '\0';
    *host = address;
    *port = atoi(c + 1);
}




int client(char *hostname, int portno) {
    int sockfd, n;
    struct sockaddr_in serveraddr;
    struct hostent *server;

    // socket: create the socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        return -1;
    }

    // gethostbyname: get the server's DNS entry
    if ((server = gethostbyname(hostname)) == NULL) {
        fprintf(stderr,"ERROR, no such host as %s\n", hostname);
        return -1;
    }

    // build the server's Internet address 
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
	  (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(portno);

    // connect: create a connection with the server 
    if (connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1) {
        perror("connect");
        return -1;
    }

    return sockfd;
}






int server(int portno, void *(*handler)(void *)) {
    int parentfd; // parent socket
    int childfd; // child socket
    int clientlen; // byte size of client's address
    struct sockaddr_in serveraddr; // server's addr
    struct sockaddr_in clientaddr; // client addr
    struct hostent *hostp; // client host info
    char buf[BUFSIZE]; // message buffer
    char *hostaddrp; // dotted decimal host addr string
    int optval; // flag value for setsockopt
    int n; // message byte size

    // socket: create the parent socket
    if ((parentfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) { 
        perror("ERROR opening socket");
        return -1;
    }

    // prevent binding errors on consequitive connections
    optval = 1;
    setsockopt(parentfd, SOL_SOCKET, SO_REUSEADDR, 
            (const void *)&optval , sizeof(int));

    // build the server's Internet address
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons((unsigned short)portno);

    // bind: associate the parent socket with a port
    if (bind(parentfd, (struct sockaddr *) &serveraddr, 
            sizeof(serveraddr)) == -1) {
        perror("binding");
        return -1;
    }

    // listen: make this socket ready to accept connection requests 
    if (listen(parentfd, 5) == -1) { // |queue| = 5
        perror("listen");
        return -1;
    }

    // run handler on connection
    clientlen = sizeof(clientaddr);
    while (1) {
        // accept: wait for a connection request
        if ((childfd = accept(parentfd, (struct sockaddr *) &clientaddr, &clientlen)) == -1) {
            perror("accept");
            continue;
        }

        // gethostbyaddr: determine who sent the message
        if ((hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, 
                    sizeof(clientaddr.sin_addr.s_addr), AF_INET)) == NULL) {
            perror("gethostbyaddr");
            continue;
        }
        if ((hostaddrp = inet_ntoa(clientaddr.sin_addr)) == NULL) {
            perror("inet_ntoa");
            continue;
        }

        // pthread create
        pthread_t tid;
        if (pthread_create(&tid, NULL, handler, &childfd) != 0) {
            perror("pthread_create");
            continue;
        }

    }

    return -1;
}




