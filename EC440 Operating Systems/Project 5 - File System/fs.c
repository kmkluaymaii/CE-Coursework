#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

#include "disk.h"

#define MAX_FILE 64
#define MAX_FILE_NAME 15
#define MAX_FILDES 32

#define FREE 0
#define XX 1

#define DISK_BLOCKS  8192      /* number of blocks on the disk                */
#define BLOCK_SIZE   4096      /* block size on "disk"                        */

#define FAT_IDX 1

int i = 0;
int j = 0;

int make_disk(char *name);     /* create an empty, virtual disk file          */
int open_disk(char *name);     /* open a virtual disk (file)                  */
int close_disk();              /* close a previously opened disk (file)       */
int block_write(int block, char *buf); /* write a block of size BLOCK_SIZE to disk    */
int block_read(int block, char *buf); /* read a block of size BLOCK_SIZE from disk   */

int fs_close(int fildes_index);

struct super_block{
    int fat_idx; // First block of the FAT
    int fat_len; // Length of FAT in blocks
    int dir_idx; // First block of directory
    int dir_len; // Length of directory in blocks
    int data_idx; // First block of file-data
};

struct dir_entry{
    int used; // Is this file-”slot” in use
    char name[MAX_FILE_NAME + 1]; // DOH!
    int size; // file size
    int head; // first data block of file
    int ref_cnt;
    // how many open file descriptors are there?
    // ref_cnt > 0 -> cannot delete file
};

struct file_descriptor{
    int used; // fd in use
    int file; // the first block of the file
              // (f) to which fd refers too
    int offset; // position of fd within f
};

struct super_block fs;
struct file_descriptor fildes[MAX_FILDES];
int *FAT = NULL; 
struct dir_entry *DIR = NULL;

int make_fs(char *disk_name){
    if(make_disk(disk_name) != 0){
        return -1;
    };
    
    if(open_disk(disk_name) != 0){
        return -1;
    };

    //Create a block to write back to the disk
    char block[BLOCK_SIZE];
    memset(block, 0, BLOCK_SIZE);

    // Initialize new superblock
    fs.fat_idx = FAT_IDX;
    fs.fat_len = (DISK_BLOCKS * sizeof(int))/BLOCK_SIZE;
    if ((DISK_BLOCKS * sizeof(int)) % BLOCK_SIZE != 0) {
        fs.fat_len++;
    }
    fs.dir_idx = FAT_IDX + fs.fat_len;
    fs.dir_len = (MAX_FILE * sizeof(struct dir_entry))/BLOCK_SIZE;
    if(((MAX_FILE * sizeof(struct dir_entry)) % BLOCK_SIZE) != 0){
        fs.dir_len++;
    }
    fs.data_idx = fs.dir_idx + fs.dir_len;
    // fs.data_idx = BLOCK_SIZE;

    // Copy the superblock data to block
    memcpy(block, &fs, sizeof(struct super_block));
    // Write superblock block back to disk (block 0)
    if (block_write(0, block) != 0){
        return -1;
    }
    memset(block, 0, BLOCK_SIZE);

    // Initialize new FAT
    FAT = malloc(fs.fat_len * BLOCK_SIZE); 
    if (FAT == NULL) {
        return -1;
    }

    for (i = 0; i < DISK_BLOCKS; i++) {
        if (i < fs.data_idx) {
            FAT[i] = XX;  
        } else {
            FAT[i] = FREE;  
        }
    }

    FAT[DISK_BLOCKS - 1] = -1;

    for(i = 0; i < fs.fat_len; i++){
        // Copy the FAT data to block (block 1)
        memcpy(block, &FAT[i*(BLOCK_SIZE / sizeof(int))], BLOCK_SIZE);
        // Write directory back to disk
        if (block_write(fs.fat_idx+i, block) != 0){
            return -1;
        }
        memset(block, 0, BLOCK_SIZE);
    }

    DIR = malloc(fs.dir_len * BLOCK_SIZE); 
    if (DIR == NULL){
        return -1;
    }
    // // Initialize new directory entry
    for(i = 0; i < MAX_FILE; i++){
        DIR[i].used = 0;
        DIR[i].size = 0;
        DIR[i].head = -1;
        DIR[i].ref_cnt = 0;
        memset(DIR[i].name, 0, MAX_FILE_NAME + 1);
    }
    // Copy the directory data to block (block 2)
    memcpy(block, DIR, MAX_FILE * sizeof(struct dir_entry));
    // Write directory back to disk
    if (block_write(fs.dir_idx, block) != 0){
        return -1;
    }
    memset(block, 0, BLOCK_SIZE);

    if(close_disk() != 0){
        return -1;
    };

    return 0;
}

