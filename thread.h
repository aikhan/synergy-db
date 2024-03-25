#ifndef THREAD_H
#define THREAD_H

#include <setjmp.h>

#define STACK_SIZE 1024
#define MAX_THREADS 10

typedef struct Thread
{
    jmp_buf env;            // For saving and restoring context
    char stack[STACK_SIZE]; // Stack for the thread
    // Other thread-related data...
} Thread;

Thread *createThread(void (*function)());

#endif // THREAD_H