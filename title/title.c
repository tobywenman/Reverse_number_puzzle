#include "title.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ALPHABET_SIZE 35
#define CHAR_WIDTH 12
#define CHAR_HEIGHT 6

char ***letters;

void initTitleGen(char* fileName)
{
    letters = (char***)malloc(sizeof(char***)*ALPHABET_SIZE);

    for (size_t i=0; i<ALPHABET_SIZE; i++)
    {
        letters[i] = (char**)malloc(sizeof(char**)*CHAR_HEIGHT);
        for (size_t j=0; j<CHAR_HEIGHT; j++)
        {
            letters[i][j] = (char*)malloc(sizeof(char)*CHAR_WIDTH);
        }
    }

    FILE *fp;

    fp = fopen(fileName, "r");

    char buf[CHAR_WIDTH*CHAR_HEIGHT];

    for (size_t letter=0; letter<ALPHABET_SIZE; letter++)
    {
        for (size_t i=0; i<CHAR_HEIGHT; i++)
        {
            bool endLine = false;
            for (size_t j=0; j<CHAR_WIDTH-1; j++)
            {
                char nextChar;
                if (!endLine)
                {
                    nextChar = fgetc(fp);
                    if(nextChar == '\n')
                    endLine = true;
                }

                if(endLine)
                {
                    letters[letter][i][j] = ' ';
                }
                else
                {
                    letters[letter][i][j] = nextChar;
                }
            }
            letters[letter][i][CHAR_WIDTH-1] = '\0';
        }
        for (unsigned i=0; i<CHAR_HEIGHT; i++)
        {
            printf("%s\n",letters[letter][i]);
        }
    }
}

int main()
{
    initTitleGen("alphabet.txt");
}