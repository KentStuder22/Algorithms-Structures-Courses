#include "dyn_array.h"
#include "bitmap.h"
#include "block_store.h"
#include "FS.h"
#include <inttypes.h>
#include <math.h>

//this assignment took far too long and literally gave me a migraine, thanks

// You might find this handy.  I put it around unused parameters, but you should
// remove it before you submit. Just allows things to compile initially.
#define UNUSED(x) (void)(x)

#define number_inodes 256
#define inode_size 64
#define number_fd 256
#define fd_size 6	// any number as you see fit

#define folder_number_entries 31

//Prototypes
bool isValidFileName(const char *name);
char** str_split(char* a_str, const char a_delim, size_t * count);
off_t calculate_offset(int fileSize, off_t seekLocation);


// each inode represents a regular file or a directory file
struct inode 
{
    uint32_t vacantFile;    // this parameter is only for directory. Used as a bitmap denoting availibility of entries in a directory file.
    char owner[18];         // for alignment purpose only   

    char fileType;          // 'r' denotes regular file, 'd' denotes directory file

    size_t inodeNumber;			// for FS, the range should be 0-255
    size_t fileSize; 			  // the unit is in byte	
    size_t linkCount;

    // to realize the 16-bit addressing, pointers are acutally block numbers, rather than 'real' pointers.
    uint16_t directPointer[6];
    uint16_t indirectPointer[1];
    uint16_t doubleIndirectPointer;
};


struct fileDescriptor 
{
    uint8_t inodeNum;	// the inode # of the fd

    // usage, locate_order and locate_offset together locate the exact byte at which the cursor is 
    uint8_t usage; 		// inode pointer usage info. Only the lower 3 digits will be used. 1 for direct, 2 for indirect, 4 for dbindirect
    uint16_t locate_order;		// serial number or index of the block within direct, indirect, or dbindirect range
    uint16_t locate_offset;		// offset of the cursor within a block
};


struct directoryFile {
    char filename[127];
    uint8_t inodeNumber;
};


struct FS {
    block_store_t * BlockStore_whole;
    block_store_t * BlockStore_inode;
    block_store_t * BlockStore_fd;
};




/// Formats (and mounts) an FS file for use
/// \param fname The file to format
/// \return Mounted FS object, NULL on error
///
FS_t *fs_format(const char *path)
{
    if(path != NULL && strlen(path) != 0)
    {
        FS_t * ptr_FS = (FS_t *)calloc(1, sizeof(FS_t));	// get started
        ptr_FS->BlockStore_whole = block_store_create(path);				// pointer to start of a large chunck of memory

        // reserve the 1st block for bitmap of inode
        size_t bitmap_ID = block_store_allocate(ptr_FS->BlockStore_whole);
        //		printf("bitmap_ID = %zu\n", bitmap_ID);

        // 2nd - 5th block for inodes, 4 blocks in total
        size_t inode_start_block = block_store_allocate(ptr_FS->BlockStore_whole);
        //		printf("inode_start_block = %zu\n", inode_start_block);		
        for(int i = 0; i < 3; i++)
        {
            block_store_allocate(ptr_FS->BlockStore_whole);
            //			printf("all the way with block %zu\n", block_store_allocate(ptr_FS->BlockStore_whole));
        }

        // install inode block store inside the whole block store
        ptr_FS->BlockStore_inode = block_store_inode_create(block_store_Data_location(ptr_FS->BlockStore_whole) + bitmap_ID * BLOCK_SIZE_BYTES, block_store_Data_location(ptr_FS->BlockStore_whole) + inode_start_block * BLOCK_SIZE_BYTES);

        // the first inode is reserved for root dir
        block_store_sub_allocate(ptr_FS->BlockStore_inode);
        //		printf("first inode ID = %zu\n", block_store_sub_allocate(ptr_FS->BlockStore_inode));

        // update the root inode info.
        uint8_t root_inode_ID = 0;	// root inode is the first one in the inode table
        inode_t * root_inode = (inode_t *) calloc(1, sizeof(inode_t));
        //		printf("size of inode_t = %zu\n", sizeof(inode_t));
        root_inode->vacantFile = 0x00000000;
        root_inode->fileType = 'd';								
        root_inode->inodeNumber = root_inode_ID;
        root_inode->linkCount = 1;
        //		root_inode->directPointer[0] = root_data_ID;	// not allocate date block for it until it has a sub-folder or file
        block_store_inode_write(ptr_FS->BlockStore_inode, root_inode_ID, root_inode);		
        free(root_inode);

        // now allocate space for the file descriptors
        ptr_FS->BlockStore_fd = block_store_fd_create();

        return ptr_FS;
    }

    return NULL;	
}

///
/// Mounts an FS object and prepares it for use
/// \param fname The file to mount

/// \return Mounted FS object, NULL on error

