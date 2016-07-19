#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "./lib/streamsplit.h"
#include "./lib/networking.h"



static char *remotehost, *logginghost;
static int remoteport, loggingport;



static void *handler(void *sockfd_p) {
    int remote, logging, sockfd;
    sockfd = *(int *)sockfd_p;
    if ((remote = client(remotehost, remoteport)) == -1) {
        perror("client (1)");
        return (void *)-1;
    }
    if ((logging = client(logginghost, loggingport)) == -1) {
        perror("client (2)");
        return (void *)-1;
    }
    if (streamsplit(sockfd, remote, logging) == -1) {
        perror("streamsplit (1)");
        return (void *)-1;
    }
    if (streamsplit(remote, sockfd, logging) == -1) {
        perror("streamsplit (2)");
        return (void *)-1;
    }
    return NULL;
}



int main(int argc, char *argv[]) {
    // handle arguments
    if (argc != 4) {
        printf("Usage: %s port <remoteserver:port> <loggingserver:port>\n", argv[0]);
        return 0;
    } else {
        parseaddress(argv[2], &remotehost, &remoteport);
        parseaddress(argv[3], &logginghost, &loggingport);
    }
    
    // start the split proxy server
    if (server(atoi(argv[1]), handler) == -1) {
        perror("server");
        return -1;
    }
    
    return 0;
}

