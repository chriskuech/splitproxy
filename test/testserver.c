
#include "../lib/networking.h"

#define BUFSIZE 1024


static void *handler(void *fd) {
    int n;
    char buf[BUFSIZE];
    while ((n = read(*(int *)fd, buf, BUFSIZE)) > 0) {
        write(STDOUT_FILENO, buf, n);
    }
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <portno>\n", argv[0]);
        return 0;
    }

    return server(atoi(argv[1]), handler);
}

