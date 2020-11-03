#include "../hdr/includes.h"

char *getField(char *line, int fieldNum)
{
    char *tok;
    for (tok = strtok(line, ","); tok && *tok; tok = strtok(NULL, ",\n"))
    {
        if (!--fieldNum)
            return tok;
    }
    return NULL;
}

void readDictionary(char *fileName)
{
    FILE *stream = fopen(fileName, "r");

    char line[1024];
    int isMatching;
    while (fgets(line, 1024, stream))
    {
        char *tmp = strdup(line);
        int isMatching = atoi(getField(tmp, 3));
        if (isMatching)
        {
            updateCliques(getField(tmp, 1), getField(tmp, 2));
        }

        free(tmp);
    }
}
