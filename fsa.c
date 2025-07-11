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

void printIndividualGroupInformation(int blockGroup, struct ext2_super_block *block, struct ext2_group_desc desc);
{
    printf("\r\n-Group ...-");
    printf("\b\b\b%d   ", blockGroup);

    printf("\r\nBlock IDs: ...");
    if(blockGroup * (block->s_blocks_per_group + 1) <= block->s_blocks_count)
        printf("\b\b\b%d-%d  ", blockGroup * block->s_blocks_per_group + 1, blockGroup * (block->s_blocks_per_group + 1));
    else
        printf("\b\b\b%d-%d  ", blockGroup * block->s_blocks_per_group + 1, block->s_blocks_count);
    
    printf("\r\nBlock Bitmap Block ID: ...");
    printf("\b\b\b%d   ", desc->bg_block_bitmap);

    printf("\r\nInode Bitmap Block ID: ...");
    printf("\b\b\b%d   ", desc->bg_inode_bitmap);

    printf("\r\nInode Table Block ID: ...");
    printf("\b\b\b%d   ", desc->bg_inode_table);

    printf("\r\nNumber of Free Blocks: ...");
    printf("\b\b\b%d   ", desc->bg_free_blocks_count);

    printf("\r\nNumber of Free Inodes: ...");
    printf("\b\b\b%d   ", desc->bg_free_inodes_count);

    printf("\r\nNumber of Directories: ...");
    printf("\b\b\b%d   ", desc->bg_used_dirs_count);

    printf("\r\nFree Block IDs: ");
    int i = 0;
    while(i < 32)
    {
        if(1 & desc->bg_block_bitmap >> i)
        {
            printf("%d", blockGroup * block->s_blocks_per_group + ++i);
            while(1)
            {
                if(!(1 & desc->bg_block_bitmap >> i) || i >= 32)
                {
                    printf("-%d", i);
                    break;
                }
                i++;
            }
            printf(", ");
        }
        
        i++;
    }
    

    printf("\r\nFree Inode IDs: ");
    i = 0;
    while(i < 32)
    {
        if(1 & desc->bg_inode_bitmap >> i)
        {
            printf("%d", blockGroup * (block->s_inodes_per_group * block->s_inode_size / (1024 << block->s_log_block_size)) + ++i);
            while(1)
            {
                if(!(1 & desc->bg_block_bitmap >> i) || i >= 32)
                {
                    printf("-%d", i);
                    break;
                }
                i++;
            }
            printf(", ");
        }
        
        i++;
    }    

    printf("\r\n");
}

void printAllGroupsInformation(FILE* ext2, struct ext2_super_block *block)
{
    printf("\r\n\r\n--Individual Group Information--");
    while(1)
    {
        if(ftell(ext2) >= (1024 << block->s_log_block_size) * block->s_blocks_count)) //no more groups
            break;

        struct ext2_group_desc *desc = malloc(sizeof(struct ext2_group_desc));
        fseek(ext2disk, block->s_blocks_per_group * (1024 << block->s_log_block_size) - 32, SEEK_CUR);
        fread(desc, (sizeof(struct ext2_group_desc), 1, ext2disk);
        printIndividualGroupInformation(int blockGroup, struct ext2_super_block *block, struct ext2_group_desc desc);
        free(desc);
    }
}

void printEntry(struct ext2_dir_entry2 *entry)
{
    printf("\r\nInode: ...");
    printf("\b\b\b%d   ", entry->inode);

    printf("\r\nEntry Length: ...");
    printf("\b\b\b%d   ", entry->rec_len);

    printf("\r\nName Length: ...");
    printf("\b\b\b%d   ", entry->name_len);

    printf("\r\nFile Type: ...");
    printf("\b\b\b%d   ", entry->file_type);

    printf("\r\nName: ...");
    printf("\b\b\b%s   ", entry->name);
}

void printRootDirectoryEntries(FILE* ext2, struct ext2_super_block *block, struct ext2_group_desc desc)
{
    printf("\r\n\r\n--Root Directory Entries--");

    struct ext2_dir_entry2 *entry = malloc(sizeof(struct ext_dir_entry2));
    struct ext2_inode *node = malloc(sizeof(struct ext2_inode));
    fseek(ext2disk, desc->bg_inode_table * 1024 + sizeof(struct ext2_inode), SEEK_SET);
    fread(node, sizeof(struct ext2_inode), 1, ext2disk);
    entry = node->i_block[1];
    while(entry->Inode > 0)
    {
        printEntry(entry);
        entry = entry + entry->rec_len;
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

    // int groupSize = (1024 << block->s_log_block_size) * block->s_blocks_per_group;

    // fread(desc, (sizeof(struct ext2_group_desc), 1, ext2disk);

    printAllGroupsInformation(ext2disk, block);

    printRootDirectoryEntries(ext2disk, block);


    free(block);
}