#include <stdlib.h>
#include <string.h>
#include "tlpi_hdr.h"

extern char **environ;

int
mysetenv(const char *name, const char *value, int overwrite)
{
    if (getenv(name) != NULL && overwrite)
        return 0;
    char *buf = malloc(strlen(name) + strlen(value) + 2);
    if (buf == NULL)
        return -1;
    char *p = buf;

    strcpy(p, name);
    p += strlen(name);
    *p = '=';
    p += 1;
    strcpy(p, value);
    p += strlen(value);
    *p = '\0';

    if(putenv(buf) < 0)
    {
        free(buf);
        return -1;
    }

    return 0;
}

int
myunsetenv(const char *name)
{
    char **ep;
    size_t len = strlen(name);
    for (ep = environ; *ep != NULL; )
    {
        if (strncmp(name, *ep, len) == 0 && (*ep)[len] == '=')
        {
           char **ep2;
            for (ep2 = ep; *ep2 != NULL; ep2++)
                *ep2 = *(ep2+1);
        } else {
            ep++;
        }
    }
    return 0;
}

int
main(int argc, char *argv[])
{
    int j;
    char **ep;

    for (j = 1; j < argc; j++)
        if (putenv(argv[j]) != 0)
            errExit("putenv: %s", argv[j]);

    if (mysetenv("GREET", "Hello world", 0) == -1)
        errExit("mysetenv: %s", argv[j]);

    myunsetenv("BYE");

    for (ep = environ; *ep != NULL; ep++)
        puts(*ep);

    exit(EXIT_SUCCESS);
}
