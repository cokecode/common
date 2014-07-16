// Utility for inspecting exit value.
// zhangxun@taobao.com
// 2012.2

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int status;

    if (argc == 1) {
        fprintf(stderr, "Usage: %s exit-status-value\n", argv[0]);
        return 1;
    }

#define PRINT(x) printf("%-20s = %d\n", #x, x)
    status = strtol(argv[1], NULL, 0);
    printf("status = %d\n", status);
    PRINT(WIFEXITED(status));
    PRINT(WEXITSTATUS(status));
    PRINT(WIFSIGNALED(status));
    PRINT(WTERMSIG(status));
    PRINT(WCOREDUMP(status));
    PRINT(WIFSTOPPED(status));
    PRINT(WSTOPSIG(status));
    PRINT(WIFCONTINUED(status));
    printf("\n");
    printf("'man 2 wait' to show the details of macros above.\n");
    printf("'kill -l' to show signal list.\n");
    printf("\n");

    return 0;
}

// vim:ts=4:sw=4:ai:et
