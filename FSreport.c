#include "FSreport.h"

int cmpfunc (const void * a, const void * b){
    unsigned long x = ((Inode*)a)->inode;
    unsigned long y = ((Inode*)b)->inode;

    return(x-y);
}

void print_inodes(DIR *dir, int level){
    struct dirent *directory = readdir(dir);
    printf("Level %i Inodes: %s\n", level, directory->d_name);
    int fd = 0;
    int file_counter = 0;
    Inode *files = malloc(sizeof(Inode) * 1000);
    while(directory != NULL){
        //printf("dirname: %s\n",directory.d_name);
        fd = open(directory->d_name, O_RDONLY);

        struct stat filestat;
        fstat(fd,&filestat);

        if((strcmp(directory->d_name,"..")) && (strcmp(directory->d_name,"."))){
            files[file_counter].filename = malloc(sizeof(char) * 200);

            files[file_counter].inode = filestat.st_ino;
            files[file_counter].size = filestat.st_size;
            files[file_counter].blocks = filestat.st_blocks;
            strcpy(files[file_counter].filename, directory->d_name);

            file_counter++;
        }
        directory = readdir(dir);


    }

    qsort(files,file_counter,sizeof(Inode),cmpfunc);
    for(int i = 0; i < file_counter; i++){

        printf("%10lu: \t",files[i].inode);
        printf("%lu\t",files[i].size);
        printf("%lu\t",files[i].blocks);
        printf("%lu\t",(files[i].size / 512));
        printf("%s\n", files[i].filename);
    }

        for(int i = 0; i < file_counter; i++){
            free(files[i].filename);
        }
        free(files);
}

int main(int argc, char **argv){
    int report_type = 0;
    if(argc != 3){
        printf("usage: %s (-tree or -inode) (directory path)\n", argv[0]);
        return -1;
    }
    if(!strcmp("-inode", argv[1])){
        report_type = 1;
    } else if(!strcmp("-tree", argv[1])){
        report_type = 2;
    }

    DIR *rootdir = NULL;
    rootdir = opendir(argv[2]);
    if(rootdir == NULL){
        fprintf(stderr, "could not open directory\n");
        exit(1);
    }
    switch(report_type){

        case 1: 

            printf("File System Report: Inodes\n");

            print_inodes(rootdir, 1);

            //    close(fd);

            break;

        case 2:
            printf("File System Report: Tree Directory Structure\n");
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
            printf("-tree stub\n");
            break;

        default:
            printf("usage: %s (-tree or -inode) (directory path)\n", argv[0]);
    }
    closedir(rootdir);
    return 0;
}
