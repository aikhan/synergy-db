#include <setjmp.h>
#include <stdlib.h>

#define STACK_SIZE 1024
#define MAX_THREADS 10

typedef struct Thread
{
    jmp_buf env;            // For saving and restoring context
    char stack[STACK_SIZE]; // Stack for the thread
    // Other thread-related data...
} Thread;

Thread *threads[MAX_THREADS];
int currentThread = 0;

Thread *createThread(void (*function)())
{
    Thread *thread = (Thread *)malloc(sizeof(Thread));
    if (setjmp(thread->env) == 0)
    {
        // Set up the new thread's stack
        ((char *)&thread->env)[0] = (char)function;
        ((char *)&thread->env)[1] = (char)(thread->stack + STACK_SIZE);
    }
    return thread;
}