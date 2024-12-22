#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>

#define MAX_THREAD 128
#define PAGE_SIZE 4096

typedef struct thread_local_storage{
    pthread_t tid;
    unsigned int size; /* size in bytes */
    unsigned int page_num; /* number of pages */
    struct page **pages; /* array of pointers to pages */
} TLS;

struct page {
    void* address; /* start address of page */
    int ref_count; /* counter for shared pages */
};

struct tls_tid {
    pthread_t tid;
    TLS *tls;
};

/* Global Variable*/
struct tls_tid *threads[MAX_THREAD];
bool initialized = 0;
bool tls_tid_initialized = 0;
uintptr_t page_size = 0;
int i = 0;
int j = 0;
unsigned int page_number = 0, page_offset = 0;

void tls_handle_page_fault(int sig, siginfo_t *si, void *context);

void tls_protect(struct page *p){
    if (mprotect((void *) p->address, page_size, 0)) {
        fprintf(stderr, "tls_protect: could not protect page\n");
        exit(1);
    }
}

void tls_unprotect(struct page *p){
    if (mprotect((void *) p->address, page_size, PROT_READ | PROT_WRITE)) {
        fprintf(stderr, "tls_unprotect: could not unprotect page\n");
        exit(1);
    }
}

void tls_init(){
    struct sigaction sigact;
    /* get the size of a page */
    page_size = getpagesize();
    /* install the signal handler for page faults (SIGSEGV, SIGBUS) */
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = SA_SIGINFO; /* use extended signal handling */
    sigact.sa_sigaction = tls_handle_page_fault;
    sigaction(SIGBUS, &sigact, NULL);
    sigaction(SIGSEGV, &sigact, NULL);
    initialized = 1;
}

/* Handle SIGSEV, thread create seg_fault*/
void tls_handle_page_fault(int sig, siginfo_t *si, void *context){ 
    uintptr_t p_fault = ((uintptr_t) si->si_addr) & ~(page_size - 1); 
    struct page *p = NULL;
    for (i = 0; i < MAX_THREAD; i++) {
        if (threads[i] && threads[i]->tls) {
            for (j = 0; j < threads[i]->tls->page_num; j++) {
                if ((uintptr_t)threads[i]->tls->pages[j]->address == p_fault) {
                    p = threads[i]->tls->pages[j];
                    break;
                }
            }
        }
        if (p) break;
    }
    if (p && ((uintptr_t)p->address == p_fault)) {
        pthread_exit(NULL); // kill the thread
    }
    signal(SIGSEGV, SIG_DFL);
    signal(SIGBUS, SIG_DFL);
    raise(sig);
}

int tls_create(unsigned int size){
    if (size <= 0){
        return -1;
    }
    if (!initialized){
        tls_init();
    }

    if(!tls_tid_initialized){
        for (i = 0; i < MAX_THREAD; i++){
		    threads[i] = NULL;
        }
        tls_tid_initialized = 1;
    }

    //Create thread that already have LSA
    for(i = 0; i < MAX_THREAD ; i++){
        if(threads[i] != NULL && threads[i]->tid == pthread_self() && threads[i]->tls != NULL){
            return -1;
        }
    }

    TLS *tls = calloc(1, sizeof(struct thread_local_storage));
    tls -> tid = pthread_self();
    tls->size = size;
    tls -> page_num = size / PAGE_SIZE;
    if (size % PAGE_SIZE > 0){
		tls->page_num++;
	}
    tls->pages = calloc(tls->page_num, sizeof(struct page *));

    for(i = 0; i < tls -> page_num; i++){
        struct page *p;
        tls->pages[i] = malloc(sizeof(struct page));
        p = tls->pages[i]; 
        p->address = mmap(0,
        PAGE_SIZE, 0, MAP_ANON | MAP_PRIVATE,
        0, 0); // No permission to read or write 
        p->ref_count = 1;
        tls->pages[i] = p;
    }

    for(i = 0; i < MAX_THREAD ; i++){
        if(threads[i] == NULL){
            threads[i] = malloc(sizeof(struct tls_tid));
            threads[i]->tid = pthread_self();
            threads[i]->tls = tls;
            break;
        }
    }

    return 0;
}

int tls_destroy(){
    //Destroy only after thread is initialized
    if (!initialized || !tls_tid_initialized) {
        return -1; 
    }
    
    for(i = 0; i < MAX_THREAD ; i++){
        //Destroy thread that has no LSA
        if(threads[i] != NULL && threads[i]->tid == pthread_self() && threads[i]->tls == NULL){
            return -1;
        }

        //Destroy thread that already has LSA
        if(threads[i] != NULL && threads[i]->tid == pthread_self() && threads[i]->tls != NULL){
            for (j = 0; j < threads[i]->tls->page_num; ++j){
                // Find the not shared page, and free them
                if (threads[i]->tls->pages[j]->ref_count == 1){
                    munmap(threads[i]->tls->pages[j]->address, PAGE_SIZE);
                    free(threads[i]->tls->pages[j]);
                    free(threads[i]->tls->pages);
                    free(threads[i]->tls);
                    free(threads[i]);
                    threads[i] = NULL;
                    break;
                } else {
                    threads[i]->tls->pages[j]->ref_count--;
                    break;
                }
            }
            return 0;
        }
    }

    return -1;
}

