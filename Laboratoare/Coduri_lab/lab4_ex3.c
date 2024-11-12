#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

void collatz(int n) {
    printf("%d: ", n);
    while (n != 1) {
        printf("%d ", n);
        if (n % 2 == 0) {
            n = n / 2;
        } else {
            n = 3 * n + 1;
        }
    }
    printf("1.\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number>\n", argv[0]);
        return 1;
    }

    int number = atoi(argv[1]);
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        return errno;
    } else if (pid == 0) {
        // Child process
        if(number < 1)
        {
             perror("stop"); 
        }
        else{
        collatz(number);
        exit(0);
        }
    } else {
        // Parent process
        wait(NULL);
        printf("Child %d finished\n", pid);
    }
    return 0;
}