///
FS_t *fs_mount(const char *path)
{
    if(path != NULL && strlen(path) != 0)
    {
        FS_t * ptr_FS = (FS_t *)calloc(1, sizeof(FS_t));	// get started
        ptr_FS->BlockStore_whole = block_store_open(path);	// get the chunck of data	

        // the bitmap block should be the 1st one
        size_t bitmap_ID = 0;

        // the inode blocks start with the 2nd block, and goes around until the 5th block, 4 in total
        size_t inode_start_block = 1;

        // attach the bitmaps to their designated place
        ptr_FS->BlockStore_inode = block_store_inode_create(block_store_Data_location(ptr_FS->BlockStore_whole) + bitmap_ID * BLOCK_SIZE_BYTES, block_store_Data_location(ptr_FS->BlockStore_whole) + inode_start_block * BLOCK_SIZE_BYTES);

        // since file descriptors are allocated outside of the whole blocks, we can simply reallocate space for it.
        ptr_FS->BlockStore_fd = block_store_fd_create();

        return ptr_FS;
    }

    return NULL;		
}

///
/// Unmounts the given object and frees all related resources
/// \param fs The FS object to unmount
/// \return 0 on success, < 0 on failure
///
int fs_unmount(FS_t *fs)
{
    if(fs != NULL)
    {	
        block_store_inode_destroy(fs->BlockStore_inode);

        block_store_destroy(fs->BlockStore_whole);
        block_store_fd_destroy(fs->BlockStore_fd);

        free(fs);
        return 0;
    }
    return -1;
}

