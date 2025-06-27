#include "error_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    int fd, oflags, sflags;
    ssize_t numWritten;
    char *buf;
    Boolean append;

    buf = "x";

    append = argc >= 4;

    oflags = O_RDWR | O_CREAT | (append ? O_APPEND : 0);
    sflags = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

    fd = open(argv[1], oflags, sflags);
    if (fd == -1)
        errExit("open");

    if (lseek(fd, 0, SEEK_SET) == -1)
        errExit("lseek");

    for (int i = 0; i < atoi(argv[2]); i++) {
        if (append) {
            if (lseek(fd, 0, SEEK_END) == -1)
                errExit("lseek");
        }
        numWritten = write(fd, buf, 1);
        if (numWritten == -1)
            errExit("write");
    }
    exit(EXIT_SUCCESS);
}
