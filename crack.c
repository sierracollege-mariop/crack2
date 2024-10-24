#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "md5.h"

void trim(char str[]);
void checkIfFileError(FILE *file, char *fileName);

const int PASS_LEN = 20;        // Maximum any password will be
const int HASH_LEN = 33;        // Length of MD5 hash strings


// Given a target plaintext word, use it to try to find
// a matching hash in the hashFile.
char * tryWord(char * plaintext, char * hashFilename)
{
    char *hash = md5(plaintext, strlen(plaintext));

    FILE *hashFile = fopen(hashFilename, "r");

    checkIfFileError(hashFile, hashFilename);

    char line[255];

    while (fgets(line, 255, hashFile) != NULL)
    {
        trim(line);

        if (strcmp(line, hash) == 0)
            return hash;
    }

    free(hash);
    fclose(hashFile);

    return NULL;
}


int main(int argc, char *argv[])
{
    if (argc < 3) 
    {
        fprintf(stderr, "Usage: %s hash_file dict_file\n", argv[0]);
        exit(1);
    }

    char *hashFileName = argv[1];

    char *dictFileName = argv[2];

    FILE *dictFile = fopen(dictFileName, "r");

    checkIfFileError(dictFile, dictFileName);

    int hashesCracked = 0;

    char line[255];

    while (fgets(line, 255, dictFile) != NULL)
    {
        trim(line);

        char *found = tryWord(line, hashFileName);

        if (found != NULL)
        {
            hashesCracked++;
            printf("%s %s\n", found, line);
            free(found);
        }
    }

    fclose(dictFile);


    printf("%d hashes cracked!\n", hashesCracked);
}

void trim(char str[])
{
    char *newLine = strchr(str, '\n');
    if (newLine) *newLine = '\0';
}

void checkIfFileError(FILE *file, char *fileName)
{
    if (file == NULL)
    {
        printf("Error trying to read file %s\n", fileName);
        exit(1);
    }
}