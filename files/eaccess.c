#include "tlpi_hdr.h"
#include <stdlib.h>
#include <sys/stat.h>


int eaccess(const char *pathname, int mode)
{
    struct stat statbuf;
    uid_t euid;
    gid_t egid;

    mode_t mask;

    euid = geteuid();
    egid = getegid();

    if (stat(pathname, &statbuf) == -1)
        return -1;

    if (statbuf.st_uid == euid)
        if ((((statbuf.st_mode >> 6) & 0x7) & mode) == mode)
            return 0;

    if (statbuf.st_uid == egid)
        if ((((statbuf.st_mode >> 3) & 0x7) & mode) == mode)
            return 0;

    if ((((statbuf.st_mode) & 0x7) & mode) == mode)
        return 0;

    return -1;
}

int
main(int argc, char *argv[])
{

    if (eaccess(argv[1], atoi(argv[2])) == 0)
        exit(EXIT_SUCCESS);
    exit(EXIT_FAILURE);
}
