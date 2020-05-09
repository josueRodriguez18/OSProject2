
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

void listFilesRecursively(char *path);
void findName(char *path, char *name, int rm, char *execType, char *execArg);
void findMin(char *basePath, int mode, int min, int rm, char *execType, char *execArg);
void findNode(char *basePath, int inode, int rm, char *execType, char *execArg);
int critParser(char *arg);
void commander(char *path, char *execType, char *execArg);
int main()
{
    // Directory path to list files
    char userIN[500];
    char *execType = "0";
    int rm = 0;
    int numArgs;
    char bigarg[8][25] = {{0}, {0}, {0}, {0}, {0}, {0}};
    char *init = bigarg[5];
    gets(userIN);                      // take user input
    char *token = strtok(userIN, " "); //first space delimited argument
    for (numArgs = 0; token != NULL && numArgs < 6; numArgs++)
    {                                   //check input for all args
        strcpy(bigarg[numArgs], token); //copy the argument into an array for forther use
        token = strtok(NULL, " ");      //move the token to the next arg
    }
    if (strcmp(bigarg[0], "find")) // check to see if find is the first keyword
    {
        printf("\nfind keyword missing, exiting"); //input is not in correct format
        scanf(userIN);                             //pause
        return 0;                                  //exit
    }
    else
    {
        if (numArgs == 1)
        {
            listFilesRecursively(".");
        }
        else if (numArgs == 2)
        {
            listFilesRecursively(bigarg[1]);
        }
        else if (numArgs == 3)
        {
            if (!strcmp(bigarg[1], "-name"))
            {
                findName(".", bigarg[2], 0, execType, NULL);
            }
            else if (!strcmp(bigarg[1], "-nmin"))
            {
                printf("\n FILE                DATE  \n");
                findMin(".", critParser(bigarg[2]), abs(atoi(bigarg[2])), 0, execType, NULL);
            }
            else if (!strcmp(bigarg[1], "-inum"))
            {
                findNode(".", abs(atoi(bigarg[2])), 0, execType, NULL);
            }
        }
        else if (numArgs == 4)
        {
            if (!strcmp(bigarg[3], "-delete"))
            {
                if (!strcmp(bigarg[1], "-name")) //find location criteria critArg
                {
                    findName(".", bigarg[2], 1, execType, NULL);
                }
                else if (!strcmp(bigarg[1], "-nmin"))
                {
                    printf("\n FILE                DATE  \n");
                    findMin(".", critParser(bigarg[2]), abs(atoi(bigarg[2])), 1, 0, NULL);
                }
                else if (!strcmp(bigarg[1], "-inum"))
                {
                    findNode(".", abs(atoi(bigarg[2])), 1, execType, NULL);
                }
            }

            else
            {
                if (!strcmp(bigarg[2], "-name")) //find location criteria critArg
                {
                    findName(bigarg[1], bigarg[3], 0, 0, NULL);
                }
                else if (!strcmp(bigarg[2], "-nmin"))
                {
                    printf("\n FILE                DATE  \n");

                    findMin(bigarg[1], critParser(bigarg[3]), abs(atoi(bigarg[3])), 0, 0, NULL);
                }
                else if (!strcmp(bigarg[2], "-inum"))
                {
                    findNode(bigarg[1], abs(atoi(bigarg[3])), 0, 0, NULL);
                }
            }
        }
        else if (numArgs == 5)
        {
            if (!strcmp(bigarg[4], "-delete"))
            {
                if (!strcmp(bigarg[2], "-name")) //find location criteria critArg
                {
                    findName(bigarg[1], bigarg[3], 1, 0, NULL);
                }
                else if (!strcmp(bigarg[2], "-nmin"))
                {
                    printf("\n FILE                DATE  \n");
                    findMin(bigarg[1], critParser(bigarg[3]), abs(atoi(bigarg[3])), 1, 0, NULL);
                }
                else if (!strcmp(bigarg[2], "-inum"))
                {
                    findNode(bigarg[1], abs(atoi(bigarg[3])), 1, 0, NULL);
                }
            }
            else if (!strcmp(bigarg[3], "-exec"))
            {

                if (!strcmp("rm", bigarg[4]))
                {
                    rm = 1;
                }
                if (!strcmp("-name", bigarg[1]))
                {
                    findName(".", bigarg[2], rm, bigarg[4], NULL);
                }
                else if (!strcmp("-nmin", bigarg[1]))
                {
                    printf("\n FILE                DATE  \n");
                    findMin(".", critParser(bigarg[2]), abs(atoi(bigarg[2])), rm, bigarg[4], NULL);
                }
                else if (!strcmp("-inum", bigarg[1]))
                {
                    findNode(".", abs(atoi(bigarg[2])), rm, bigarg[4], NULL);
                }
            }
        }
        else if (numArgs == 6)
        {
            if (!strcmp("-exec", bigarg[3]))
            {
                if (!strcmp("-name", bigarg[1]))
                {
                    findName(".", bigarg[2], 0, bigarg[4], bigarg[5]);
                }
                else if (!strcmp("-nmin", bigarg[1]))
                {
                    printf("\n FILE                DATE  \n");
                    findMin(".", critParser(bigarg[2]), abs(atoi(bigarg[2])), 0, bigarg[4], bigarg[5]);
                }
                else if (!strcmp("-inum", bigarg[1]))
                {
                    findNode(".", abs(atoi(bigarg[2])), 0, bigarg[4], bigarg[5]);
                }
            }

            if (!strcmp("-exec", bigarg[4])) //find location criteria critArg exec command
            {
                if (!strcmp("rm", bigarg[5]))
                {
                    rm = 1;
                }
                if (!strcmp("-name", bigarg[2]))
                {
                    findName(bigarg[1], bigarg[3], rm, bigarg[5], execType);
                }
                else if (!strcmp("-nmin", bigarg[2]))
                {
                    printf("\n FILE                DATE  \n");
                    findMin(bigarg[1], critParser(bigarg[3]), abs(atoi(bigarg[3])), rm, bigarg[5], execType);
                }
                else if (!strcmp("-inum", bigarg[2]))
                {
                    findNode(bigarg[1], abs(atoi(bigarg[3])), rm, bigarg[5], execType);
                }
            }
        }
        else if (numArgs == 7) //find location criteria critArg exec mv mvarg
        {
            if (!strcmp("-name", bigarg[2]))
            {
                findName(bigarg[1], bigarg[3], 4, bigarg[5], bigarg[6]);
            }
            else if (!strcmp("-nmin", bigarg[2]))
            {
                printf("\n FILE                DATE  \n");
                findMin(bigarg[1], critParser(bigarg[3]), abs(atoi(bigarg[3])), 0, bigarg[5], bigarg[6]);
            }
            else if (!strcmp("-inum", bigarg[2]))
            {
                findNode(bigarg[1], abs(atoi(bigarg[3])), 0, bigarg[5], bigarg[6]);
            }
        }


        return 0;
    }
}
/**
 * Lists all files and sub-directories recursively 
 * considering path as base path.
 */
