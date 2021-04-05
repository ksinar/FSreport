#include "FSreport.h"

int main(int argc, char **argv){
    /*
    if(argc != 3){
        printf("Usage: ./FSreport <report type> <path>\n");
        return -1;
    }
    */

    DIR *rootdir = NULL;
    rootdir = opendir(argv[1]);
    if(rootdir == NULL){
        fprintf(stderr, "could not open directory\n");
        exit(1);
    }
    printf("---------------------------\n");
    printf("Level 1 Inodes: %s\n", argv[1]);
    struct dirent *directory = readdir(rootdir);
    int fd = 0;
    while(directory != NULL){
        //printf("dirname: %s\n",directory->d_name);


        fd = open(directory->d_name, O_RDONLY);

        struct stat filestat;
        fstat(fd,&filestat);

        printf("%10d: \t",filestat.st_ino);
        printf("%d\t",filestat.st_size);
        printf("%d\t",filestat.st_blocks);
        printf("%d\t",(filestat.st_size / 512));
        printf("%s\n", directory->d_name);
        directory = readdir(rootdir);
    }
        printf("---------------------------\n");

/*
    printf("File Permissions: \t");
    printf( (S_ISDIR(filestat.st_mode)) ? "d" : "-");
    printf( (filestat.st_mode & S_IRUSR) ? "r" : "-");
    printf( (filestat.st_mode & S_IWUSR) ? "w" : "-");
    printf( (filestat.st_mode & S_IXUSR) ? "x" : "-");
    printf( (filestat.st_mode & S_IRGRP) ? "r" : "-");
    printf( (filestat.st_mode & S_IWGRP) ? "w" : "-");
    printf( (filestat.st_mode & S_IXGRP) ? "x" : "-");
    printf( (filestat.st_mode & S_IROTH) ? "r" : "-");
    printf( (filestat.st_mode & S_IWOTH) ? "w" : "-");
    printf( (filestat.st_mode & S_IXOTH) ? "x" : "-");
    printf("\n\n");

    printf("The file %s a symbolic link\n\n", (S_ISLNK(filestat.st_mode)) ? "is" : "is not");
*/
//    close(fd);

    free(rootdir);
    return 0;
}
