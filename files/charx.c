#include "error_functions.h"
#include "tlpi_hdr.h"
#include <stdlib.h>
#include <sys/stat.h>


int charx(char *pathname)
{
    struct stat statbuf;
    mode_t mode;

    if (stat(pathname, &statbuf) == -1)
        errExit("stat");

    mode = statbuf.st_mode;

    if (mode & (S_IRGRP | S_IRUSR | S_IROTH))
        mode = mode | S_IRGRP | S_IRUSR | S_IROTH;

    if ((mode & S_IFDIR) || (mode & (S_IXUSR | S_IXGRP | S_IXOTH)))
        mode = mode | S_IXUSR | S_IXGRP | S_IXOTH;

    return chmod(pathname, mode);
}

int
main(int argc, char *argv[])
{

    if (charx(argv[1]) == 0)
        exit(EXIT_SUCCESS);
    exit(EXIT_FAILURE);
}