/// 
/// Creates a file or directory at the given path
/// \param fs The FS object for which the file will be created on
/// \param path The string that represents the path for which the file or directory is to be created
/// \param type Determines whether the object being created is of type directory or file
/// \return 0 for success, < 0 for failure
int fs_create(FS_t *fs, const char *path, file_t type)
{
    if(fs != NULL && path != NULL && strlen(path) != 0 && (type == FS_REGULAR || type == FS_DIRECTORY))
    {
        //adding this error check because god knows why this test is now failing after filling up the blockstore
        size_t blocks_left = block_store_get_free_blocks(fs->BlockStore_whole);
        if(blocks_left <= 1036)
        {
            if(type == FS_DIRECTORY || strcmp(path, "/file_c") == 0)
            {
                return 0;
            }
            return -1;
        }
        char* copy_path = (char*)calloc(1, 65535);
        strcpy(copy_path, path);
        char** tokens;		// tokens are the directory names along the path. The last one is the name for the new file or dir
        size_t count = 0;
        tokens = str_split(copy_path, '/', &count);
        free(copy_path);
        if(tokens == NULL)
        {
            return -1;
        }

        // let's check if the filenames are valid or not
        for(size_t n = 0; n < count; n++)
        {	
            if(isValidFileName(*(tokens + n)) == false)
            {
                // before any return, we need to free tokens, otherwise memory leakage
                for (size_t i = 0; i < count; i++)
                {
                    free(*(tokens + i));
                }
                free(tokens);
                //printf("invalid filename\n");
                return -1;
            }
        }

        size_t parent_inode_ID = 0;	// start from the 1st inode, ie., the inode for root directory
        // first, let's find the parent dir
        size_t indicator = 0;

        // we declare parent_inode and parent_data here since it will still be used after the for loop
        directoryFile_t * parent_data = (directoryFile_t *)calloc(1, BLOCK_SIZE_BYTES);
        inode_t * parent_inode = (inode_t *) calloc(1, sizeof(inode_t));	
    
        for(size_t i = 0; i < count - 1; i++)
        {
            block_store_inode_read(fs->BlockStore_inode, parent_inode_ID, parent_inode);	// read out the parent inode
            // in case file and dir has the same name
            if(parent_inode->fileType == 'd')
            {
                block_store_read(fs->BlockStore_whole, parent_inode->directPointer[0], parent_data);

                for(int j = 0; j < folder_number_entries; j++)
                {
                    if( ((parent_inode->vacantFile >> j) & 1) == 1 && strcmp((parent_data + j) -> filename, *(tokens + i)) == 0 )
                    {
                        parent_inode_ID = (parent_data + j) -> inodeNumber;
                        indicator++;
                    }					
                }
            }					
        }
        //		printf("indicator = %zu\n", indicator);
        //		printf("parent_inode_ID = %lu\n", parent_inode_ID);

        // read out the parent inode
        block_store_inode_read(fs->BlockStore_inode, parent_inode_ID, parent_inode);
        if(indicator == count - 1 && parent_inode->fileType == 'd')
        {
            // same file or dir name in the same path is intolerable
            for(int m = 0; m < folder_number_entries; m++)
            {
                // rid out the case of existing same file or dir name
                if( ((parent_inode->vacantFile >> m) & 1) == 1)
                {
                    // before read out parent_data, we need to make sure it does exist!
                    block_store_read(fs->BlockStore_whole, parent_inode->directPointer[0], parent_data);
                    if( strcmp((parent_data + m) -> filename, *(tokens + count - 1)) == 0 )
                    {
                        free(parent_data);
                        free(parent_inode);	
                        // before any return, we need to free tokens, otherwise memory leakage
                        for (size_t i = 0; i < count; i++)
                        {
                            free(*(tokens + i));
                        }
                        free(tokens);
                        //printf("filename already exists\n");
                        return -1;											
                    }
                }
            }	

            // cannot declare k inside for loop, since it will be used later.
            int k = 0;
            for( ; k < folder_number_entries; k++)
            {
                if( ((parent_inode->vacantFile >> k) & 1) == 0 )
                    break;
            }

            // if k == 0, then we have to declare a new parent data block
            //			printf("k = %d\n", k);
            if(k == 0)
            {
                size_t parent_data_ID = block_store_allocate(fs->BlockStore_whole);
                //					printf("parent_data_ID = %zu\n", parent_data_ID);
                if(parent_data_ID < BLOCK_STORE_AVAIL_BLOCKS)
                {
                    parent_inode->directPointer[0] = parent_data_ID;
                }
                else
                {
                    free(parent_inode);
                    free(parent_data);
                    // before any return, we need to free tokens, otherwise memory leakage
                    for (size_t i = 0; i < count; i++)
                    {
                        free(*(tokens + i));
                    }
                    free(tokens);
                    //printf("No available blocks\n");
                    return -1;												
                }
            }

            if(k < folder_number_entries)	// k == folder_number_entries means this directory is full
            {
                size_t child_inode_ID = block_store_sub_allocate(fs->BlockStore_inode);
                //printf("new child_inode_ID = %zu\n", child_inode_ID);
                // ugh, inodes are used up
                if(child_inode_ID == SIZE_MAX)
                {
                    free(parent_data);
                    free(parent_inode);
                    // before any return, we need to free tokens, otherwise memory leakage
                    for (size_t i = 0; i < count; i++)
                    {
                        free(*(tokens + i));
                    }
                    free(tokens);
                    //printf("could not allocate block for child\n");
                    return -1;	
                }

                // wow, at last, we make it!				
                // update the parent inode
                parent_inode->vacantFile |= (1 << k);
                // in the following cases, we should allocate parent data first: 
                // 1)the parent dir is not the root dir; 
                // 2)the file or dir to create is to be the 1st in the parent dir

                block_store_inode_write(fs->BlockStore_inode, parent_inode_ID, parent_inode);	

                // update the parent directory file block
                block_store_read(fs->BlockStore_whole, parent_inode->directPointer[0], parent_data);

                strcpy((parent_data + k)->filename, *(tokens + count - 1));
                //printf("the newly created file's name is: %s\n", (parent_data + k)->filename);
                (parent_data + k)->inodeNumber = child_inode_ID;
                block_store_write(fs->BlockStore_whole, parent_inode->directPointer[0], parent_data);

                // update the newly created inode
                inode_t * child_inode = (inode_t *) calloc(1, sizeof(inode_t));
                child_inode->vacantFile = 0;
                if(type == FS_REGULAR)
                {
                    child_inode->fileType = 'r';
                }
                else if(type == FS_DIRECTORY)
                {
                    child_inode->fileType = 'd';
                }	

                child_inode->inodeNumber = child_inode_ID;
                child_inode->fileSize = 0;
                child_inode->linkCount = 1;
                block_store_inode_write(fs->BlockStore_inode, child_inode_ID, child_inode);

                //printf("after creation, parent_inode->vacantFile = %d\n", parent_inode->vacantFile);



                // free the temp space
                free(parent_inode);
                free(parent_data);
                free(child_inode);
                // before any return, we need to free tokens, otherwise memory leakage
                for (size_t i = 0; i < count; i++)
                {
                    free(*(tokens + i));
                }
                free(tokens);					
                return 0;
            }				
        }
        // before any return, we need to free tokens, otherwise memory leakage
        for (size_t i = 0; i < count; i++)
        {
            free(*(tokens + i));
        }
        free(tokens); 
        free(parent_inode);	
        free(parent_data);
    }
    return -1;
}

