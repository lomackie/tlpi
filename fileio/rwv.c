#include "error_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <fcntl.h>
#include "tlpi_hdr.h"
#include <string.h>

ssize_t
myreadv(int fd, const struct iovec *iov, int iovcnt)
{
    ssize_t numRead;
    size_t totalLength = 0;
    size_t totalCpyLength = 0;

    for (int i = 0; i < iovcnt; i = i + 1)
    {
        totalLength += iov[i].iov_len;
    }

    char *str = malloc(totalLength);
    if (str == NULL)
        return -1;

    numRead = read(fd, str, totalLength);
    if (numRead == -1)
    {
        free(str);
        return -1;
   }

    for (int i = 0; i < iovcnt && totalCpyLength < (size_t) numRead; i += 1)
    {
        size_t toCpy = iov[i].iov_len;
        if (totalCpyLength + toCpy > (size_t) numRead)
            toCpy = numRead - totalCpyLength;

        memcpy(iov[i].iov_base, str + totalCpyLength, iov[i].iov_len);
        totalCpyLength += iov[i].iov_len;
    }

    free(str);
    return numRead;
}

ssize_t
mywritev(int fd, const struct iovec *iov, int iovcnt)
{
    ssize_t numWritten;
    size_t totalLength = 0;
    size_t cpyOffset = 0;

    for (int i = 0; i < iovcnt; i += 1)
    {
        totalLength += iov[i].iov_len;
    }

    char *str = malloc(totalLength);
    if (str == NULL)
        return -1;

    for (int i = 0; i < iovcnt; i += 1)
    {
        memcpy(str + cpyOffset, iov[i].iov_base, iov[i].iov_len);
        cpyOffset += iov[i].iov_len;
    }

    numWritten = write(fd, str, totalLength);
    free(str);
    return numWritten;
}

int
main(int argc, char *argv[])
{
    return 0;
}
