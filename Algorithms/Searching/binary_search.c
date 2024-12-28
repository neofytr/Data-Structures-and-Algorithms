#include <stdio.h>
#include <stdlib.h>
#include "../../Strix/header/strix.h"

void insertion_sort(int64_t *nums, size_t len)
{
    if (!nums || !len)
    {
        return;
    }
    for (size_t counter = 1; counter < len; counter++)
    {
        size_t j = counter - 1;
        while (j > 0 && nums[j] < nums[j - 1])
        {
            int64_t temp = nums[j - 1];
            nums[j - 1] = nums[j];
            nums[j] = temp;
            j--;
        }
    }
}

int64_t binary_search(int64_t *num_arr, int64_t num, size_t len)
{
    if (!num_arr || !len)
    {
        return -1;
    }

    size_t front = 0;
    size_t end = len - 1;

    while (front <= end)
    {
        size_t mid = front + (end - front) / 2;

        if (num_arr[mid] == num)
        {
            return (int64_t)mid;
        }
        else if (num_arr[mid] > num)
        {
            if (mid == 0)
            {
                break;
            }
            end = mid - 1;
        }
        else
        {
            front = mid + 1;
        }
    }

    return -1;
}

int main()
{
    strix_t *input_strix = conv_file_to_strix("input.txt");
    if (!input_strix)
    {
        return EXIT_FAILURE;
    }

    strix_arr_t *lines = strix_split_by_delim(input_strix, '\n');
    if (!lines)
    {
        strix_free(input_strix);
        return EXIT_FAILURE;
    }

    strix_free(input_strix);

    size_t num_len = 0;

    for (size_t counter = 0; counter < lines->len; counter++)
    {
        strix_t *line = lines->strix_arr[counter];
        position_t *pos = strix_find_all(line, " ");
        if (!pos)
        {
            strix_free(input_strix);
            strix_free_strix_arr(lines);
            return EXIT_FAILURE;
        }

        num_len += (pos->len + 1);

        strix_position_free(pos);
    }

    int64_t *nums = (int64_t *)malloc(sizeof(int64_t) * num_len);
    if (!nums)
    {
        strix_free(input_strix);
        strix_free_strix_arr(lines);
        return EXIT_FAILURE;
    }

    size_t nums_written = 0;

    for (size_t counter = 0; counter < lines->len; counter++)
    {
        strix_t *line = lines->strix_arr[counter];
        strix_arr_t *num_arr = strix_split_by_delim(line, ' ');
        if (!num_arr)
        {
            strix_free(input_strix);
            strix_free_strix_arr(lines);
            free(nums);
            return EXIT_FAILURE;
        }

        for (size_t index = nums_written; index < num_arr->len + nums_written; index++)
        {
            // fprintf(stdout, STRIX_FORMAT"\n", STRIX_PRINT(num_arr->strix_arr[index - nums_written]));
            nums[index] = strix_to_signed_int(num_arr->strix_arr[index - nums_written]);
            // fprintf(stdout, "%ld\n", nums[index]);
        }

        nums_written += num_arr->len;
    }

    strix_free_strix_arr(lines);

    /* for (size_t counter = 0; counter < num_len; counter++)
    {
        fprintf(stdout, "%ld\n", nums[counter]);
    } */

    insertion_sort(nums, num_len);

    fprintf(stdout, "%ld\n", binary_search(nums, 234, num_len));

    free(nums);
    return EXIT_SUCCESS;
}