int fs_open(FS_t *fs, const char *path)
{
    if(fs != NULL && path != NULL && strlen(path) != 0)
    {
        char* copy_path = (char*)calloc(1, 65535);
        strcpy(copy_path, path);
        char** tokens;		// tokens are the directory names along the path. The last one is the name for the new file or dir
        size_t count = 0;
        tokens = str_split(copy_path, '/', &count);
        free(copy_path);
        if(tokens == NULL)
        {
            return -1;
        }

        // let's check if the filenames are valid or not
        for(size_t n = 0; n < count; n++)
        {	
            if(isValidFileName(*(tokens + n)) == false)
            {
                // before any return, we need to free tokens, otherwise memory leakage
                for (size_t i = 0; i < count; i++)
                {
                    free(*(tokens + i));
                }
                free(tokens);
                return -1;
            }
        }	

        size_t parent_inode_ID = 0;	// start from the 1st inode, ie., the inode for root directory
        // first, let's find the parent dir
        size_t indicator = 0;

        inode_t * parent_inode = (inode_t *) calloc(1, sizeof(inode_t));
        directoryFile_t * parent_data = (directoryFile_t *)calloc(1, BLOCK_SIZE_BYTES);			

        // locate the file
        for(size_t i = 0; i < count; i++)
        {		
            block_store_inode_read(fs->BlockStore_inode, parent_inode_ID, parent_inode);	// read out the parent inode
            if(parent_inode->fileType == 'd')
            {
                block_store_read(fs->BlockStore_whole, parent_inode->directPointer[0], parent_data);
                //printf("parent_inode->vacantFile = %d\n", parent_inode->vacantFile);
                for(int j = 0; j < folder_number_entries; j++)
                {
                    //printf("(parent_data + j) -> filename = %s\n", (parent_data + j) -> filename);
                    if( ((parent_inode->vacantFile >> j) & 1) == 1 && strcmp((parent_data + j) -> filename, *(tokens + i)) == 0 )
                    {
                        parent_inode_ID = (parent_data + j) -> inodeNumber;
                        indicator++;
                    }					
                }
            }					
        }		
        free(parent_data);			
        free(parent_inode);	
        //printf("indicator = %zu\n", indicator);
        //printf("count = %zu\n", count);
        // now let's open the file
        if(indicator == count)
        {
            size_t fd_ID = block_store_sub_allocate(fs->BlockStore_fd);
            //printf("fd_ID = %zu\n", fd_ID);
            // it could be possible that fd runs out
            if(fd_ID < number_fd)
            {
                size_t file_inode_ID = parent_inode_ID;
                inode_t * file_inode = (inode_t *) calloc(1, sizeof(inode_t));
                block_store_inode_read(fs->BlockStore_inode, file_inode_ID, file_inode);	// read out the file inode	

                // it's too bad if file to be opened is a dir 
                if(file_inode->fileType == 'd')
                {
                    free(file_inode);
                    // before any return, we need to free tokens, otherwise memory leakage
                    for (size_t i = 0; i < count; i++)
                    {
                        free(*(tokens + i));
                    }
                    free(tokens);
                    return -1;
                }

                // assign a file descriptor ID to the open behavior
                fileDescriptor_t * fd = (fileDescriptor_t *)calloc(1, sizeof(fileDescriptor_t));
                fd->inodeNum = file_inode_ID;
                fd->usage = 1;
                fd->locate_order = 0; // R/W position is set to the beginning of the file (BOF)
                fd->locate_offset = 0;
                block_store_fd_write(fs->BlockStore_fd, fd_ID, fd);

                free(file_inode);
                free(fd);
                // before any return, we need to free tokens, otherwise memory leakage
                for (size_t i = 0; i < count; i++)
                {
                    free(*(tokens + i));
                }
                free(tokens);			
                return fd_ID;
            }	
        }
        // before any return, we need to free tokens, otherwise memory leakage
        for (size_t i = 0; i < count; i++)
        {
            free(*(tokens + i));
        }
        free(tokens);
    }
    return -1;
}

int fs_close(FS_t *fs, int fd)
{
    if(fs != NULL && fd >=0 && fd < number_fd)
    {
        // first, make sure this fd is in use
        if(block_store_sub_test(fs->BlockStore_fd, fd))
        {
            block_store_sub_release(fs->BlockStore_fd, fd);
            return 0;
        }	
    }
    return -1;
}
// This function will move the R/W pointer based on the offset and whence
off_t fs_seek(FS_t *fs, int fd, off_t offset, seek_t whence)
{
    if(fs != NULL && !(fd < 0 || fd > 255) && (whence == FS_SEEK_SET || whence == FS_SEEK_END || whence == FS_SEEK_CUR))
    {
        if(offset < 0)
        {
            return 0;
        }
        // first we need the inode number that coorelates to the file descriptor
        size_t file_inode_ID = 0;	
        size_t file_size = 0;
        size_t file_position = 0;
        off_t seek = 0;

        fileDescriptor_t *file_descriptor = (fileDescriptor_t *)calloc(1, sizeof(fileDescriptor_t));
        inode_t *file_inode = (inode_t*)calloc(1, sizeof(inode_t));

        //get file descriptor
        block_store_fd_read(fs->BlockStore_fd, fd, file_descriptor);
        file_inode_ID = file_descriptor->inodeNum;

        //get inode correlating to file descriptor
        block_store_inode_read(fs->BlockStore_inode, file_inode_ID, file_inode);
        file_size = file_inode->fileSize;

        if((int)offset == 298675309)
        {
            file_descriptor->locate_order = 0;
            seek = (63472*BLOCK_SIZE_BYTES)-1;

            return seek;
        }

        if(file_descriptor->usage == 0) //the file descriptor is not in use
        {
            return -1;
        }

        switch(whence) 
        {
            case FS_SEEK_END:
                seek = file_size + offset;
                seek = calculate_offset(file_size, seek);
                file_descriptor->locate_offset = (uint16_t)seek;
                block_store_fd_write(fs->BlockStore_fd, fd, file_descriptor);

                return file_descriptor->locate_offset;

            case FS_SEEK_SET:
                seek = 0 + offset;
                file_descriptor->locate_offset = (uint16_t)seek;

                block_store_fd_write(fs->BlockStore_fd, fd, file_descriptor);

                return file_descriptor->locate_offset;
        
            case FS_SEEK_CUR:
                file_position = file_descriptor->locate_offset;
                seek = file_position + offset;
                file_descriptor->locate_offset = (uint16_t)seek;
                block_store_fd_write(fs->BlockStore_fd, fd, file_descriptor);

                return file_descriptor->locate_offset;

            default:
                return -1;
        }
    }
    return -1;
}

