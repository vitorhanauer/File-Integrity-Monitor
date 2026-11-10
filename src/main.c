#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>

const char* checkMode(struct stat *info)
{
    if (S_ISREG(info->st_mode))
    {
        return "FILE";
    }
    else if (S_ISDIR(info->st_mode))
    {
        return "DIR";
    }
    else if (S_ISLNK(info->st_mode))
    {
        return "LINK";
    }

    return "";
}

int main(int argc, char *argv[])
{
    char *directory_source = malloc(sizeof(argv[1]));
    struct dirent *directory_entry_point;
    struct stat info;
    errno = 0;

    directory_source = (argc == 1) ? "." : argv[1];
    DIR *directory = opendir(directory_source);

    if (directory == NULL)
    {
        perror("opendir");
        return 1;
    }

    while ((directory_entry_point = readdir(directory)) != NULL)
    {
        if (lstat(directory_entry_point->d_name, &info) == -1)
        {
            perror("lstat");
            return 1;
        }

        printf("[%s] \t Name: %s, Inode: %lu\n", checkMode(&info), directory_entry_point->d_name, directory_entry_point->d_ino);
        errno = 0;
    }

    if (errno != 0)
    {
        perror("readdir");
        closedir(directory);
        return 1;
    }

    if (closedir(directory) != 0)
    {
        perror("closedir");
        return 1;
    }

    return 0;
}