#include "../hdr/includes.h"

#define MAXLINE 1024

char *getField(char *line, int fieldNum)
{
    char *tok;
    for (tok = strtok(line, ","); tok && *tok; tok = strtok(NULL, ",\n"))
    {
        if (!--fieldNum)
        {
            return tok;
        }
    }
    return NULL;
}

void readDictionary(char *fileName, HashTable *hashTable)
{
    FILE *stream = fopen(fileName, "r");
    if (stream == NULL)
    {
        printf("Could not create %s\n", fileName);
        exit(EXIT_FAILURE);
    }

    char line[MAXLINE];
    while (fgets(line, MAXLINE, stream))
    {
        char *tmp1 = strdup(line), *tmp2 = strdup(line), *tmp3 = strdup(line);
        int isMatching = atoi(getField(tmp1, 3));
        if (isMatching)
        {
            updateCliques(getField(tmp2, 1), getField(tmp3, 2), hashTable);
        }

        free(tmp1);
        free(tmp2);
        free(tmp3);
    }

    fclose(stream);
}
