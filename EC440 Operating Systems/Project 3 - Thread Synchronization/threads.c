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
#include <semaphore.h>

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
    EXITED,
    BLOCKED
};

// Thread Control Block (TCB)
struct thread{
    int id;
    jmp_buf context; // set of registers
    void* stack;
    enum thread_status status;
    void* exit;
    int wait;
};

// Semaphore
struct semaphore{
    // sem_t *sem_ptr;
    int sem_value;
    int sem_flag;
    int sem_queue[MAX_THREAD];
    int sem_count;
    int queue_front;
    int queue_back;
};

int num_thread = 0;
int num_semaphore = 0;
struct thread threads[MAX_THREAD];
struct semaphore semaphore[MAX_THREAD];
int create_id = 1;
int current_thread_index = 0;
int block_thread_index = 0;
sigset_t siglock;

void lock(){
  sigemptyset(&siglock);
  sigaddset(&siglock, SIGALRM);
  sigprocmask(SIG_BLOCK, &siglock, NULL);
}

void unlock(){
  sigemptyset(&siglock);
  sigaddset(&siglock, SIGALRM);
  sigprocmask(SIG_UNBLOCK, &siglock, NULL);
}

void pthread_exit_wrapper()
{
    unsigned long int res;
    asm("movq %%rax, %0\n":"=r"(res));
    pthread_exit((void *) res);
}

int sem_init(sem_t *sem, int pshared, unsigned value){
    lock();

    if (pshared != 0) return -1;
    int index = -1;
    for(index = 1; index < MAX_THREAD; index++){
        if(semaphore[index].sem_flag == 0){
            break;
        }
    }

    if (index == MAX_THREAD) {
        unlock();
        return -1; 
    }
    sem->__align = index;
    semaphore[index].sem_flag = 1;
    semaphore[index].sem_value = value;
    //semaphore[index].sem_ptr = sem;
    semaphore[index].sem_count = 0;
    semaphore[index].queue_front = 0;
    semaphore[index].queue_back = 0;
    unlock();
    return 0;   
}

int sem_wait(sem_t *sem){
    lock();
    int index = sem->__align;
    while(semaphore[index].sem_value == 0){  //Thread being blocked 
        semaphore[index].sem_queue[semaphore[index].queue_back] = current_thread_index;
        semaphore[index].queue_back = (semaphore[index].queue_back + 1) % MAX_THREAD;
        semaphore[index].sem_count++;
        threads[current_thread_index].status = BLOCKED;
        unlock();
        schedule();
        lock();
	} 
    semaphore[index].sem_value--;
    unlock();
    return 0;
}

int sem_post(sem_t *sem){
    lock();
    int index = sem->__align;
    semaphore[index].sem_value++;
    if(semaphore[index].sem_count > 0){ //&& semaphore[index].sem_value <= 0
        int next_thread = semaphore[index].sem_queue[semaphore[index].queue_front];
        semaphore[index].queue_front = (semaphore[index].queue_front + 1) % MAX_THREAD;
		semaphore[index].sem_count--;	
        threads[next_thread].status = READY;	
    }
    unlock();
    return 0;	
}


int sem_destroy(sem_t *sem){
    lock();
    int index = sem->__align;
    if(semaphore[index].sem_flag == 1){
        semaphore[index].sem_flag = 0;
    }
    unlock();
    return 0;	
}


int pthread_join(pthread_t thread, void **value_ptr){
    if (threads[thread].status != EXITED) {
        threads[current_thread_index].wait = thread;
        threads[current_thread_index].status = BLOCKED;


        while(threads[thread].status != EXITED){
            schedule();
        }
    }
   
    if(value_ptr != NULL){
        *value_ptr = threads[thread].exit;
    } 
    

    free(threads[thread].stack);
    return 0;
}

int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg){
    static int is_main = true;

    // Create thread for main + set alarm
    if(is_main){
        alarm_handler();
        is_main = false;
        threads[0].id = 0;
        threads[0].status = RUNNING;
        threads[0].exit = NULL;
        threads[0].wait = -1; 
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
    threads[next_thread_index].wait = -1; 
    threads[next_thread_index].exit = NULL;

    void * stack_top = threads[next_thread_index].stack;
    stack_top = stack_top + STACK_SIZE - sizeof(void *);
    *(unsigned long int*)stack_top = (unsigned long int)pthread_exit_wrapper;

    setjmp(threads[next_thread_index].context);
    threads[next_thread_index].context[0].__jmpbuf[JB_PC] = ptr_mangle((unsigned long long)start_thunk);
    threads[next_thread_index].context[0].__jmpbuf[JB_RSP] = ptr_mangle((unsigned long long)stack_top);

    threads[next_thread_index].context[0].__jmpbuf[JB_R12] = (unsigned long long)start_routine;
    threads[next_thread_index].context[0].__jmpbuf[JB_R13] = (unsigned long long)arg;
    return 0;


}

void pthread_exit(void *value_ptr){
    threads[current_thread_index].status = EXITED;
    threads[current_thread_index].exit = value_ptr;
    num_thread--;
    int i = 0;
    for(i = 0; i < MAX_THREAD ; i++){
        if (threads[i].wait == current_thread_index && threads[i].status == BLOCKED){
            threads[i].status = READY;
            threads[i].wait = -1;
            break;
        }
    }
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
