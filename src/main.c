#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

void travelDirectory(char *directory_source){
    DIR *directory = opendir(directory_source);
    
    if(directory == NULL){
        perror(directory_source);
        return;
    }

    struct dirent *directory_entry_point;
    struct stat info;

    while((directory_entry_point = readdir(directory)) != NULL){
        if( strcmp(directory_entry_point->d_name, ".") == 0 || 
            strcmp(directory_entry_point->d_name, "..") == 0
        ){
            continue;
        }

        const char *relative_path = directory_entry_point->d_name;

        char *fullpath = strcat(directory_source, relative_path);   
        printf("%s", fullpath);

        if(lstat(directory_entry_point->d_name, &info) == -1){
            perror("lstat");
            return;
        }
        
        if(S_ISDIR(info.st_mode)) {
            printf("Caminho passado para a função: %s ", directory_entry_point->d_name);
            travelDirectory(directory_entry_point->d_name);
        }
        if(S_ISREG(info.st_mode)) printf("Name: %s\n", directory_entry_point->d_name);
    }

    closedir(directory);
}

int main(int argc, char *argv[])
{
    char *directory_source = malloc(sizeof(argv[1]));
    directory_source = realpath((argc == 1) ? "./" : argv[1], NULL);

    travelDirectory(directory_source);
    return 0;
}