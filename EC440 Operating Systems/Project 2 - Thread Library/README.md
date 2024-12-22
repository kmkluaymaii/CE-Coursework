## Project 2: Preemptive User Mode Threading Library

The functions of this code to:
    1. Create a thread (pthread.create())
    2. Exit a thread once it's done (pthread.exit())
    3. Schedule the next ready thread every 50 second (schedule() & alarm_handler())
    4. Return the id of the current thread (pthread_self())

In pthread.create(), 
    1. If this is the first time pthread.create() is being called then create a thread control block for main and also starts the alarm_handler and set the status to RUNNING.
    2. Continue creating a thread by creating a new thread control block for each thread, including create a new id, create a new stack,  and set the status to READY. In the top of the stack store the pthread_exit.
    3. Setjmp the context of the current thread.

In pthread_exit()
    1. If the current running thread finishes running and the pthread_exit got called, this function would set the status to EXITED, deallocates the stack, decrement the thread counter and triggers the scheduler to switch to the next thread.

In pthread_self(),
    1. Returns the ID of the currently running thread.

In schedule()
    1. If something is running, set it to being ready.
    2. If there is only main left, then continues running main until it ends.
    3. If the thread has been called longjmp before then the function returns immediately, allowing the resumed thread to continue execution from where it was previously suspended.
    4. Find the next ready thread and then set its status to be RUNNING.
    5. Longjmp to the next thread.

In alarm_handler()
    1. Sets up the SIGALRM signal to trigger preemptive scheduling every 50ms.