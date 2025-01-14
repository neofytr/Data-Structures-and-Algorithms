#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

int random_between(int min, int max)
{
    if (min > max)
    {
        int temp = min;
        min = max;
        max = temp;
    }
    return rand() % (max - min + 1) + min;
}

#define NUM 10000000
#define MAX 10000000

int64_t method_one(int *arr)
{
    int64_t ret = 0;
    if (!arr)
    {
        return -1;
    }

    for (int64_t counter = 0; counter < NUM; counter++)
    {
        int flag = 0;
        for (int64_t index = 0; index < NUM; index++)
        {
            if (arr[index] == arr[counter] && index != counter)
            {
                flag = 1;
                break;
            }
        }
        if (!flag)
        {
            ret++;
        }
    }

    return ret;
}

int64_t method_two(int *arr)
{
    int64_t ret = 0;
    if (!arr)
    {
        return -1;
    }

    int *temp = (int *)calloc(MAX, sizeof(int));
    if (!temp)
    {
        return -1;
    }

    for (int64_t counter = 0; counter < NUM; counter++)
    {
        temp[arr[counter]]++;
    }

    for (int64_t counter = 0; counter < NUM; counter++)
    {
        if (temp[arr[counter]] == 1)
        {
            ret++;
        }
    }

    return ret;
}

int main()
{
    srand(time(NULL));
    int *arr = (int *)malloc(sizeof(int) * NUM);
    if (!arr)
    {
        return EXIT_FAILURE;
    }

    for (int64_t counter = 0; counter < NUM; counter++)
    {
        arr[counter] = random_between(0, MAX);
    }

    printf("%ld\n", method_two(arr));
}