void listFilesRecursively(char *basePath)
{
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);
    // Unable to open directory stream
    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {

            // Construct new path from our base path
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);
            printf("%s\n", path);
            listFilesRecursively(path);
        }
    }
    closedir(dir);
}

void findName(char *basePath, char *name, int rm, char *execType, char *execArg)
{
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);
    // Unable to open directory stream
    if (!dir)
        return;
    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);
            if (!strcmp(name, dp->d_name))
            {
                printf("%s\n", path);
                if (rm)
                {
                    remove(path);
                    printf("\nRemoved");
                }
                if (strcmp(execType, "0"))
                {
                    commander(path, execType, execArg);
                }
            }
            // Construct new path from our base path
            findName(path, name, rm, execType, execArg);
        }
    }
    closedir(dir);
}

//0 = less      1 = exactly     2 = more than
void findMin(char *basePath, int mode, int min, int rm, char *execType, char *execArg)
{
    char path[1000];
    struct dirent *dp;
    struct stat stats;
    int window = time(NULL) - 60 * min;
    DIR *dir = opendir(basePath);
    // Unable to open directory stream
    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);
            stat(path, &stats);
            int window = time(NULL) - min * 60;
            if (stats.st_mtime > window && mode == 0)
            {
                printf("\n%s ||  %s", path, ctime(&stats.st_mtim));
                if (rm)
                {
                    remove(path);
                    printf("\nRemoved");
                }
                if (strcmp(execType, "0"))
                {
                    commander(path, execType, execArg);
                }
            }
            else if (stats.st_mtime < window && mode == 2)
            {
                printf("\n%s ||  %s", path, ctime(&stats.st_mtim));
                if (rm)
                {
                    remove(path);
                    printf("\nRemoved");
                }
                if (strcmp(execType, "0"))
                {
                    commander(path, execType, execArg);
                }
            }
            else if (stats.st_mtime == window && mode == 1)
            {
                printf("\n%s ||  %s", path, ctime(&stats.st_mtim));
                if (rm)
                {
                    remove(path);
                    printf("\nRemoved");
                }
                if (strcmp(execType, "0"))
                {
                    commander(path, execType, execArg);
                }
            }
            // Construct new path from our base path
            findMin(path, mode, min, rm, execType, execArg);
        }
    }
    closedir(dir);
}

void findNode(char *basePath, int inode, int rm, char *execType, char *execArg)
{
    char path[1000];
    struct dirent *dp;
    struct stat stats;
    DIR *dir = opendir(basePath);
    // Unable to open directory stream
    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);
            stat(path, &stats);

            // Construct new path from our base path
            if ((int)stats.st_ino == inode)
            {
                printf("\n%s", path);
                if (rm)
                {
                    remove(path);
                    printf("\nRemoved");
                }
                if (strcmp(execType, "0"))
                {
                    commander(path, execType, execArg);
                }
            }
            findNode(path, inode, rm, execType, execArg);
        }
    }
    closedir(dir);
}

int critParser(char *arg)
{
    switch (arg[0])
    {
    case '+':
        return 2;
        break;
    case '-':
        return 0;
        break;
    default:
        return 1;
        break;
    }
}

void commander(char *path, char *execType, char *execArg)
{
    char temp[400];
    printf("%s", execArg);
    strcpy(temp, execType);
    strcat(temp, " ");
    strcat(temp, path);
    if (!strcmp("mv", execType))
    {
        strcat(temp, " ");
        strcat(temp, execArg);
        system(temp);
    }
    else if (!strcmp("cat", execType))
    {
        system(temp);
    }
    return;
}