int mount_fs(char *disk_name){
    if(open_disk(disk_name) != 0){
        return -1;
    };

    //Create a block to write back to the disk
    char block[BLOCK_SIZE];
    memset(block, 0, BLOCK_SIZE);

    //Load the meta-information that is necessary to handle the file system operations
    
    //Read the block 0 of the disk (superblock)
    if (block_read(0, block) != 0){
        // close_disk();
        return -1;
    }
    memcpy(&fs, block, sizeof(struct super_block));
    memset(block, 0, BLOCK_SIZE);

    FAT = malloc(fs.fat_len * BLOCK_SIZE);
    if (FAT == NULL) {
        // close_disk();
        return -1;
    }

    for(i = 0; i < fs.fat_len; i++){
         //Read the block 1 of the disk (FAT block)
        if (block_read(fs.fat_idx+i, block) != 0){
            // free(FAT);
            // close_disk();
            return -1;
        }
        memcpy(&FAT[i*(BLOCK_SIZE / sizeof(int))], block, BLOCK_SIZE);
        memset(block, 0, BLOCK_SIZE);
    }

    // DIR = (struct dir_entry *)malloc(MAX_FILE * sizeof(struct dir_entry));
    DIR = malloc(fs.dir_len * BLOCK_SIZE);
    if (DIR == NULL){
        // free(FAT);
        // close_disk();
        return -1;
    }
    // //Read the block 2 of the disk (directory block)
    if (block_read(fs.dir_idx, block) != 0){
        // free(FAT);
        // free(DIR);
        // close_disk();
        return -1;
    }
    memcpy(DIR, block, MAX_FILE * sizeof(struct dir_entry));
    memset(block, 0, BLOCK_SIZE);

    for(i = 0; i < MAX_FILDES; i++){
        fildes[i].used = 0; 
        fildes[i].file = -1; 
        fildes[i].offset = 0;
    }

    return 0;
}

int umount_fs(char *disk_name){
    for(i = 0; i < MAX_FILDES; i++){
        fs_close(i);
    }

    //Create a block to write back to the disk
    char block[BLOCK_SIZE];
    memset(block, 0, BLOCK_SIZE);

    //Write back the meta-information to disk
    
    // Copy the superblock data to block
    memcpy(block, &fs, sizeof(struct super_block));
    // Write superblock block back to disk (block 0)
    if (block_write(0, block) != 0){
        return -1;
    }
    memset(block, 0, BLOCK_SIZE);

    for(i = 0; i < fs.fat_len; i++){
        // Copy the FAT data to block (block 1)
        memcpy(block, &FAT[i*(BLOCK_SIZE / sizeof(int))], BLOCK_SIZE);
        // Write directory back to disk
        if (block_write(fs.fat_idx+i, block) != 0){
            return -1;
        }
        memset(block, 0, BLOCK_SIZE);
    }

    // Copy the directory data to block 
    memcpy(block, DIR, MAX_FILE * sizeof(struct dir_entry));
    // Write directory back to disk
    if (block_write(fs.dir_idx, block) != 0){
        return -1;
    }
    memset(block, 0, BLOCK_SIZE);

    free(FAT);
    free(DIR);

    if(close_disk() != 0){
        return -1;
    };

    return 0;
}

int fs_open(char *name){
    if(strlen(name) > MAX_FILE_NAME) {
        return -1; 
    }

    int fildes_index = -1;
    for(j = 0; j < MAX_FILDES; j++){
        if(fildes[j].used == 0){
            fildes_index = j; 
            break;
        }
    }

    if (fildes_index == -1) {
        return -1; 
    }

    int dir_index = -1;
    for (i = 0; i < MAX_FILE; i++) {
        if (DIR[i].used == 1 && strncmp(DIR[i].name, name, MAX_FILE_NAME + 1) == 0) {
            dir_index = i;
            break;
        }
    }

    if (dir_index == -1) {
        return -1; 
    }

    if (DIR[dir_index].used == 0) {
        return -1;
    }

    fildes[fildes_index].used = 1;
    fildes[fildes_index].file = dir_index; 
    fildes[fildes_index].offset = 0;

    DIR[dir_index].ref_cnt++;

    return fildes_index;
}

int fs_close(int fildes_index){
    if(fildes_index < 0 || fildes_index >= MAX_FILDES || fildes[fildes_index].used == 0){
        return -1;
    }

    int file_index = fildes[fildes_index].file;
    DIR[file_index].ref_cnt--; 
    fildes[fildes_index].used = 0;  

    return 0;
}

