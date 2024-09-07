#include <stdio.h>
#include <dirent.h>

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
            printf("%s\n", entry->d_name);
        }
    }

    printf("\n");
    closedir(dir);
    return 0;
}
