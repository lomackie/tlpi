#include "error_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    int fd, numWritten;

    fd = open(argv[1], O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if (fd == -1)
        errExit("open");

    if (lseek(fd, 0, SEEK_SET) == -1)
        errExit("lseek");

    numWritten = write(fd, &argv[2][0], strlen(&argv[2][0]));
    if (numWritten == -1)
        errExit("write");
    exit(EXIT_SUCCESS);
}
