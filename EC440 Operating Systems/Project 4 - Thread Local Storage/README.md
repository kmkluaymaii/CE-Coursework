## Project 4: Thread Local Storage

This project involves implementing a thread-local storage (TLS) library that allows threads to manage their private memory. The library supports creating, destroying, reading, writing, and cloning TLS while handling synchronization and memory efficiency.

---

### Functional Overview

#### 1. **`tls_create`**
Creates a local storage area (LSA) for the current thread:
- Initializes the signal handler during the first invocation.
- Sets up a global data structure (adapted from `hash_table`) to associate thread IDs (TIDs) with their TLS for up to 128 threads.
- Allocates memory for TLS using `calloc` and `mmap`, initializing `size`, `page_num`, and reference count.
- Adds the TLS struct (containing TID and LSA) to the global data structure.

#### 2. **`tls_destroy`**
Destroys the current thread's TLS:
- Returns `-1` if no TLS exists for the thread.
- Unmaps and frees memory if pages are not shared; decrements the reference count if they are shared.
- Removes the thread's TLS struct from the global data structure.

#### 3. **`tls_read`**
Reads data from the TLS:
- Verifies that TLS exists and that the read operation is within bounds.
- Identifies the relevant pages and offsets, temporarily unprotects the pages, and reads the data.
- Reapplies protection after the read operation to maintain memory safety.

#### 4. **`tls_write`**
Writes data to the TLS:
- Verifies that TLS exists and that the write operation is within bounds.
- Handles **Copy-On-Write (COW)**:
  - If a shared page is being written to, creates a copy of the page using `memcpy`, decrements the reference count of the shared page, and performs the write on the new page.
- Temporarily unprotects the pages for writing and restores protection after completion.

#### 5. **`tls_clone`**
Clones the TLS of a target thread to the current thread:
- Ensures that both the current and target threads have valid TLS.
- Copies the structure of the target thread’s TLS, increments the reference count of shared pages, and associates the cloned TLS with the current thread in the global data structure.

