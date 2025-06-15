#include "tlpi_hdr.h"
#include <sys/fcntl.h>

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

int main(int argc, char *argv[]) {
    int opt, outputFd, openFlags;
    Boolean append;
    mode_t filePerms;
    ssize_t numRead;
    char buf[BUF_SIZE];

    append = FALSE;
    while ((opt = getopt(argc, argv, "a")) != -1) {
    if (opt == 'a')
      append = TRUE;
    }

    openFlags = O_CREAT | O_WRONLY | (append ? O_APPEND : O_TRUNC);
    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
        S_IROTH | S_IWOTH;

    outputFd = open(argv[optind], openFlags, filePerms);
    if (outputFd == -1)
        errExit("opening file %s", argv[1]);

    while ((numRead = read(STDIN_FILENO, buf, BUF_SIZE)) > 0)
        if (write(outputFd, buf, numRead) != numRead)
            fatal("write() returned error or partial write occurred");
    if (numRead == -1)
        errExit("read");

    if (close(outputFd) == -1)
        errExit("close output");

    exit(EXIT_SUCCESS);
}
