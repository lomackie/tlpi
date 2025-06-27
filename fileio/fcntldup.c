#include "error_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

int
dup(int oldfd)
{

    int newfd = fcntl(oldfd, F_DUPFD, 0);
    if (newfd == -1)
        errExit("fcntl");

    return newfd;
}

int
dup2(int oldfd, int newfd)
{
    int returnfd;

    if (oldfd == newfd) {
        if (fcntl(oldfd, F_GETFL) == -1) {
            errno = EBADF;
            return -1;
        }
        returnfd = oldfd;
    }
    else {
        if (close(newfd) == -1)
            errExit("close");

        returnfd = fcntl(oldfd, F_DUPFD, newfd);

        if (returnfd != newfd)
            errMsg("Didn't get newfd back");
    }

    return returnfd;
}

int
main(int argc, char *argv[])
{
    exit(EXIT_SUCCESS);
}
