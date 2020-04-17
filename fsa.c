#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <ext2fs/ext2fs.h>

void printGeneralFileSystemInformation(struct ext2_super_block *ext2)
{
    printf("\r\n--General File System Information--");
    printf("\r\nBlock Size in Bytes: ...");
    printf("\b\b\b%d   ", (1024 << ext2->s_log_block_size));
      
    printf("\r\nTotal Number of Blocks: ...");
    printf("\b\b\b%d   ", ext2->s_blocks_count);

    printf("\r\nDisk Size in Bytes: ...");
    printf("\b\b\b%d   ", (1024 << ext2->s_log_block_size) * ext2->s_blocks_count);

    printf("\r\nMaximum Number of Blocks Per Group: ...");
    printf("\b\b\b%d   ", ext2->s_blocks_per_group);
    
    printf("\r\nInode Size in Bytes: ...");
    printf("\b\b\b%d   ", ext2->s_inode_size);
    
    printf("\r\nNumber of Inodes Per Group: ...");
    printf("\b\b\b%d   ", ext2->s_inodes_per_group);

    printf("\r\nNumber of Inode Blocks Per Group: ...");
    printf("\b\b\b%d   ", ext2->s_inodes_per_group * ext2->s_inode_size / (1024 << ext2->s_log_block_size));

    printf("\r\nNumber of Groups: ...");
    printf("\b\b\b%d   ", ext2->s_blocks_count / ext2->s_blocks_per_group);
}

void printIndividualGroupInformation(FILE* ext2, int block)
{
    printf("\r\n-Group ...-");
    printf("\b\b\b%d   ", block);

    printf("\r\nBlock IDs: ...");
    printf("\b\b\b%d   ",);
    
    printf("\r\nBlock Bitmap Block ID: ...");
    printf("\b\b\b%d   ",);

    printf("\r\nInode Bitmap Block ID: ...");
    printf("\b\b\b%d   ",);

    printf("\r\nInode Table Block ID: ...");
    printf("\b\b\b%d   ",);

    printf("\r\nNumber of Free Blocks: ...");
    printf("\b\b\b%d   ",);

    printf("\r\nNumber of Free Inodes: ...");
    printf("\b\b\b%d   ",);

    printf("\r\nNumber of Directories: ...");
    printf("\b\b\b%d   ",);

    printf("\r\nFree Block IDs: ...");
    printf("\b\b\b%d   ",);

    printf("\r\nFree Inode IDs: ...");
    printf("\b\b\b%d   ",);

    printf("\r\n");
}

void printAllGroupsInformation(FILE* ext2)
{
    printf("\r\n\r\n--Individual Group Information--");
    while(1)
    {
        if(1) //no more groups
            break;

        printIndividualGroupInformation(ext2, 0);
    }
}

void printRootDirectoryEntry(FILE* ext2)
{
    printf("\r\nInode: ...");
    printf("\r\nEntry Length: ...");
    printf("\r\nName Length: ...");
    printf("\r\nFile Type: ...");
    printf("\r\nName: ...");
}

void printRootDirectoryEntries(FILE* ext2)
{
    printf("\r\n\r\n--Root Directory Entries--");
    while(1)
    {
        if(1) //no more entries
            break;

        printRootDirectoryEntry(ext2);
    }
    
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("ERROR: Please include the path to a disk image as the first argument");
        exit(EXIT_FAILURE);
    }
    
    if (open(argv[1], O_RDONLY) == -1) {
        printf("disk_image_file open failed");
        exit(EXIT_FAILURE);
    }

    struct ext2_super_block *block = malloc(sizeof(struct ext2_super_block));
    FILE *ext2disk = fopen(argv[1], O_RDONLY);
     
    fseek(ext2disk, SUPERBLOCK_OFFSET, SEEK_SET);
    fread(block, sizeof(struct ext2_super_block), 1, ext2disk);

    printGeneralFileSystemInformation(block);

    int groupSize = (1024 << block->s_log_block_size) * block->s_blocks_per_group;
}