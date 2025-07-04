#include "error_functions.h"
#include "tlpi_hdr.h"
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

char *myrealpath(const char *pathname, char *resolved_path) {
    char buf[PATH_MAX];
    ssize_t numBytes;
    int len, symlink_count;

    symlink_count = 0;

    if (pathname[0] != '/') {
        if (getcwd(buf, PATH_MAX - 1) == NULL)
            errExit("getcwd");
        strcpy(resolved_path, buf);
        size_t len = strlen(buf);
        resolved_path[len] = '/';
        strcpy(resolved_path + len + 1, pathname);
    } else
        strcpy(resolved_path, pathname);

    chdir("/");

    struct stack *toks = malloc(sizeof(struct stack));
    toks->prev = NULL;
    toks->val = NULL;

    for (int i = 1; i < strlen(resolved_path);) {
        char *p = strchr(resolved_path + i, '/');
        if (p)
            len = p - (resolved_path + i);
        else
            len = strlen(resolved_path + i);

        if (len == 0) {
            i++;
            continue;
        }
        char current_ref[len + 1];
        strncpy(current_ref, resolved_path + i, len);
        current_ref[len] = '\0';

        if (strcmp(".", current_ref) == 0)
            ;
        else if (strcmp("..", current_ref) == 0) {
            char *fp = pop(&toks);
            chdir(fp);
            free(fp);
        } else {
            struct stat st;
            lstat(current_ref, &st);

            if (S_ISLNK(st.st_mode)) {
                if (++symlink_count >= 40)
                    errExit("TOO MANY SYM LINKS");

                numBytes = readlink(current_ref, buf, PATH_MAX - 1);

                if (numBytes == -1)
                    errExit("readlink");
                buf[numBytes] = '\0';

                char suffix_buf[PATH_MAX];
                strcpy(suffix_buf, resolved_path + i + len);

                if (buf[0] == '/') {
                    strcpy(resolved_path, buf);
                    strcpy(resolved_path + strlen(buf), suffix_buf);
                } else {
                    strcpy(resolved_path + i, buf);
                    strcpy(resolved_path + i + strlen(buf), suffix_buf);
                }

                i = 1;
                clear(&toks);
                chdir("/");
                continue;
            }
            if (S_ISDIR(st.st_mode)) {
                chdir(current_ref);
            }
            push(current_ref, &toks);
        }

        i += len + 1;
    }

    memset(resolved_path, 0, PATH_MAX);
    resolved_path[0] = '/';

    struct stack *rev_toks = malloc(sizeof(struct stack));
    while (toks->val) {
        char *fp = pop(&toks);
        push(fp, &rev_toks);
    }
    free(toks);

    int i = 1;
    while (rev_toks->val) {
        char *fp = pop(&rev_toks);
        strcpy(resolved_path + i, fp);
        i += strlen(fp);
        resolved_path[i++] = '/';
        free(fp);
    }
    free(rev_toks);

    if (strlen(resolved_path) > 0) {
        resolved_path[strlen(resolved_path) - 1] = '\0';
    }
    return resolved_path;
}

int main(int argc, char *argv[]) {
    char buf[BUF_SIZE];

    if (myrealpath(argv[1], buf) == NULL)
        errExit("realpath");
    printf("realpath: %s --> %s\n", argv[1], buf);

    exit(EXIT_SUCCESS);
}