int fs_create(char *name){
    if(strlen(name) > MAX_FILE_NAME || strlen(name) <= 0){
        return -1;
    }
    int create_fs = 0;
    int empty_dir = -1;
    for (i = 0; i < MAX_FILE; i++) {
        if (DIR[i].used == 1 && strncmp(DIR[i].name, name, MAX_FILE_NAME + 1) == 0) {
            return -1;  
        }

        if(DIR[i].used == 0){
            DIR[i].used = 1;
            DIR[i].size = 0;
            DIR[i].head = -1;
            DIR[i].ref_cnt = 0;
            strncpy(DIR[i].name, name, MAX_FILE_NAME);
            create_fs++;
            empty_dir = i;
            break;
        }
    }

    if(empty_dir == -1){
        printf("No empty file directory to create");
        return -1;
    }

    if(create_fs == 0){
        return -1;
    }

    return 0;
}

int fs_delete(char *name){
    if (strlen(name) > MAX_FILE_NAME) {
        return -1; 
    }

    int dir_index = -1;

    for (i = 0; i < MAX_FILE; i++) {
        if(DIR[i].used == 1 && (strncmp(DIR[i].name, name, MAX_FILE_NAME + 1) == 0)){
            dir_index = i;
            break;
        }
    }

    if(dir_index == -1) {
        return -1; 
    }
    if(DIR[dir_index].ref_cnt > 0){
        return -1;
    }

    int block = DIR[dir_index].head;

    while (block != -1) {
        int next_block = FAT[block]; 
        FAT[block] = FREE;         
        block = next_block;         
    }

    DIR[dir_index].used = 0;
    DIR[dir_index].size = 0;
    DIR[dir_index].head = -1;
    DIR[dir_index].ref_cnt = 0;
    memset(DIR[dir_index].name, 0, MAX_FILE_NAME + 1); 

    return 0;
}

int fs_read(int fildes_index, void *buf, size_t nbyte){
    if(fildes_index < 0 || fildes_index >= MAX_FILDES || fildes[fildes_index].used == 0){
        return -1;
    }

    struct file_descriptor *fd = &fildes[fildes_index];
    int dir_index = fd->file; 

    if (dir_index < 0 || dir_index >= MAX_FILE || DIR[dir_index].used == 0) {
        return -1; 
    }

    struct dir_entry *file = &DIR[dir_index]; 

    if (fd->offset >= file->size) {
        return 0; 
    }

    if (fd->offset + nbyte > file->size) {
        nbyte = file->size - fd->offset; 
    }

    size_t bytes_read = 0;
    char read_buf[BLOCK_SIZE];
    int current_block = file->head;

    int block_number = fd->offset / BLOCK_SIZE; 
    int block_offset = fd->offset % BLOCK_SIZE; 

    for (i = 0; i < block_number; i++) {
        if (current_block == -1 || current_block < 0 || current_block >= DISK_BLOCKS) {
            return -1; 
        }
        current_block = FAT[current_block];
    }

    while (bytes_read < nbyte && current_block != -1) {
        if (block_read(current_block, read_buf) != 0) {
            return -1; 
        }

        int read_len = BLOCK_SIZE - block_offset;
        if (bytes_read + read_len > nbyte) {
            read_len = nbyte - bytes_read;
        }

        memcpy((char *)buf + bytes_read, read_buf + block_offset, read_len);

        bytes_read += read_len;
        block_offset = 0;
        current_block = FAT[current_block];

    }

    fd->offset += bytes_read;
    return bytes_read;
}

int fs_write(int fildes_index, void *buf, size_t nbyte) {
    if(fildes_index < 0 || fildes_index >= MAX_FILDES || fildes[fildes_index].used == 0){
        return -1;
    }

    struct file_descriptor *fd = &fildes[fildes_index];
    int dir_index = fd->file;

    if (dir_index < 0 || dir_index >= MAX_FILE || DIR[dir_index].used == 0) {
        return -1;
    }

    struct dir_entry *file = &DIR[dir_index];

    size_t new_size = fd->offset + nbyte;
    if(new_size > file->size){
        file->size = new_size;
    }

    size_t bytes_written = 0;
    char write_buf[BLOCK_SIZE];
    int current_block = file->head;
    int previous_block = -1;

    if(current_block == -1){
        int free_block = -1;
        for(i = fs.data_idx; i < DISK_BLOCKS; i++){
            if(FAT[i] == FREE){
                FAT[i] = -1;   
                free_block = i;
                break;
            }
        }
        if(free_block == -1){
            return -1; 
        }

        FAT[free_block] = -1;
        file->head = free_block;
        current_block = free_block;
    }

    int block_number = fd->offset / BLOCK_SIZE;
    int block_offset = fd->offset % BLOCK_SIZE;

    for (i = 0; i < block_number; i++) {
        previous_block = current_block;
        current_block = FAT[current_block];
        if (current_block == -1) {
            int free_block = -1;
            for(i = fs.data_idx; i < DISK_BLOCKS; i++){
                if(FAT[i] == FREE){
                    FAT[i] = -1;   
                    free_block = i;
                    break;
                }
            }
            if (free_block == -1) {
                return -1;
            }
            FAT[previous_block] = free_block;
            FAT[free_block] = -1;
            current_block = free_block;
        }
    }


    while(bytes_written < nbyte) {
        if (block_read(current_block, write_buf) != 0){
            return -1;
        }

        int write_offset =  (bytes_written == 0) ? block_offset : 0;
        int write_len = BLOCK_SIZE - write_offset;

        if (write_len > (nbyte - bytes_written)){
            write_len = nbyte - bytes_written;
        }

        memcpy(write_buf + write_offset, (char *)buf + bytes_written, write_len);

        if (block_write(current_block, write_buf) != 0){
            return -1;
        }

        bytes_written += write_len;

        if (bytes_written < nbyte){
            int new_block = -1;
            previous_block = current_block;
            current_block = FAT[current_block];
            if (current_block == -1) {
                for(i = fs.data_idx; i < DISK_BLOCKS; i++){
                    if(FAT[i] == FREE){
                        FAT[i] = -1;   
                        new_block = i;
                        break;
                    }
                }
                if(new_block == -1){
                    return -1; 
                }
                FAT[previous_block] = new_block;
                FAT[new_block] = -1;
                current_block = new_block;
            }
        }
    }

    fd->offset += bytes_written;
    return bytes_written;
}


