#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Define the structure for a thread
typedef struct
{
    pid_t pid; // Process ID
} my_thread_t;

// Function to be executed by each thread
void thread_function()
{
    printf("Hello from thread %d\n", getpid());
}

// Create a thread
int my_thread_create(my_thread_t *thread)
{
    thread->pid = fork();
    if (thread->pid < 0)
    {
        perror("fork");
        return -1;
    }
    else if (thread->pid == 0)
    { // Child process
        thread_function();
        exit(0);
    }
    return 0;
}

// Join a thread
int my_thread_join(my_thread_t *thread)
{
    int status;
    waitpid(thread->pid, &status, 0);
    return 0;
}

int main()
{
    my_thread_t thread;

    // Create and join a thread
    if (my_thread_create(&thread) == 0)
    {
        my_thread_join(&thread);
    }

    return 0;
}
