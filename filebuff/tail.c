#include "error_functions.h"
#include "tlpi_hdr.h"
#include <sys/fcntl.h>

#ifndef BUF_SIZE
#define BUF_SIZE 4096
#endif

int main(int argc, char *argv[]) {
    int opt, num, fd, bufPos;
    ssize_t numRead;
    off_t pos, toRead;
    char buf[BUF_SIZE];

    num = 10;

    while ((opt = getopt(argc, argv, "n:")) != -1) {
        if (opt == 'n')
            num = atoi(optarg);
    }

    if (num <= 0)
        exit(EXIT_SUCCESS);

    fd = open(argv[optind], O_RDONLY);
    if (fd == -1)
        errExit("opening file %s", argv[optind]);

    lseek(fd, -1, SEEK_END);
    read(fd, buf, 1);
    if (buf[0] == '\n')
        num++;

    pos = lseek(fd, 0, SEEK_END);

    while (pos > 0)
    {
        toRead = min(pos, sizeof(buf));
        pos = lseek(fd, -1*toRead, SEEK_CUR);

        numRead = read(fd, buf, toRead);
        if (numRead == -1)
            errExit("read");
        lseek(fd, pos, SEEK_SET);


        bufPos = numRead - 1;
        while (bufPos >= 0) {
            if (buf[bufPos] == '\n')
                if (--num == 0)
                    break;
            bufPos--;
        }
        if (num == 0)
        {
            bufPos++;
            break;
        }
    }

    lseek(fd, pos+bufPos, SEEK_SET);
    while ((numRead = read(fd, buf, BUF_SIZE)) > 0)
    {
        write(STDOUT_FILENO, buf, numRead);
    }

    exit(EXIT_SUCCESS);
}