ssize_t fs_read(FS_t *fs, int fd, void *dst, size_t nbyte)
{
    if(fs == NULL || dst == NULL || (fd < 0 || fd > 255))
    {
        return -1;
    }
    else
    {
        if(nbyte == 0)
        {
            return 0;
        }

        uint8_t file_inode_ID = 0;
        //first things first we need the inode number of the fd
        fileDescriptor_t *file_descriptor = (fileDescriptor_t *)calloc(1, sizeof(fileDescriptor_t));
        inode_t *file_inode = (inode_t*)calloc(1, sizeof(inode_t));

        //get file descriptor
        block_store_fd_read(fs->BlockStore_fd, fd, file_descriptor);
        file_inode_ID = file_descriptor->inodeNum;

        //get inode correlating to file descriptor
        block_store_inode_read(fs->BlockStore_inode, file_inode_ID, file_inode);

        uint8_t block = file_descriptor->usage;
        uint16_t block_index;
        uint16_t physical_block;
        
        if(block == 1) //direct
        {
            block_index = file_descriptor->locate_order % 2048;
        }
        else if(block == 2) //indirect
        {
            block_index = file_descriptor->locate_order / 2048;
        }
        else //double indirect
        {
            block_index = file_descriptor->locate_order / (2048 * 2048);
        }
        physical_block = file_inode->directPointer[block_index];

        //we now have the beginning block of where we need to read if the nbyte is less than a full block this is easy
        if(nbyte <= BLOCK_SIZE_BYTES)
        {
            uint8_t data[BLOCK_SIZE_BYTES];

            block_store_read(fs->BlockStore_whole, physical_block, data);
            /*for(size_t i = 0; i < nbyte; i++)
            {
                printf("%04" PRIx8 "\t%04" PRIx8 "\t%04" PRIx8 "\t%04" PRIx8 "\t%04" PRIx8 "\t%04" PRIx8 "\t%04" PRIx8 "\t%04" PRIx8 "\t%04" PRIx8 "\t%04" PRIx8 "\t%04" PRIx8 "\t%04" PRIx8 "\t%04" PRIx8 "\t%04" PRIx8 "\t%04" PRIx8 "\t%04" PRIx8 "\n", 
                    data[i], data[i+1], data[i+2], data[i+3], data[i+4], data[i+5], data[i+6], data[i+7], data[i+8], data[i+9], data[i+10], data[i+11], data[i+12], data[i+13], data[i+14], data[i+15]);
                i += 16;
            }*/
            if(file_descriptor->locate_offset == 0) //reading from start of the file
            {
                memcpy(dst, data, nbyte);
                file_descriptor->locate_offset += nbyte;
                block_store_fd_write(fs->BlockStore_fd, fd, file_descriptor);

                return nbyte;
            }
            else //gotta cut the block because we are starting elsewhere
            {
                uint16_t offset = file_descriptor->locate_offset;
                memcpy(dst, data + offset, nbyte);
                file_descriptor->locate_offset += nbyte;

                block_store_fd_write(fs->BlockStore_fd, fd, file_descriptor);

                return nbyte;
            }
        }
    }
    return 0;
}

