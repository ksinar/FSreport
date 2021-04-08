#include "FSreport.h"

void swapInode(Inode *xp, Inode *yp)
{
    Inode temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void bubbleSortInode(Inode **files, int n)
{
    for (int i = 0; i < n - 1; i++){
        for (int j = 0; j < n - i - 1; j++){
            if (files[j]->inode > files[j + 1]->inode){
                swapInode(files[j], files[j + 1]);
            }
        }
    }
}

void print_tree(DIR *dir, int level, char *path){
    struct dirent *directory = readdir(dir);
    printf("Level %i: %s\n", level, path);
    int fd = 0;
    int file_counter = 0;
    int dir_counter = 0;
    TreeLevel **files = malloc(sizeof(TreeLevel *) * 1000);
    TreeLevel **directories = malloc(sizeof(TreeLevel *) * 1000);
    for(int i = 0; i < 1000; i++){
        files[i] = malloc(sizeof(TreeLevel));
        files[i]->permissions = malloc((sizeof(char) * 10) + 1);
        files[i]->ownership = malloc(sizeof(char) * 200);
        files[i]->filename = malloc(sizeof(char) * 200);
        files[i]->last_accessed = malloc(sizeof(char) * 200);
        files[i]->last_modified = malloc(sizeof(char) * 200);
        files[i]->inode = 0;
        files[i]->size = 0;

        directories[i] = malloc(sizeof(TreeLevel));
        directories[i]->permissions = malloc((sizeof(char) * 10) + 1);
        directories[i]->ownership = malloc(sizeof(char) * 200);
        directories[i]->filename = malloc(sizeof(char) * 200);
        directories[i]->last_accessed = malloc(sizeof(char) * 200);
        directories[i]->last_modified = malloc(sizeof(char) * 200);
        directories[i]->inode = 0;
        directories[i]->size = 0;
    }
    char **dir_names = malloc(sizeof(char *) * 100);
    for(int i = 0; i < 100; i++){
        dir_names[i] = malloc(sizeof(char) * 200);
    }
    while(directory != NULL){
        if((strcmp(directory->d_name,"..")) && (strcmp(directory->d_name,"."))){
            char *open_name = malloc(sizeof(char) * 1000);
            sprintf(open_name,"%s/%s",path,directory->d_name);
            fd = open(open_name, O_RDONLY);
            if(fd == -1){
                printf("Unable to open file. Errno: %d\n", errno);
                break;
            }
            struct stat filestat;
            fstat(fd,&filestat);
            struct passwd *pw = getpwuid(filestat.st_uid);
            struct group  *gr = getgrgid(filestat.st_gid);
            mode_t perm = filestat.st_mode;
            if(S_ISDIR(filestat.st_mode)){
                sprintf(dir_names[dir_counter],"%s%s",path, directory->d_name);
                directories[dir_counter]->inode = filestat.st_ino;
                directories[dir_counter]->size = filestat.st_size;
                sprintf(directories[dir_counter]->ownership,"%s (%s)", pw->pw_name, gr->gr_name);
                directories[dir_counter]->permissions[0] = 'd';
                directories[dir_counter]->permissions[1] = (perm & S_IRUSR) ? 'r' : '-';
                directories[dir_counter]->permissions[2] = (perm & S_IWUSR) ? 'w' : '-';
                directories[dir_counter]->permissions[3] = (perm & S_IXUSR) ? 'x' : '-';
                directories[dir_counter]->permissions[4] = (perm & S_IRGRP) ? 'r' : '-';
                directories[dir_counter]->permissions[5] = (perm & S_IWGRP) ? 'w' : '-';
                directories[dir_counter]->permissions[6] = (perm & S_IXGRP) ? 'x' : '-';
                directories[dir_counter]->permissions[7] = (perm & S_IROTH) ? 'r' : '-';
                directories[dir_counter]->permissions[8] = (perm & S_IWOTH) ? 'w' : '-';
                directories[dir_counter]->permissions[9] = (perm & S_IXOTH) ? 'x' : '-';
                directories[dir_counter]->permissions[10] = '\0';
                
                struct tm *ac;
                time_t acc_time;
                acc_time = filestat.st_atime;
                ac = localtime(&acc_time);
                char *accessed = asctime(ac);
                accessed[strcspn(accessed, "\n")] = 0;
                strcpy(directories[dir_counter]->last_accessed,accessed);

                struct tm *mo;
                time_t mod_time;
                mod_time = filestat.st_mtime;
                mo = localtime(&mod_time);
                char *modified = asctime(mo);
                modified[strcspn(modified, "\n")] = 0;
                strcpy(directories[dir_counter]->last_modified,modified);
                strcpy(directories[dir_counter]->filename,directory->d_name);
                dir_counter++;
            }else{
                files[file_counter]->inode = filestat.st_ino;
                files[file_counter]->size = filestat.st_size;
                sprintf(files[file_counter]->ownership,"%s (%s)", pw->pw_name, gr->gr_name);
                files[file_counter]->permissions[0] = '-';
                files[file_counter]->permissions[1] = (perm & S_IRUSR) ? 'r' : '-';
                files[file_counter]->permissions[2] = (perm & S_IWUSR) ? 'w' : '-';
                files[file_counter]->permissions[3] = (perm & S_IXUSR) ? 'x' : '-';
                files[file_counter]->permissions[4] = (perm & S_IRGRP) ? 'r' : '-';
                files[file_counter]->permissions[5] = (perm & S_IWGRP) ? 'w' : '-';
                files[file_counter]->permissions[6] = (perm & S_IXGRP) ? 'x' : '-';
                files[file_counter]->permissions[7] = (perm & S_IROTH) ? 'r' : '-';
                files[file_counter]->permissions[8] = (perm & S_IWOTH) ? 'w' : '-';
                files[file_counter]->permissions[9] = (perm & S_IXOTH) ? 'x' : '-';
                files[file_counter]->permissions[10] = '\0';

                struct tm *ac;
                time_t acc_time;
                acc_time = filestat.st_atime;
                ac = localtime(&acc_time);
                char *accessed = asctime(ac);
                accessed[strcspn(accessed, "\n")] = 0;
                strcpy(files[file_counter]->last_accessed,accessed);

                struct tm *mo;
                time_t mod_time;
                mod_time = filestat.st_mtime;
                mo = localtime(&mod_time);
                char *modified = asctime(mo);
                modified[strcspn(modified, "\n")] = 0;
                strcpy(files[file_counter]->last_modified,modified);
                strcpy(files[file_counter]->filename,directory->d_name);
                file_counter++;
            }
            free(open_name);
        }
        directory = readdir(dir);
        close(fd);
    }

    if(dir_counter > 0){
        printf("Directories\n");
    }

    for(int i = 0; i < dir_counter; i++){
        
        printf("%s\t", directories[i]->ownership);
        printf("%10lu\t", directories[i]->inode);
        printf("%s\t", directories[i]->permissions);
        printf("%lu\t",directories[i]->size);
        printf("%s\n", directories[i]->filename);

        printf("%s\t", directories[i]->last_accessed);
        printf("%s\n", directories[i]->last_modified);
    }
    printf("\n");

    if(file_counter > 0){
        printf("Files\n");
    }

    for(int i = 0; i < file_counter; i++){
        
        printf("%s\t", files[i]->ownership);
        printf("%10lu\t", files[i]->inode);
        printf("%s\t", files[i]->permissions);
        printf("%lu\t",files[i]->size);
        printf("%s\n", files[i]->filename);

        printf("%s\t", files[i]->last_accessed);
        printf("%s\n", files[i]->last_modified);
    }
    printf("\n");

    for(int i = 0; i < dir_counter; i++){
        DIR *dir_recur = opendir(dir_names[i]);
        if(dir_recur != NULL){
            print_tree(dir_recur,level+1,dir_names[i]);
            closedir(dir_recur);
        }
    }

    for(int i = 0; i < 100; i++){
        free(dir_names[i]);
    }
    free(dir_names);

    for(int i = 0; i < 1000; i++){
        free(files[i]->ownership);
        free(files[i]->permissions);
        free(files[i]->last_modified);
        free(files[i]->last_accessed);
        free(files[i]->filename);
        free(files[i]);

        free(directories[i]->ownership);
        free(directories[i]->permissions);
        free(directories[i]->last_modified);
        free(directories[i]->last_accessed);
        free(directories[i]->filename);
        free(directories[i]);
    }
    free(files);
    free(directories);
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

    bubbleSortInode(files, file_counter);

    for(int i = 0; i < file_counter; i++){

        printf("%10lu: \t",files[i]->inode);
        printf("%lu\t",files[i]->size);
        printf("%lu\t",files[i]->blocks);
        printf("%lu\t",(files[i]->size / 512));
        printf("%s\n", files[i]->filename);

    }
    printf("\n");

    for(int i = 0; i < dir_counter; i++){
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

    int len = strlen(argv[2]);
    if(argv[2][len - 1] != '/'){
        strcat(argv[2], "/");
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

            print_tree(rootdir, 1, argv[2]);

            break;

        default:
            printf("usage: %s (-tree or -inode) (directory path)\n", argv[0]);
    }
    closedir(rootdir);
    return 0;
}
