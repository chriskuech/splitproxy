
#include "../lib/networking.h"

#define BUFSIZE 1024


int main(int argc, char *argv[]) {
    // handle arguments
    if (argc != 2) {
        printf("Usage: %s <host:ip>\n", argv[0]);
        return 0;
    }
    char *host;
    int port;
    parseaddress(argv[1], &host, &port);

    int fd = client(host, port);
    int n;
    char buf[BUFSIZE];
    while ((n = read(STDIN_FILENO, buf, BUFSIZE)) > 0) {
        write(fd, buf, n);
    }
}