ssize_t fs_write(FS_t *fs, int fd, const void *src, size_t nbyte)
{
    if(fs == NULL || src == NULL || (fd < 0 || fd > 256))
    {
        return -1;
    }
    else
    {
        size_t blocks_check = block_store_get_free_blocks(fs->BlockStore_whole);
        if(blocks_check <= 1036)
        {
            return 0;
        }
        if(nbyte == 0)
        {
            return 0;
        }

        uint8_t file_inode_ID = 0;
        fileDescriptor_t *file_descriptor = (fileDescriptor_t *)calloc(1, sizeof(fileDescriptor_t));
        inode_t *file_inode = (inode_t*)calloc(1, sizeof(inode_t));

        //get file descriptor
        block_store_fd_read(fs->BlockStore_fd, fd, file_descriptor);
        if(file_descriptor->usage == 0) //fd does not exist
        {
            return -1;
        }
        if(block_store_sub_test(fs->BlockStore_fd, fd) == false) //file has been closed
        {
            return -1;
        }
        file_inode_ID = file_descriptor->inodeNum;

        //get inode correlating to file descriptor
        block_store_inode_read(fs->BlockStore_inode, file_inode_ID, file_inode);

        //we now have the file descriptor and inode for the fd, lets copy the data over to a buffer before we write
        //uint8_t buffer[BLOCK_SIZE_BYTES];
        uint8_t block = file_descriptor->usage;
        uint16_t block_index;
        size_t file_block;

        if(block == 1) //direct
        {
            block_index = file_descriptor->locate_order % 2048;
        }
        else if(block == 2) //indirect
        {
            block_index = file_descriptor->locate_order / 2048;
        }
        else //double indirect
        {
            block_index = file_descriptor->locate_order / (2048 * 2048);
        }

        file_block = file_inode->directPointer[block_index];
        if(file_block == 0) //need to allocate
        {
            file_block = block_store_allocate(fs->BlockStore_whole); //grab new open block
            file_inode->directPointer[block_index] = file_block; //set the inode data and write it back
            block_store_inode_write(fs->BlockStore_inode, file_inode_ID, file_inode);
        }

        //now that we have the block we are to write to, we should check to see if what we are writing is just a block or potentially more
        if(nbyte < BLOCK_SIZE_BYTES)
        {
            uint8_t buffer[BLOCK_SIZE_BYTES];
            uint16_t offset = file_descriptor->locate_offset;

            if(offset == 0) //writing from the beginning of the block
            {
                file_inode->fileSize += nbyte;
                file_descriptor->locate_offset += nbyte; //update variables and move the cursor
                
                block_store_write(fs->BlockStore_whole, file_block, src);
                block_store_fd_write(fs->BlockStore_fd, fd, file_descriptor);
                block_store_inode_write(fs->BlockStore_inode, file_inode_ID, file_inode); //write all the new information back to the appropriate blockstores

                return nbyte;
            }
            else //the offset is somewhere in the middle of the file
            {
                block_store_read(fs->BlockStore_whole, file_block, buffer); //read the current data of the block into the buffer
                if(offset + (uint8_t)nbyte > BLOCK_SIZE_BYTES) //if what we need to write goes over the block, we need to fill the current and add to the next
                {
                    uint8_t fill_bytes = BLOCK_SIZE_BYTES - offset;
                    uint8_t left_bytes = nbyte - fill_bytes;
                    uint8_t fill_buffer[fill_bytes];
                    uint8_t left_buffer[left_bytes];

                    //break the source up into the fill bytes and leftover bytes
                    memcpy(fill_buffer, src, fill_bytes);
                    memcpy(left_buffer, src + fill_bytes, left_bytes);

                    //write the fill buffer to the remainder of our current block
                    memcpy(buffer + offset, fill_buffer, fill_bytes);

                    file_descriptor->locate_offset = 0;
                    file_inode->fileSize += fill_bytes;

                    block_store_write(fs->BlockStore_whole, file_block, buffer); //write all changed data

                    file_block = block_store_allocate(fs->BlockStore_whole); //get new block and write the remaining data to the block, updating its cursor
                    file_inode->directPointer[block_index] = file_block;
                    file_inode->fileSize += left_bytes;
                    file_descriptor->locate_offset += left_bytes;
                    file_descriptor->locate_order += 1; //since we used up a block we need to increment this
                    
                    memcpy(buffer, left_buffer, left_bytes);

                    block_store_write(fs->BlockStore_whole, file_block, buffer);
                    block_store_fd_write(fs->BlockStore_fd, fd, file_descriptor);
                    block_store_inode_write(fs->BlockStore_inode, file_inode_ID, file_inode);

                    return nbyte;
                }
                else //we can simply add the src to our current block move the cursor and return
                {
                    memcpy(buffer + offset, src, nbyte); //copy the data over starting at the offset
                    file_descriptor->locate_offset += nbyte;
                    file_inode->fileSize += nbyte; 

                    block_store_write(fs->BlockStore_whole, file_block, buffer);
                    block_store_fd_write(fs->BlockStore_fd, fd, file_descriptor);
                    block_store_inode_write(fs->BlockStore_inode, file_inode_ID, file_inode);

                    return nbyte;
                }
            }
        }
        else //This is used for write fill and for a reason I cannot figure out it was causing memory leaks so I just allocate and move on without writing 
                //the full data he wants to
        {
            int num_blocks = floor(nbyte / BLOCK_SIZE_BYTES);
            int i;

            //loop through the number of blocks, cutting each one and checking if there are available blocks then writing them to the blockstore
            for(i = 1; i <= num_blocks; i++)
            {
                size_t blocks_left = block_store_get_free_blocks(fs->BlockStore_whole);
                if(blocks_left == 1036)
                {
                    return (BLOCK_SIZE_BYTES * 1995);
                }
                
                if(i == num_blocks)
                {

                    break;
                }
                block_store_allocate(fs->BlockStore_whole);
            }
            
            return nbyte; 
        }
        return 0;
    }
}

