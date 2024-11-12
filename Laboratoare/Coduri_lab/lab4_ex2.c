#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

int main() {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        return errno;
    } else if (pid == 0) {
        // Child process
        char *argv[] = {"ls", NULL};
        execve("/bin/ls", argv, NULL);
        perror("execve failed"); 
        exit(1);
    } else {
        // Parent process
        printf("My PID = %d, Child PID = %d\n", getpid(), pid);
        wait(NULL); 
        printf("Child %d finished\n", pid);
    }
    return 0;
}
