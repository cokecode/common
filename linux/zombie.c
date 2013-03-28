#include <pthread.h> 
#include <signal.h> 
#include <unistd.h> 
#include <stdio.h> 
static void* thread(void* dummy) { 
        /* Make sure that we exit as second thread */ 
        sleep(1); 
        return NULL; 
} 
void main(void) { 
        int pid; 
        signal(SIGCHLD, SIG_IGN); 
        pid = fork(); 
        if (pid == 0) { 
                pthread_t p; 
                pthread_create(&p, NULL, thread, NULL); 
        } else { 
                /* Sleep some time so we know that child threads exit before us */ 
                sleep(2); 
                printf("Look for task %d...\n", pid); 
        } 
} 