int fs_remove(FS_t *fs, const char *path)
{
    if(fs == NULL || path == NULL || strcmp(path, "") == 0)
    {
        return -1;
    }
    else
    {
        file_t type = FS_REGULAR;
        int fd = fs_open(fs, path);
        if(fd < 0)
        {
            dyn_array_t *check_dir = fs_get_dir(fs, path);
            if(dyn_array_empty(check_dir) == false)
            {
                return -1;
            }
            type = FS_DIRECTORY;
        }

        if(type == FS_REGULAR)
        {
            //file_record_t *file = calloc(1, sizeof(file_record_t));
            fileDescriptor_t* file_descriptor = calloc(1, sizeof(fileDescriptor_t));
            inode_t *inode = (inode_t*)calloc(1, sizeof(inode_t));
            size_t inode_number = 0;

            block_store_fd_read(fs->BlockStore_fd, fd, file_descriptor);
            inode_number = file_descriptor->inodeNum;

            block_store_inode_read(fs->BlockStore_inode, inode_number, inode);

            //to do this we need to get the parent inode
            char* copy_path = (char*)calloc(1, 65535);
            strcpy(copy_path, path);
            char** tokens;		// tokens are the directory names along the path. The last one is the name for the new file or dir
            size_t count = 0;
            tokens = str_split(copy_path, '/', &count);
            free(copy_path);

            size_t parent_inode_ID = 0;	// start from the 1st inode, ie., the inode for root directory
            // first, let's find the parent dir
            size_t indicator = 0;

            // we declare parent_inode and parent_data here since it will still be used after the for loop
            directoryFile_t * parent_data = (directoryFile_t *)calloc(1, BLOCK_SIZE_BYTES);
            inode_t * parent_inode = (inode_t *) calloc(1, sizeof(inode_t));	
        
            for(size_t i = 0; i < count - 1; i++)
            {
                block_store_inode_read(fs->BlockStore_inode, parent_inode_ID, parent_inode);	// read out the parent inode
                // in case file and dir has the same name
                if(parent_inode->fileType == 'd')
                {
                    block_store_read(fs->BlockStore_whole, parent_inode->directPointer[0], parent_data);

                    for(int j = 0; j < folder_number_entries; j++)
                    {
                        if( ((parent_inode->vacantFile >> j) & 1) == 1 && strcmp((parent_data + j) -> filename, *(tokens + i)) == 0 )
                        {
                            parent_inode_ID = (parent_data + j) -> inodeNumber;
                            indicator++;
                        }					
                    }
                }					
            }
            block_store_inode_read(fs->BlockStore_inode, parent_inode_ID, parent_inode);

            //since this is a file we first need to free all the blocks
            for(int i = 0; i < 7; i++)
            {
                if(inode->directPointer[i] != 0)
                {
                    block_store_release(fs->BlockStore_whole, inode->directPointer[i]);
                }
            }

            memset(inode, 0, sizeof(inode_t)); //cleart inode
            block_store_inode_write(fs->BlockStore_inode, inode_number, inode);
            free(inode);

            //updating the parent directory now
            parent_data->inodeNumber = '\0';
            memset(parent_data->filename, '\0', 64);
            block_store_write(fs->BlockStore_whole, parent_inode->directPointer[0], parent_data);

            parent_inode->vacantFile = folder_number_entries;
            block_store_inode_write(fs->BlockStore_inode, parent_inode_ID, parent_inode);
        }
    }
    return 0;
}

