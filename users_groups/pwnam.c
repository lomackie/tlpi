#include <pwd.h>
#include "tlpi_hdr.h"

struct passwd
*mygetpwnam(const char *name)
{
    setpwent();
    struct passwd *pwd;
    while ((pwd = getpwent()) != NULL)
    {
        if (strcmp(pwd->pw_name, name) == 0)
            return pwd;
    }

    return NULL;
}

int
main(int argc, char *argv[])
{
    puts(mygetpwnam("nobody")->pw_shell);
}