int tls_read(unsigned int offset, unsigned int length, char *buffer){
    //Read only after thread is initialized (Test Case 0)
    if (!initialized || !tls_tid_initialized) {
        return -1; 
    }

    for(i = 0; i < MAX_THREAD ; i++){
        //Read thread that has no LSA
        if(threads[i] != NULL && threads[i]->tid == pthread_self() && threads[i]->tls == NULL){
            return -1;
        }

        //Read thread that has LSA but too small
        if(((offset + length) > threads[i]->tls->size) && threads[i] != NULL && threads[i]->tid == pthread_self() && threads[i]->tls != NULL){
            return -1;
        }

        //Read thread that already has LSA with enough space
        if(((offset + length) <= threads[i]->tls->size) && threads[i] != NULL && threads[i]->tid == pthread_self() && threads[i]->tls != NULL){
            int cnt = 0;
            char *src;

            for(cnt = 0, j = offset; j < (offset + length); ++cnt, ++j){ //for cong purpose
                struct page *p;
                page_number = j / PAGE_SIZE;
                page_offset = j % PAGE_SIZE;
                p = threads[i]->tls->pages[page_number];
                tls_unprotect(p);
                src  = (char *) p->address + page_offset;
                buffer[cnt] = *src;
            }

            for(j = offset; j < (offset + length); ++j){
                page_number = j / PAGE_SIZE;
                tls_protect(threads[i]->tls->pages[page_number]);
            }

            break;
        }
    }


    return 0;
}

int tls_write(unsigned int offset, unsigned int length, char *buffer){
    //Write only after thread is initialized
    if (!initialized || !tls_tid_initialized) {
        return -1; 
    }

    for(i = 0; i < MAX_THREAD ; i++){
        //Write thread that has no LSA
        if(threads[i] != NULL && threads[i]->tid == pthread_self() && threads[i]->tls == NULL){
            return -1;
        }

        //Write thread that has LSA but too small
        if(((offset + length) > threads[i]->tls->size) && threads[i] != NULL && threads[i]->tid == pthread_self() && threads[i]->tls != NULL){
            return -1;
        }

        //Write thread that already has LSA with enough space
        if(((offset + length) <= threads[i]->tls->size) && threads[i] != NULL && threads[i]->tid == pthread_self() && threads[i]->tls != NULL){
            int cnt = 0;
            char *dst;

            for(cnt = 0, j = offset; j < (offset + length); ++cnt, ++j){
                struct page *p, *copy;
                page_number = j / PAGE_SIZE;
                page_offset = j % PAGE_SIZE;
                p = threads[i]->tls->pages[page_number];
                tls_unprotect(p);
                if (p->ref_count > 1) {
                    /* this page is shared, create a private copy (COW) */ //save memory
                    copy = (struct page *) calloc(1, sizeof(struct page));
                    copy->address = mmap(0,
                    page_size, PROT_WRITE,
                    MAP_ANON | MAP_PRIVATE, 0, 0);

                    memcpy(copy->address, p->address, PAGE_SIZE);

                    copy->ref_count = 1;
                    threads[i]->tls->pages[page_number] = copy;

                    /* update original page */
                    p->ref_count--;
                    tls_protect(p);
                    p = copy;
                }
                dst = ((char *) p->address) + page_offset;
                *dst = buffer[cnt];
            }


            for(j = offset; j < (offset + length); ++j){
                page_number = j / PAGE_SIZE;
                tls_protect(threads[i]->tls->pages[page_number]);
            }

            break;
        }
    }
    

    return 0;
}

int tls_clone(pthread_t tid){
    //Clone only after thread is initialized
    if (!initialized || !tls_tid_initialized) {
        return -1; 
    }

    for(i = 0; i < MAX_THREAD ; i++){
        //Target thread doesn't exist (Test Case 10)
        if(threads[i] == NULL){
            return -1;
        }
        //Current thread already has a LSA
        if(threads[i] != NULL && threads[i]->tid == pthread_self() && threads[i]->tls != NULL){
            return -1;
        }

        //Target thread to clone has no LSA
        if(threads[i] != NULL && threads[i]->tid == tid && threads[i]->tls == NULL){
            return -1;
        }

        if(threads[i] != NULL && threads[i]->tid == tid && threads[i]->tls != NULL){
           TLS *tls = calloc(1, sizeof(struct thread_local_storage));
            tls -> tid = pthread_self();
            tls -> size = threads[i]->tls->size;
            tls -> page_num = threads[i]->tls->page_num;
            tls->pages = calloc(tls->page_num, sizeof(struct page *));
            for (j = 0; j < tls->page_num; j++){
                tls->pages[j] = threads[i]->tls->pages[j];
                tls->pages[j]->ref_count++;
            }

            for(i = 0; i < MAX_THREAD ; i++){
                if(threads[i] == NULL){
                    threads[i] = malloc(sizeof(struct tls_tid));
                    threads[i]->tid = pthread_self();
                    threads[i]->tls = tls;
                    break;
                }
            }
            
            break;
	    }

    }
    return 0;
}

