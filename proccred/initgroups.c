#define _BSD_SOURCE
#include <grp.h>
#include "tlpi_hdr.h"

int
myinitgroups(const char *user, gid_t group)
{
    struct group *grp;
    gid_t grouplist[NGROUPS_MAX + 1];
    int gidsetsize = 0;

    grouplist[gidsetsize++] = group;
    setgrent();
    while ((grp = getgrent()) != NULL)
    {
        char **pgr_mem;
        for (pgr_mem = grp->gr_mem; *pgr_mem != NULL; pgr_mem++)
        {
            if (strcmp(user, *pgr_mem) == 0)
            {
                grouplist[gidsetsize] = grp->gr_gid;
                gidsetsize++;
            }
        }
    }
    setgroups(gidsetsize, grouplist);
    endgrent();
    return 0;
}

int
main(int argc, char *argv[])
{
    int gidsetsize = NGROUPS_MAX+1;
    gid_t grouplist[NGROUPS_MAX + 1];

    for (int i = 0; i < getgroups(gidsetsize, grouplist); ++i)
        printf("group[%d] = %d\n", i, grouplist[i]);

    myinitgroups("louismackie", 0);

    for (int i = 0; i < getgroups(gidsetsize, grouplist); ++i)
        printf("group[%d] = %d\n", i, grouplist[i]);
}
