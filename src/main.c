#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

void travel_directory(char *directory_source){
    DIR *directory = opendir(directory_source);
    
    if(directory == NULL){
        perror(directory_source);
        return;
    }

    struct dirent *directory_entry_point;
    struct stat info;

    while((directory_entry_point = readdir(directory)) != NULL){
        if( strcmp(directory_entry_point->d_name, ".") == 0 || 
            strcmp(directory_entry_point->d_name, "..") == 0 ||
            strcmp(directory_entry_point->d_name, ".git") == 0 
        ){
            continue;
        }

        char path[4096];

        int result = snprintf(
            path,
            sizeof(path),
            "%s/%s",
            directory_source,
            directory_entry_point->d_name
        );

        if(result < 0 || (size_t) result >= sizeof(path)){
            fprintf(stderr, "Caminho muito longo: %s\n", directory_entry_point->d_name);
            continue;
        }

        if(lstat(path, &info) == -1){
            perror("lstat");
            return;
        }
        
        if(S_ISDIR(info.st_mode)) travel_directory(path);
        if(S_ISREG(info.st_mode)) printf("File Path: %s\n", path);
    }

    closedir(directory);
}

int main(int argc, char *argv[])
{
    char *directory_source = malloc(sizeof(argv[1]));
    directory_source = realpath((argc == 1) ? "./" : argv[1], NULL);

    travel_directory(directory_source);
    return 0;
}