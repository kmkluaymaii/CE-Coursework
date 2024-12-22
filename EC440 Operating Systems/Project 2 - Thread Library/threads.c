#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>

#include "ec440threads.h"

#define MAX_THREAD 128
#define STACK_SIZE 32767
#define TIME 50 * 1000

#define JB_RBX 0
#define JB_RBP 1
#define JB_R12 2
#define JB_R13 3
#define JB_R14 4
#define JB_R15 5
#define JB_RSP 6
#define JB_PC 7

void *start_thunk();
unsigned long int ptr_mangle(unsigned long int p);
void schedule();
void alarm_handler();
struct sigaction sa;

enum thread_status{
    READY,
    RUNNING,
    EXITED
};

// Thread Control Block (TCB)
struct thread{
    int id;
    jmp_buf context; // store CPU's set of registers
    void* stack;
    enum thread_status status;
};

int num_thread = 0;
struct thread threads[MAX_THREAD];
int create_id = 1;
int current_thread_index = 0;


int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg){
    static int is_main = true;
    // Create thread for main + set alarm
    if(is_main){
        alarm_handler();
        is_main = false;
        threads[0].id = 0;
        threads[0].status = RUNNING;
        num_thread++;
    }

    if(num_thread >= MAX_THREAD) return -1;
    if (attr != NULL) return 1;

    int next_thread_index = create_id;
    threads[next_thread_index].id = create_id;

    *thread = threads[next_thread_index].id;
    num_thread++; 
    create_id++;

    threads[next_thread_index].stack = malloc(STACK_SIZE);
    if (threads[next_thread_index].stack == NULL) return -1;
    
    threads[next_thread_index].status = READY;

    void * stack_top = threads[next_thread_index].stack;
    stack_top = stack_top + STACK_SIZE - sizeof(void *);
    *(unsigned long int*)stack_top = (unsigned long int)pthread_exit;

    setjmp(threads[next_thread_index].context); // save current context
    threads[next_thread_index].context[0].__jmpbuf[JB_PC] = ptr_mangle((unsigned long long)start_thunk);
    threads[next_thread_index].context[0].__jmpbuf[JB_RSP] = ptr_mangle((unsigned long long)stack_top);

    threads[next_thread_index].context[0].__jmpbuf[JB_R12] = (unsigned long long)start_routine;
    threads[next_thread_index].context[0].__jmpbuf[JB_R13] = (unsigned long long)arg;

    return 0;


}

void pthread_exit(void *value_ptr){
    threads[current_thread_index].status = EXITED;
    free(threads[current_thread_index].stack);
    num_thread--;
    schedule();
    exit(0);
}


void schedule(){
    if(threads[current_thread_index].status == RUNNING){
        threads[current_thread_index].status = READY;
    }

    if(num_thread == 1){
        current_thread_index = 0;
        longjmp(threads[0].context, 1);	
    }

    if(setjmp(threads[current_thread_index].context)!=0){
        return;
    }
    //handle long jump return, just exit

    // Find the next ready thread
    while(1){
        current_thread_index++;	
        if(current_thread_index == MAX_THREAD-1){
            current_thread_index = 0;				
        } 

        if (threads[current_thread_index].status == READY){
            if(current_thread_index<create_id){
                break;  
            }
        }
    }
    threads[current_thread_index].status = RUNNING;
    longjmp(threads[current_thread_index].context, 1);	
    // jump to the next's thread context and run it
}

pthread_t pthread_self(void){
    return threads[current_thread_index].id;
}

void alarm_handler(){
    struct sigaction sa;
    sa.sa_handler = schedule;
    sa.sa_flags = SA_NODEFER;
    sigaction(SIGALRM, &sa, NULL);

    //Set timer
    ualarm(TIME, TIME);
}




