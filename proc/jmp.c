#include <setjmp.h>

static jmp_buf env;

int
f1()
{
    return setjmp(env);
}

int
main(int argc, char *argv[])
{
    int v = f1();
    longjmp(env, v);
}
