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
        strix_arr_t *num_arr = strix_split_by_delim(lines->strix_arr[counter], ' ');
        if (!num_arr)
        {
            strix_free_strix_arr(lines);
            return EXIT_FAILURE;
        }
        num_len += num_arr->len;
        strix_free_strix_arr(num_arr);
    }

    int64_t *nums = (int64_t *)malloc(sizeof(int64_t) * num_len);
    if (!nums)
    {
        strix_free_strix_arr(lines);
        return EXIT_FAILURE;
    }

    size_t nums_written = 0;
    for (size_t counter = 0; counter < lines->len; counter++)
    {
        strix_arr_t *num_arr_one = strix_split_by_delim(lines->strix_arr[counter], ' ');
        if (!num_arr_one)
        {
            free(nums);
            strix_free_strix_arr(lines);
            return EXIT_FAILURE;
        }

        for (size_t index = 0; index < num_arr_one->len; index++)
        {
            if (nums_written < num_len)
            {
                nums[nums_written++] = strix_to_signed_int(num_arr_one->strix_arr[index]);
            }
        }
        strix_free_strix_arr(num_arr_one);
    }

    strix_free_strix_arr(lines);

    insertion_sort(nums, num_len);

    for (size_t counter = 0; counter < num_len; counter++)
    {
        fprintf(stdout, "%ld\n", nums[counter]);
    }

    free(nums);
    return EXIT_SUCCESS;
}