int fs_get_filesize(int fildes_index){
    if(fildes_index < 0 || fildes_index >= MAX_FILDES || fildes[fildes_index].used == 0){
        return -1;
    }
    int file_index = fildes[fildes_index].file;
    if(file_index < 0 || file_index >= MAX_FILE || DIR[file_index].used == 0){
        return -1;
    }

    return DIR[file_index].size;
}

int fs_listfiles(char ***files){
    char **file_list = calloc(MAX_FILE + 1,sizeof(char *));
    if (file_list == NULL) {
        return -1;
    }
    int count = 0;
    for (i = 0; i < MAX_FILE; ++i){
		if (DIR[i].used == 1){
			file_list[count] = malloc(MAX_FILE_NAME);
            if (file_list[count] == NULL){
                for(j = 0; j < count; j++) {
                    free(file_list[j]);
                }
                free(file_list);
                return -1;
            }
			strncpy(file_list[count], DIR[i].name, MAX_FILE_NAME);
			file_list[count][MAX_FILE_NAME] = '\0';
			++count;
		}
	}

	file_list[count] = NULL;
	
	*files = file_list;

	return 0;
}

int fs_lseek(int fildes_index, off_t offset){
    if(fildes_index < 0 || fildes_index >= MAX_FILDES || fildes[fildes_index].used == 0 || offset < 0 || offset > DIR[fildes[fildes_index].file].size){
        return -1;
    }

    fildes[fildes_index].offset = offset;
    return 0;
}

int fs_truncate(int fildes_index, off_t length){
    if(fildes_index < 0 || fildes_index >= MAX_FILDES || fildes[fildes_index].used == 0 || length < 0){
        return -1;
    }

    struct file_descriptor *fd = &fildes[fildes_index];
    int dir_index = fd->file;
    if(dir_index < 0 || dir_index >= MAX_FILE || DIR[dir_index].used == 0){
        return -1;
    }

    struct dir_entry *file = &DIR[dir_index];

    if(length > file->size){
        return -1;
    }

    if(length == file->size){
        return 0;
    }

    if(length == 0){
        int current_block = file->head;
        while(current_block != -1){
            int next_block = FAT[current_block];
            FAT[current_block] = FREE;
            current_block = next_block;
        }
        file->head = -1;
        file->size = 0;
        fd->offset = 0;
        return 0;
    }

    int current_block = file->head;

    int last_block_number = (length - 1) / BLOCK_SIZE;
    int last_block_offset = (length - 1) % BLOCK_SIZE;

    for (i = 0; i < last_block_number; i++) {
        if (current_block == -1) {
            return -1; 
        }
        current_block = FAT[current_block];
    }

    if(current_block == -1){
        return -1; 
    }

    int free_block = FAT[current_block];
    FAT[current_block] = -1;

    while(free_block != -1){
        int next_block = FAT[free_block];
        FAT[free_block] = FREE;
        free_block = next_block;
    }
    if(last_block_offset + 1 < BLOCK_SIZE){
        char truncate_buf[BLOCK_SIZE];
        if(block_read(current_block, truncate_buf) != 0){
            return -1;
        }
        memset(truncate_buf + last_block_offset + 1, 0, BLOCK_SIZE - (last_block_offset + 1));
        if(block_write(current_block, truncate_buf) != 0){
            return -1;
        }
    }

    file->size = length;
    fd->offset = length;

    return 0;
}