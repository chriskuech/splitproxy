#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "streamsplit.h"



int main(int argc, char *argv[]) {
    int in, out1, out2;
    switch (argc) {
        case 2:
            in   = STDIN_FILENO;
            out1 = STDOUT_FILENO;
            out2 = open(argv[1], O_RDWR);
            break;
        case 3:
            in   = STDIN_FILENO;
            out1 = open(argv[1], O_RDWR);
            out2 = open(argv[2], O_RDWR);
            break;
        case 4:
            in   = open(argv[1], O_RDWR);
            out1 = open(argv[2], O_RDWR);
            out2 = open(argv[3], O_RDWR);
            break;
        default:
            printf("Usage:\n  %s <input> <output1> <output2>\n", argv[0]);
            return -1;
    }
    
    if (in == -1) {
        perror("opening file in");
        return -1;
    }
    if (out1 == -1) {
        perror("opening file out1");
        return -1;
    }
    if (out2 == -1) {
        fprintf(stderr, "%s\n", argv[2]);
        perror("opening file out2");
        return -1;
    }
    
    return streamsplit(in, out1, out2);
    
}

