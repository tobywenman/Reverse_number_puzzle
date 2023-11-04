#include "title.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

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
    }
}

void destroyTitleGen()
{
    for (size_t letter=0; letter<ALPHABET_SIZE; letter++)
    {
        for (size_t i=0; i<CHAR_HEIGHT; i++)
        {
                free(letters[letter][i]);
        }
        free(letters[letter]);
    }
    free(letters);
}

size_t charToIdx(char in)
{
    if (in >= 97 && in <= 122)
    {
        return ((size_t)in)-97;
    }
    else
    {
        switch (in)
        {
        case '?': return 26;
        case '.': return 27;
        case '!': return 28;
        case '_': return 29;
        case '-': return 30;
        case '<': return 31;
        case '>': return 32;
        case ':': return 33;
        case ',': return 34;
        default:  return 35;
        }
    }
}

void printTitle(char* in)
{
    size_t len = strlen(in);
    for(unsigned row=0; row<CHAR_HEIGHT; row++)
    {
        for(unsigned i=0; i<len; i++)
        {
            size_t idx = charToIdx(in[i]);

            if (idx == 35)
            {
                for(unsigned j=0; j<CHAR_WIDTH; j++)
                {
                    printf(" ");
                }
            }
            else
            {
                printf("%s",letters[idx][row]);
            }
        }
        printf("\n");
    }
}

int main()
{
    initTitleGen("alphabet.txt");

    printTitle("hello world!");

    destroyTitleGen();
}