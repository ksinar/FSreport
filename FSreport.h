#ifndef FSREPORT_H_
#define FSREPORT_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

typedef struct file_inode{
    unsigned long inode;
    unsigned long size;
    unsigned long blocks;
    char *filename;
} Inode;

#endif
