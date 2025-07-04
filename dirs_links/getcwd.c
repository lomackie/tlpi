#include "error_functions.h"
#include "tlpi_hdr.h"
#include <dirent.h>
#include <fcntl.h>
#include <limits.h> /* For definition of PATH_MAX */
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define BUF_SIZE PATH_MAX

struct stack {
    struct stack *prev;
    char *val;
};

char *pop(struct stack **stack) {
    struct stack *p = *stack;
    char *ret = (*stack)->val;

    *stack = (*stack)->prev;
    free(p);

    return ret;
}

void push(char *val, struct stack **stack) {
    struct stack *new = malloc(sizeof(struct stack));
    char *mval = malloc(sizeof(char) * strlen(val));
    strcpy(mval, val);
    new->val = mval;
    new->prev = *stack;
    *stack = new;
}

void clear(struct stack **stack) {
    while ((*stack)->val) {
        char *val = pop(stack);
        free(val);
    }
}

char *mygetcwd() {
    struct stat sb;
    int fd;
    char *result = malloc(PATH_MAX);
    dev_t st_dev;
    ino_t st_ino;
    struct stack *toks = malloc(sizeof(struct stack));

    fd = open(".", O_RDONLY);
    stat(".", &sb);
    st_dev = sb.st_dev;
    st_ino = sb.st_ino;
    chdir("..");

    Boolean root = 0;

    while (!root) {
        DIR *dirp;
        struct dirent *dp;
        dirp = opendir(".");

        for (;;) {
            errno = 0;
            dp = readdir(dirp);
            if (dp == NULL)
                break;
            stat(dp->d_name, &sb);
            if (sb.st_dev == st_dev && sb.st_ino == st_ino) {
                if (strcmp(dp->d_name, ".") == 0) {
                    root = 1;
                    break;
                }
                push(dp->d_name, &toks);
                closedir(dirp);
                break;
            }
        }

        stat(".", &sb);
        st_dev = sb.st_dev;
        st_ino = sb.st_ino;
        chdir("..");
    }

    result[0] = '/';

    int i = 1;
    while (toks->val) {
        char *fp = pop(&toks);
        strcpy(result + i, fp);
        i += strlen(fp);
        result[i++] = '/';
        free(fp);
    }
    free(toks);

    if (strlen(result) > 0) {
        result[strlen(result) - 1] = '\0';
    }

    fchdir(fd);
    return result;
}

int main(int argc, char *argv[]) {
    char *cwd = mygetcwd();
    puts(cwd);
    free(cwd);
    exit(EXIT_SUCCESS);
}
