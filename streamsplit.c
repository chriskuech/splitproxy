

#include "streamsplit.h"



/**
 * Convenience function for creating pipes
 */
static inline void pipeassign(int *in, int *out) {
    int fds[2];
    pipe(fds);
    *out  = fds[0];
    *in = fds[1];
}



/**
 * Copies max(n bytes, remaining bytes) from file descriptor in 
 * to file descriptor out moving the file pointer, returning the
 * number of bytes copied
 */
static inline int splicen(int n, int in, int out) {
    int bytesread = 0, bytesleft = n;
    while (bytesleft) {
        int len = splice(in, NULL, out, NULL, bytesleft, SPLICE_F_MOVE);
        if (len == 0) {
            return bytesread;
        } else if (len == -1) {
            perror("splice");
            return -1;
        } else {
            bytesread += len;
            bytesleft -= len;
        }
    }
    return bytesread;
}



/**
 * Copies max(n bytes, remaining bytes) from file descriptor in 
 * to file descriptor out without moving the file pointer, returning
 * the number of bytes copied
 */
static inline int teen(int n, int in, int out) {
    int bytesread = 0, bytesleft = n;
    while (bytesleft) {
        int len = tee(in, out, n, SPLICE_F_NONBLOCK);
        if (len == 0) {
            return bytesread;
        } else if (len == -1) {
            if (errno == EAGAIN) {
                continue;
            } else {
                perror("tee");
                return -1;
            }
        } else {
            bytesread += len;
            bytesleft -= len;
        }
    }
    return bytesread;
}






/**
 * Copies all the data from file descriptor infile to 
 * file descriptors outfile1 and outfile2
 */
int streamsplit(int infile, int outfile1, int outfile2) {
    // create buffers for splicing from sockets
    int inboundbuf_in,   inboundbuf_out,
        outboundbuf1_in, outboundbuf1_out,
        outboundbuf2_in, outboundbuf2_out;
    pipeassign(&inboundbuf_in,   &inboundbuf_out  );
    pipeassign(&outboundbuf1_in, &outboundbuf1_out);
    pipeassign(&outboundbuf2_in, &outboundbuf2_out);

    
    while (1) {
        int n; // num bytes to read

        // copy the input file to the input buffer
        if ((n = splicen(INT_MAX, infile, inboundbuf_in)) == -1) {
            perror("1st");
            return -1;
        } else if (n == 0) {
            return 0;
        }

        // copy the inbound buffer to outbound buffer 1 without consuming 
        if ((n = teen(n, inboundbuf_out, outboundbuf1_in)) == -1) {
            perror("2nd");
            return -1;
        } else if (n == 0) {
            perror("3rd");
            return -1;
        }

        // copy the inbound buffer to outbound buffer 2
        if ((n = splicen(n, inboundbuf_out, outboundbuf2_in)) == -1) {
            perror("4th");
            return -1;
        } else if (n == 0) {
            perror("5th");
            return -1;
        }

        // copy outbound buffer 1 to output file 1
        if ((n = splicen(n, outboundbuf1_out, outfile1)) == -1) {
            perror("6th");
            return -1;
        } else if (n == 0) {
            perror("7th");
            return -1;
        }

        // copy outbound buffer 2 to output file 2  
        if ((n = splicen(n, outboundbuf2_out, outfile2)) == -1) {
            perror("8th");
            return -1;
        } else if (n == 0) {
            perror("9th");
            return -1;
        }

    }

}


