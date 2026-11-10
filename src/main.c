#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>

int main(int argc, char *argv[]){
    char *directory_source = malloc(sizeof(argv[1]));
    directory_source = (argc == 1) ? "." : argv[1];

    DIR *directory = opendir(directory_source);

    if(directory == NULL){
        perror("opendir");
        return 1;
    }

    struct dirent *directory_entry_point;
    errno = 0;

    while((directory_entry_point = readdir(directory)) != NULL){
        printf("Name: %s\n", directory_entry_point->d_name);
        printf("Inode: %lu\n", directory_entry_point->d_ino);
        errno = 0;
    }

    if(errno != 0){
        perror("readdir");
        closedir(directory);
        return 1;
    }

    if(closedir(directory) != 0){
        perror("closedir");
        return 1;
    }

    return 0;
}