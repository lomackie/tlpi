#include "tlpi_hdr.h"
#include <dirent.h>
#include <stddef.h>

static void /* List all files in directory 'dirPath' */
listFiles(const char *dirpath) {
    DIR *dirp;
    struct dirent *entryp;
    struct dirent *result;
    size_t len;

    len = offsetof(struct dirent, d_name) + NAME_MAX + 1;
    entryp = malloc(len);

    Boolean isCurrent; /* True if 'dirpath' is "." */
    isCurrent = strcmp(dirpath, ".") == 0;
    dirp = opendir(dirpath);

    if (dirp == NULL) {
        errMsg("opendir failed on '%s'", dirpath);
        return;
    }
    /* For each entry in this directory, print directory + filename */
    for (;;) {
        if (readdir_r(dirp, entryp, &result) != 0) {
            errExit("readdir_r");
        }
        if (result == NULL)
            break;
        if (strcmp(entryp->d_name, ".") == 0 ||
            strcmp(entryp->d_name, "..") == 0)
            continue; /* Skip . and .. */
        if (!isCurrent)
            printf("%s/", dirpath);
        printf("%s\n", entryp->d_name);
    }
    if (closedir(dirp) == -1)
        errMsg("closedir");
}

int main(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "--help") == 0)
        usageErr("%s [dir...]\n", argv[0]);
    if (argc == 1) /* No arguments - use current directory */
        listFiles(".");
    else
        for (argv++; *argv; argv++)
            listFiles(*argv);
    exit(EXIT_SUCCESS);
}