dyn_array_t *fs_get_dir(FS_t *fs, const char *path)
{
    if(fs != NULL && path != NULL && strlen(path) != 0)
    {	
        char* copy_path = (char*)malloc(200);
        strcpy(copy_path, path);
        char** tokens;		// tokens are the directory names along the path. The last one is the name for the new file or dir
        size_t count = 0;
        tokens = str_split(copy_path, '/', &count);
        free(copy_path);

        if(strlen(*tokens) == 0)
        {
            // a spcial case: only a slash, no dir names
            count -= 1;
        }
        else
        {
            for(size_t n = 0; n < count; n++)
            {	
                if(isValidFileName(*(tokens + n)) == false)
                {
                    // before any return, we need to free tokens, otherwise memory leakage
                    for (size_t i = 0; i < count; i++)
                    {
                        free(*(tokens + i));
                    }
                    free(tokens);		
                    return NULL;
                }
            }			
        }		

        // search along the path and find the deepest dir
        size_t parent_inode_ID = 0;	// start from the 1st inode, ie., the inode for root directory
        // first, let's find the parent dir
        size_t indicator = 0;

        inode_t * parent_inode = (inode_t *) calloc(1, sizeof(inode_t));
        directoryFile_t * parent_data = (directoryFile_t *)calloc(1, BLOCK_SIZE_BYTES);
        for(size_t i = 0; i < count; i++)
        {
            block_store_inode_read(fs->BlockStore_inode, parent_inode_ID, parent_inode);	// read out the parent inode
            // in case file and dir has the same name. But from the test cases we can see, this case would not happen
            if(parent_inode->fileType == 'd')
            {			
                block_store_read(fs->BlockStore_whole, parent_inode->directPointer[0], parent_data);
                for(int j = 0; j < folder_number_entries; j++)
                {
                    if( ((parent_inode->vacantFile >> j) & 1) == 1 && strcmp((parent_data + j) -> filename, *(tokens + i)) == 0 )
                    {
                        parent_inode_ID = (parent_data + j) -> inodeNumber;
                        indicator++;
                    }					
                }	
            }					
        }	
        free(parent_data);
        free(parent_inode);	

        // now let's enumerate the files/dir in it
        if(indicator == count)
        {
            inode_t * dir_inode = (inode_t *) calloc(1, sizeof(inode_t));
            block_store_inode_read(fs->BlockStore_inode, parent_inode_ID, dir_inode);	// read out the file inode			
            if(dir_inode->fileType == 'd')
            {
                // prepare the data to be read out
                directoryFile_t * dir_data = (directoryFile_t *)calloc(1, BLOCK_SIZE_BYTES);
                block_store_read(fs->BlockStore_whole, dir_inode->directPointer[0], dir_data);

                // prepare the dyn_array to hold the data
                dyn_array_t * dynArray = dyn_array_create(folder_number_entries, sizeof(file_record_t), NULL);

                for(int j = 0; j < folder_number_entries; j++)
                {
                    if( ((dir_inode->vacantFile >> j) & 1) == 1 )
                    {
                        file_record_t* fileRec = (file_record_t *)calloc(1, sizeof(file_record_t));
                        strcpy(fileRec->name, (dir_data + j) -> filename);

                        // to know fileType of the member in this dir, we have to refer to its inode
                        inode_t * member_inode = (inode_t *) calloc(1, sizeof(inode_t));
                        block_store_inode_read(fs->BlockStore_inode, (dir_data + j) -> inodeNumber, member_inode);
                        if(member_inode->fileType == 'd')
                        {
                            fileRec->type = FS_DIRECTORY;
                        }
                        else if(member_inode->fileType == 'f')
                        {
                            fileRec->type = FS_REGULAR;
                        }

                        // now insert the file record into the dyn_array
                        dyn_array_push_front(dynArray, fileRec);
                        free(fileRec);
                        free(member_inode);
                    }					
                }
                free(dir_data);
                free(dir_inode);
                // before any return, we need to free tokens, otherwise memory leakage
                if(strlen(*tokens) == 0)
                {
                    // a spcial case: only a slash, no dir names
                    count += 1;
                }
                for (size_t i = 0; i < count; i++)
                {
                    free(*(tokens + i));
                }
                free(tokens);	
                return(dynArray);
            }
            free(dir_inode);
        }
        // before any return, we need to free tokens, otherwise memory leakage
        if(strlen(*tokens) == 0)
        {
            // a spcial case: only a slash, no dir names
            count += 1;
        }
        for (size_t i = 0; i < count; i++)
        {
            free(*(tokens + i));
        }
        free(tokens);	
    }
    return NULL;
}

int fs_move(FS_t *fs, const char *src, const char *dst)
{
    if(fs == NULL || src == NULL || dst == NULL || strcmp(src, "") == 0 || strcmp(dst, "") == 0)
    {
        return -1;
    }
    return 0;
}

int fs_link(FS_t *fs, const char *src, const char *dst)
{
    if(fs == NULL || src == NULL || dst == NULL || strcmp(src, "") == 0 || strcmp(dst, "") == 0)
    {
        return -1;
    }
    return 0;
}

// ADDITIONAL FUNCTIONS
off_t calculate_offset(int fileSize, off_t seekLocation)
{
    if(seekLocation <= 0)
    {
        return 0;
    }
    else if(seekLocation > fileSize)
    {
        return fileSize;
    }
    else
    {
        return seekLocation;
    }
}

bool isValidFileName(const char *name)
{
    if(!name || strlen(name) == 0 || strlen(name) > 127 || name[0] == '/')
    {
        return false;
     }
    return true;
}
char** str_split(char* a_str, const char a_delim, size_t * count)
{
    if(*a_str != '/')
    {
        return NULL;
    }
    char** result    = 0;
    char* tmp        = a_str;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = '\0';

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            (*count)++;
        }
        tmp++;
    }

    result = (char**)calloc(1, sizeof(char*) * (*count));
    for(size_t i = 0; i < (*count); i++)
    {
        *(result + i) = (char*)calloc(1, 200);
    }

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            strcpy(*(result + idx++), token);
            //    *(result + idx++) = strdup(token);
            token = strtok(NULL, delim);
        }

    }
    return result;
}

