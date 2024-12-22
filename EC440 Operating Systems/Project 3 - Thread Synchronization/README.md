## Project 3: Thread Synchronization

This project builds upon the previous threading library by introducing semaphores to manage thread synchronization effectively. It also includes new thread operations and locking mechanisms to handle concurrent operations.

---

### Functional Overview

#### 1. **Semaphore Operations**
The project implements the following semaphore functions:
- **`sem_init()`**: Initializes a semaphore with a given value.
- **`sem_wait()`**: Decrements the semaphore value and blocks the thread if the value is less than 0.
- **`sem_post()`**: Increments the semaphore value and unblocks a waiting thread if necessary.
- **`sem_destroy()`**: Destroys a semaphore and frees its resources.

#### 2. **Thread Operations**
Incorporates threading functionalities from the previous project, including:
- **`pthread_create()`**: Creates a new thread.
- **`pthread_exit()`**: Exits a thread after execution.
- **`pthread_self()`**: Retrieves the current thread's ID.
- **`schedule()`**: Manages thread scheduling.
- **`alarm_handler()`**: Triggers preemptive scheduling.

#### 3. **New Features**
- **`pthread_join()`**:
  - Waits for a thread to finish execution.
  - Optionally retrieves the thread's return value using `value_ptr`.
  - Frees the thread’s stack memory upon exit.

- **`lock()` and `unlock()`**:
  - `lock()`: Blocks the `SIGALRM` signal to prevent preemptive scheduling during critical sections.
  - `unlock()`: Unblocks `SIGALRM`, allowing preemptive scheduling to resume.

---

### Semaphore Function Details

#### **1. `sem_init()`**
Initializes a semaphore:
1. Finds an unused semaphore by searching for `flag = 0`, then sets the flag to 1.
2. Initializes the queue, count, and value for the semaphore.

#### **2. `sem_wait()`**
Waits for the semaphore:
1. Decrements the semaphore value.
2. If the value is less than 0:
   - Blocks the calling thread and adds it to the semaphore queue.
   - Increments the queue and `sem_count`.
   - Triggers the `schedule()` function to switch to the next ready thread.

#### **3. `sem_post()`**
Signals the semaphore:
1. Increments the semaphore value.
2. If threads are waiting (`sem_count > 0`) and the value is non-positive:
   - Moves the first thread in the queue to the `READY` state for scheduling.

#### **4. `sem_destroy()`**
Destroys the semaphore:
- Sets the semaphore's `flag` back to 0, freeing it for reuse.
