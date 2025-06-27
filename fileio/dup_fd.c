#include "error_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int
main(int argc, char *argv[])
{
    int fd1, fd2, flags, accessMode;
    off_t dup_off;

    fd1 = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
    if (fd1 == -1)
        errExit("open");

    fd2 = dup(fd1);
    if (fd2 == -1)
        errExit("dup");

    if (lseek(fd1, 5, SEEK_CUR) == -1)
        errExit("lseek");

    dup_off = lseek(fd2, 0, SEEK_CUR);
    if (dup_off == -1)
        errExit("lseek 2");

    printf("Duplicate offset %lld\n", dup_off);

    flags = fcntl(fd2, F_GETFL);
    if (flags & O_RDWR)
        printf("Flags are the same!\n");
}
