
/*

To compile: gcc SearchHW2.c
To run: ./<executable> <commands> <directory>

*/

#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>

typedef struct
{
    int sVal;
    char fVal[300];
    char tVal[2];
    int S_arg;
    int f_level;
    int s_arg;
    int f_arg;
    int t_arg;

} progArgument;

typedef void Print_Args(char *pathToFile, char *directoryFilePath, progArgument proArgs, int tabCount);
void traverseDirectories(char *directoryName, Print_Args *printOutput, progArgument proArgs, int tabCount);

void printOutput(char *pathToFile, char *directoryFilePath, progArgument proArgs, int tabCount)
{
    struct stat buffer;
    lstat(pathToFile, &buffer);
    char output[100];
    strcpy(output, "");
    strcat(output, directoryFilePath);

    if (proArgs.S_arg)
    {
        char stringSize[10];
        sprintf(stringSize, " %d", (int)buffer.st_size);
        strcat(output, stringSize);
    }

    if (proArgs.f_arg)
    {
        if (strstr(directoryFilePath, proArgs.fVal) == NULL)
        {

            strcpy(output, "");
        }
        if (proArgs.f_level != -1)
            if (proArgs.f_level != tabCount)
            {
                strcpy(output, "");
            }
    }
    if (proArgs.t_arg)
    {
        if (strcmp(proArgs.tVal, "d") == 0)
        {
            if (S_ISREG(buffer.st_mode) != 0)
            {
                strcpy(output, "");
            }
        }
        if (strcmp(proArgs.tVal, "f") == 0)
        {
            if (S_ISDIR(buffer.st_mode) != 0)
            {
                strcpy(output, "");
            }
        }
    }
    if (proArgs.s_arg)
    {
        if (proArgs.sVal > (int)buffer.st_size)
        {
            strcpy(output, "");
        }
    }
    if (strcmp(output, "") != 0)
    {
        for (int i = 0; i <= tabCount; i++)
        {
            printf("\t");
        }
        printf("%s\n", output);
    }
}

void traverseDirectories(char *directoryName, Print_Args *printOutput, progArgument proArgs, int tabCount)
{
    struct dirent *dirent;
    DIR *parentDirectory = opendir(directoryName);
    if (parentDirectory == NULL)
    {
        printf("Error opening directory '%s'\n", directoryName);
        exit(-1);
    }
    while ((dirent = readdir(parentDirectory)) != NULL)
    {
        if (strcmp((*dirent).d_name, "..") != 0 &&
            strcmp((*dirent).d_name, ".") != 0)
        {
            char pathToFile[300];
            sprintf(pathToFile, "%s/%s", directoryName, ((*dirent).d_name));
            printOutput(pathToFile, (*dirent).d_name, proArgs, tabCount);
            if ((*dirent).d_type == DT_DIR)
            {
                tabCount++;
                traverseDirectories(pathToFile, printOutput, proArgs, tabCount);
                tabCount--;
            }
        }
    }
    closedir(parentDirectory);
}

int main(int argc, char **argv)
{
    int options = 0;

    progArgument proArgs;
    proArgs.S_arg = 0;
    proArgs.s_arg = 0;
    proArgs.f_arg = 0;
    proArgs.t_arg = 0;
    proArgs.f_level = -1;

    while ((options = getopt(argc, argv, "Ss:f:t:")) != -1)
    {
        switch (options)
        {
        case 'f':
            proArgs.f_arg = 1;
            if (optind < argc)
                proArgs.f_level = atoi(argv[optind]);
            // printf("%d", atoi(argv[optind]));
            strcpy(proArgs.fVal, optarg);
            break;

        case 's':
            proArgs.s_arg = 1;
            proArgs.sVal = atoi(optarg);
            break;
        case 't':
            proArgs.t_arg = 1;
            strcpy(proArgs.tVal, optarg);
            break;
        case 'S':
            proArgs.S_arg = 1;
            break;
        }
    }

    if (opendir(argv[argc - 1]) == NULL)
    {
        char defDirectory[299];
        getcwd(defDirectory, 299);
        printf("%s\n", defDirectory);
        traverseDirectories(defDirectory, printOutput, proArgs, 0);
        return 0;
    }
    printf("%s\n", argv[argc - 1]);
    traverseDirectories(argv[argc - 1], printOutput, proArgs, 0);
    return 0;
}
