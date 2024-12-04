#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define XMAS_SIZE 4
#define XMAS "XMAS"

typedef struct
{
    int dx;
    int dy;
} Direction;

Direction directions[8] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}

};

int find_XMAS(char **data, int num_rows, int num_cols)
{
    int xmas = 0;

    for (int i = 0; i < num_rows; i++)
    {
        for (int j = 0; j < num_cols; j++)
        {
            if (data[i][j] == 'X')
            {
                for (int d = 0; d < sizeof(directions) / sizeof(directions[0]); d++)
                {
                    int valid = 1;
                    for (int k = 1; k < XMAS_SIZE; k++)
                    {
                        int ni = i + k * directions[d].dx;
                        int nj = j + k * directions[d].dy;
                        if (ni < 0 || ni >= num_rows || nj < 0 || nj >= num_cols ||
                            data[ni][nj] != XMAS[k])
                        {
                            valid = 0;
                        }
                    }
                    if (valid)
                    {
                        xmas++;
                    }
                }
            }
        }
    }
    return xmas;
}

int main()
{
    FILE *file = NULL;
    file = fopen("../../input.txt", "r");
    char line[2024];
    int num_rows = 0;
    int num_cols = 0;
    char **data = NULL;

    if (file != NULL)
    {
        while (fgets(line, sizeof(line), file))
        {
            num_cols = strlen(line) - 1;
            data = (char **)realloc(data, (num_rows + 1) * sizeof(char *));
            data[num_rows] = strdup(line);
            num_rows++;
        }
        fclose(file);
    }
    else
    {
        printf("Error opening file.\n");
        return 1;
    }

    int result = find_XMAS(data, num_rows, num_cols);
    printf("%d\n", result);

    for (int i = 0; i < num_rows; i++)
    {
        free(data[i]);
    }
    free(data);

    return 0;
}
