## Project 2: Preemptive User Mode Threading Library

This project implements a user-mode threading library that supports preemptive multitasking with the following functionalities:
- **Thread Creation**: `pthread_create()`
- **Thread Exit**: `pthread_exit()`
- **Preemptive Scheduling**: `schedule()` and `alarm_handler()`
- **Thread ID Retrieval**: `pthread_self()`

---

### Functional Overview

#### 1. **Thread Creation (`pthread_create`)**
The `pthread_create()` function manages thread initialization:
1. **First Invocation**:
   - Creates a thread control block (TCB) for the `main` thread.
   - Starts the `alarm_handler` to enable preemptive scheduling.
   - Sets the `main` thread's status to `RUNNING`.
2. **Subsequent Calls**:
   - Allocates a TCB for the new thread.
   - Generates a unique thread ID.
   - Allocates a new stack for the thread and sets the status to `READY`.
   - Stores the `pthread_exit()` function at the top of the stack.
   - Captures the current thread's context using `setjmp()`.

---

#### 2. **Thread Exit (`pthread_exit`)**
The `pthread_exit()` function handles thread termination:
1. Sets the current thread's status to `EXITED`.
2. Deallocates the thread's stack and decrements the thread counter.
3. Invokes the scheduler to switch to the next ready thread.

---

#### 3. **Thread ID Retrieval (`pthread_self`)**
The `pthread_self()` function:
- Returns the ID of the currently running thread.

---

#### 4. **Scheduling (`schedule`)**
The `schedule()` function orchestrates thread switching:
1. Sets the status of the currently running thread to `READY`.
2. If only the `main` thread remains, continues running it until termination.
3. Uses `longjmp()` to resume a previously suspended thread.
4. Identifies the next `READY` thread, updates its status to `RUNNING`, and switches to it using `longjmp()`.

---

#### 5. **Preemptive Scheduling (`alarm_handler`)**
The `alarm_handler()` function:
1. Configures the `SIGALRM` signal to trigger every 50 milliseconds.
2. Initiates the `schedule()` function to switch threads preemptively.
