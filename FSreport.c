#include "FSreport.h"
#include <errno.h>

int cmpfunc (const void * a, const void * b){
    Inode *x = (Inode *)a;
    Inode *y = (Inode *)b;
    //printf("Comparing inodes %s and %s\n",x->filename, y->filename);

    if(x->inode > y->inode){
        //printf("%10lu is greater than %10lu\n", x->inode, y->inode);
        return 1;
    }else{
        //printf("%10lu is greater than %10lu\n", y->inode, x->inode);
        return -1;
    }
}

void print_inodes(DIR *dir, int level, char *path){
    struct dirent *directory = readdir(dir);
    printf("Level %i Inodes: %s\n", level, path);
    int fd = 0;
    int file_counter = 0;
    int dir_counter = 0;
    Inode **files = malloc(sizeof(Inode *) * 1000);
    for(int i = 0; i < 1000; i++){
        files[i] = malloc(sizeof(Inode));
        files[i]->filename = malloc(sizeof(char) * 200);
        files[i]->blocks = 0;
        files[i]->inode = 0;
        files[i]->size = 0;
    }
    char **directories = malloc(sizeof(char*) * 100);
    for(int i = 0; i < 100; i++){
        directories[i] = malloc(sizeof(char) * 200);
    }
    while(directory != NULL){
        if((strcmp(directory->d_name,"..")) && (strcmp(directory->d_name,"."))){
        char *open_name = malloc(sizeof(char) * 1000);
        sprintf(open_name,"%s/%s",path,directory->d_name);
        //printf("%s\n", open_name);
        fd = open(open_name, O_RDONLY);
        if(fd == -1){
            printf("Unable to open file. Errno: %d\n", errno);
            break;
        }
        struct stat filestat;
        fstat(fd,&filestat);

            files[file_counter]->inode = filestat.st_ino;
            files[file_counter]->size = filestat.st_size;
            files[file_counter]->blocks = filestat.st_blocks;
            strcpy(files[file_counter]->filename, directory->d_name);

            file_counter++;
            if(S_ISDIR(filestat.st_mode)){
                sprintf(directories[dir_counter],"%s%s",path, directory->d_name);
                dir_counter++;
            }
            free(open_name);
        }
        directory = readdir(dir);
        close(fd);
        
    }

    qsort(files,file_counter,sizeof(Inode),cmpfunc);

    for(int i = 0; i < file_counter; i++){

        printf("%10lu: \t",files[i]->inode);
        printf("%lu\t",files[i]->size);
        printf("%lu\t",files[i]->blocks);
        printf("%lu\t",(files[i]->size / 512));
        printf("%s\n", files[i]->filename);
        
    }
    printf("\n");

    for(int i = 0; i < dir_counter; i++){
        //printf("%s\n",directories[i]);
        DIR *dir_recur = opendir(directories[i]);
        if(dir_recur != NULL){
            print_inodes(dir_recur,level+1,directories[i]);
            closedir(dir_recur);
        }
    }

    for(int i = 0; i < 100; i++){
        free(directories[i]);
    }
    free(directories);

    for(int i = 0; i < 1000; i++){
        free(files[i]->filename);
        free(files[i]);
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

            print_inodes(rootdir, 1, argv[2]);

            break;

        case 2:
            printf("File System Report: Tree Directory Structure\n");
            printf("-tree stub\n");
            break;

        default:
            printf("usage: %s (-tree or -inode) (directory path)\n", argv[0]);
    }
    closedir(rootdir);
    return 0;
}
