#include <unistd.h>
#include <stdio.h>

int main()
{
    char *argv[] = {"pwd", NULL};
    execve("/usr/bin/pwd", argv, NULL);
    printf("After execve");
}