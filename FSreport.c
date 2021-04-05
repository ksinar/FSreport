#include "FSreport.h"

int main(int argc, char **argv){
    /*
    if(argc != 3){
        printf("Usage: ./FSreport <report type> <path>\n");
        return -1;
    }
    */
    int fd = 0;
    fd = open(argv[1], O_RDONLY);

    struct stat filestat;
    fstat(fd,&filestat);

    printf("---------------------------\n");
    printf("%d: ",filestat.st_ino);
    printf("%d\t",filestat.st_size);
    printf("%d\t",filestat.st_blocks);
    printf("%d\t",(filestat.st_size / 512));
    printf("%s\n", argv[1]);
    printf("---------------------------\n");

    //printf("Number of Links: \t%d\n",filestat.st_nlink);
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
    close(fd);
    return 0;
}
