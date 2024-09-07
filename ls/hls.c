#include <stdio.h>
#include <dirent.h>

/**
 * main - Enrty point for program
 * lists the contents of the current direcroty
 * similar to the 'ls' command
 * 
 * Return: 0 on success, 1 on failure
 */
int main(void)
{
    struct dirent *entry;
    DIR *dir = opendir(".");

    if (dir == NULL)
    {
        perror("opendir");
        return 1;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] != '.')
        {
            printf("%s  ", entry->d_name);
        }
    }

    printf("\n");
    closedir(dir);
    return 0;
}
