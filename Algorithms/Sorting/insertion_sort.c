#include <stdio.h>
#include <stdlib.h>
#include "../../Strix/header/strix.h"

int main()
{
    strix_t *input_strix = conv_file_to_strix("input.txt");
    if (!input_strix)
    {
        return EXIT_FAILURE;
    }

    strix_arr_t *lines = strix_split_by_delim(input_strix, "\n");
    if (!lines)
    {
        strix_free(input_strix);
        return EXIT_FAILURE;
    }

    size_t num_len = 0;

    for (size_t counter = 0; counter < lines->len; counter++)
    {
        strix_t *line = lines->strix_arr[counter];
        position_t *pos = strix_find_all(line, " ");
        if (!pos)
        {
            strix_free(input_strix);
            strix_free(lines);
            return EXIT_FAILURE;
        }

        num_len += (pos->len + 1);
    }

    int64_t *nums = (int64_t *)malloc(sizeof(int64_t) * lines->len);
    if (!nums)
    {
        strix_free(input_strix);
        strix_free_strix_arr(input_strix);
        return EXIT_FAILURE;
    }
}