## Project 5: File System


This project implements a simple file system (FS) on top of a virtual disk, providing basic file operations such as creation, deletion, reading, writing, and metadata management. The file system is designed to handle files efficiently using a custom superblock, FAT, and directory structure.

---

### Functional Overview

#### 1. **`make_fs`**
- Creates a file system on a virtual disk.
- Initializes the superblock, FAT, and directory structures:
  - **Superblock** contains:
    - Start and size of the FAT blocks.
    - Start and size of the directory blocks.
    - Start of the data blocks.
  - Data blocks start immediately after the directory blocks, maximizing usable space.
- Returns `-1` if the file system creation fails or the file already exists.

#### 2. **`mount_fs`**
- Loads file system metadata (superblock, FAT, directory) from the disk into memory.

#### 3. **`unmount_fs`**
- Writes the file system metadata (superblock, FAT, directory) back to disk.
- Frees allocated memory and closes the virtual disk.

#### 4. **`fs_open`**
- Opens a file by its name:
  - Searches the directory for the file.
  - Assigns a file descriptor and increases the reference count.
- Returns `-1` if the file doesn’t exist or no descriptors are available.

#### 5. **`fs_close`**
- Closes a file descriptor and decreases the file's reference count.
- Marks the descriptor as available for reuse.

#### 6. **`fs_create`**
- Creates a new file with a given name:
  - Allocates a directory entry and initializes file metadata.
- Returns `-1` if the file already exists or the directory is full.

#### 7. **`fs_delete`**
- Deletes a file by its name:
  - Frees the FAT entries used by the file and clears its directory entry.
- Returns `-1` if the file is open or doesn’t exist.

#### 8. **`fs_read`**
- Reads up to `nbyte` bytes starting from the current offset:
  - Copies data into the buffer and updates the offset.
  - Handles reading across multiple blocks if necessary.
- Returns the number of bytes read or `-1` on failure.

#### 9. **`fs_write`**
- Writes up to `nbyte` bytes starting from the current offset:
  - Allocates new blocks if needed to extend the file.
  - Updates the offset and handles writing across multiple blocks.
- Returns the number of bytes written or `-1` on failure.

#### 10. **`fs_get_filesize`**
- Returns the size of the file associated with a file descriptor.
- Returns `-1` if the descriptor is invalid or unused.

#### 11. **`fs_listfiles`**
- Lists all active files in the directory by name:
  - Allocates memory for the file list, which the caller must free.
- Returns `-1` if memory allocation fails.

#### 12. **`fs_lseek`**
- Adjusts the file descriptor offset to a specified position:
  - Ensures the offset does not exceed the file size.
- Returns `-1` if the offset is invalid.

#### 13. **`fs_truncate`**
- Truncates a file to a specified length:
  - Frees FAT blocks beyond the truncation point and updates file size and offset.
- Returns `-1` if the length exceeds the file size or truncation fails.
