#include <inttypes.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int ceil_log2(unsigned x)
{
    return (sizeof(unsigned)*8) - __builtin_clz(x);
}

unsigned ipow(unsigned base, unsigned exp)
{
    unsigned result = 1;
    for (;;)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (!exp)
            break;
        base *= base;
    }

    return result;
}

struct params
{
    uint64_t *values;
    uint64_t **workingArray; //2D array to store current and next step
    uint64_t maxVal;

    size_t numDigits;
    size_t numSteps;
};

struct params calcMultValues(size_t size, unsigned base)
{
    struct params out;

    out.maxVal = ipow(base,size);

    out.numDigits = size;

    out.numSteps = ceil_log2(size);

    unsigned arraySize = 1 << size;

    uint64_t multValue = base;

    out.values = (uint64_t*)malloc(out.numSteps*sizeof(uint64_t));

    out.workingArray = (uint64_t**)malloc(sizeof(uint64_t *) * 2);
    out.workingArray[0] = (uint64_t*)malloc(sizeof(uint64_t) * arraySize);
    out.workingArray[1] = (uint64_t*)malloc(sizeof(uint64_t) * arraySize);

    for (size_t i=out.numSteps; i-- > 0;)
    {
        out.values[i] = multValue;
        multValue *= multValue;
    }

    return out;
}

struct num
{
    bool valid;
    char *data;
};

void allocNum(struct num *in, struct params param)
{
    in->valid = true;
    in->data = malloc(sizeof(char)*(param.numDigits+1));
}

char intToAscii(unsigned in)
{
    static const char ascii[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
    return ascii[in];
}

void intToString(struct params param, uint64_t data, struct num* out, bool forwards)
{
    if (data >= param.maxVal)
    {
        out->valid = false;
        return;
    }

    unsigned numChars = 2;
    unsigned prevWorkArray = 0;
    unsigned curWorkArray = 1;
    param.workingArray[0][0] = data;

    for (unsigned i=0; i<param.numSteps; i++)
    {
        for (unsigned j=0; j<numChars; j+=2)
        {
            param.workingArray[curWorkArray][j] = param.workingArray[prevWorkArray][j/2] / param.values[i];
            param.workingArray[curWorkArray][j+1] = param.workingArray[prevWorkArray][j/2] % param.values[i];
        }
        numChars <<= 1;
        if (prevWorkArray)
        {
            prevWorkArray = 0;
            curWorkArray = 1;
        }
        else
        {
            curWorkArray = 0;
            prevWorkArray = 1;
        }
    }

    for (unsigned i=0; i<param.numDigits; i++)
    {
        if (forwards)
        {
            out->data[param.numDigits-i-1] = intToAscii(param.workingArray[prevWorkArray][(1<<param.numSteps)-i-1]);            
        }
        else
        {
            out->data[i] = intToAscii(param.workingArray[prevWorkArray][(1<<param.numSteps)-i-1]);
        }
    }
    out->data[param.numDigits] = 0;
}

bool check(struct num forward, struct num backward, size_t len)
{
    
    if (!forward.valid || !backward.valid)
    {
        return false;
    }
    for (unsigned i=0; i<len; i++)
    {
        if (forward.data[i]!=backward.data[i])
        {
            return false;
        }
    }
    return true;
}

int main()
{
    size_t size = 6;
    unsigned base = 10;
    unsigned mult = 4;
    struct params vals;

    vals = calcMultValues(size, base);

    struct num forward;
    struct num backward;

    allocNum(&forward, vals);
    allocNum(&backward, vals);

    for (unsigned i=0; i<vals.maxVal/mult; i++)
    {
        intToString(vals, i, &forward, true);
        intToString(vals, i*mult, &backward, false);

        if (check(forward,backward,size))
        {
            printf(">>>>>>value found: %s\n", forward.data);
        }
    }

    return 0;
}