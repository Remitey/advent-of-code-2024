#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TARGET '^'
#define WALL '#'
#define EMPTY '.'

typedef struct
{
    int dx, dy;
} directions;

directions Direction[4] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

typedef struct
{
    int x, y, direction;
} state;

void find_initial_position(char **map, int num_rows, int num_cols, int *x, int *y)
{
    for (int i = 0; i < num_rows; i++)
    {
        for (int j = 0; j < num_cols; j++)
        {
            if (map[i][j] == TARGET)
            {
                *x = i;
                *y = j;
                return;
            }
        }
    }
    printf("Guard not find\n");
}

bool guard_path(char **map, int num_rows, int num_cols, int start_x, int start_y, int obstacle_x, int obstacle_y)
{
    int x = start_x, y = start_y, direction = 0;
    state visited[10000];
    int visited_count = 0;

    char original = map[obstacle_x][obstacle_y];
    map[obstacle_x][obstacle_y] = WALL;

    while (true)
    {
        for (int i = 0; i < visited_count; i++)
        {
            if (visited[i].x == x && visited[i].y == y && visited[i].direction == direction)
            {
                map[obstacle_x][obstacle_y] = original;
                return true;
            }
        }

        visited[visited_count++] = (state){x, y, direction};

        int next_x = x + Direction[direction].dx;
        int next_y = y + Direction[direction].dy;

        if (next_x < 0 || next_x >= num_rows || next_y < 0 || next_y >= num_cols)
        {
            break;
        }

        if (map[next_x][next_y] == WALL)
        {
            direction = (direction + 1) % 4;
        }
        else
        {
            x = next_x;
            y = next_y;
        }
    }

    map[obstacle_x][obstacle_y] = original;
    return false;
}

int count_obstruction_positions(char **map, int num_rows, int num_cols)
{
    int start_x, start_y;
    find_initial_position(map, num_rows, num_cols, &start_x, &start_y);

    int count = 0;

    for (int i = 0; i < num_rows; i++)
    {
        for (int j = 0; j < num_cols; j++)
        {
            if (map[i][j] != EMPTY || (i == start_x && j == start_y))
            {
                continue;
            }

            if (guard_path(map, num_rows, num_cols, start_x, start_y, i, j))
            {
                count++;
            }
        }
    }

    return count;
}

int main()
{
    FILE *file = fopen("../input.txt", "r");
    if (!file)
    {
        printf("Error file\n");
        return 1;
    }

    char line[2024];
    int num_rows = 0;
    int num_cols = 0;
    char **map = NULL;

    while (fgets(line, sizeof(line), file))
    {
        line[strcspn(line, "\n")] = '\0';
        num_cols = strlen(line);
        map = (char **)realloc(map, (num_rows + 1) * sizeof(char *));
        map[num_rows] = strdup(line);
        num_rows++;
    }
    fclose(file);

    int result = count_obstruction_positions(map, num_rows, num_cols);
    printf("Circle : %d\n", result);

    for (int i = 0; i < num_rows; i++)
    {
        free(map[i]);
    }
    free(map);

    return 0;
}
