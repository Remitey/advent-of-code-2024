#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TARGET '^'
#define WALL '#'
typedef struct {
  int dx;
  int dy;
} directions;

directions Direction[4] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

void find_initial_position(char **map, int num_rows, int num_cols, int *x,
                           int *y) {
  for (int i = 0; i < num_rows; i++) {
    for (int j = 0; j < num_cols; j++) {
      if (map[i][j] == TARGET) {
        *x = i;
        *y = j;
        return;
      }
    }
  }
  printf("Target not find");
}

int guard_path(char **map, int num_rows, int num_cols) {
  int x, y, next_x, next_y;
  int index_direction = 0;
  int add_guard_position = 0;
  int add_block = 0;

  bool **visited = (bool **)malloc(num_rows * sizeof(bool *));
  for (int i = 0; i < num_rows; i++) {
    visited[i] = (bool *)malloc(num_cols * sizeof(bool));
    for (int j = 0; j < num_cols; j++) {
      visited[i][j] = false;
    }
  }

  find_initial_position(map, num_rows, num_cols, &x, &y);
  visited[x][y] = true;
  add_guard_position++;

  while (true) {
    next_x = x + Direction[index_direction].dx;
    next_y = y + Direction[index_direction].dy;

    if (next_x < 0 || next_x >= num_rows || next_y < 0 || next_y >= num_cols) {
      break;
    }

    if (map[next_x][next_y] == WALL) {

      index_direction = (index_direction + 1) % 4;
      int x_1 = x + Direction[index_direction].dx;
      int y_1 = y + Direction[index_direction].dy;

      switch (index_direction) {
      case 1:
        for (int i = x; i < num_cols; i++) {

          x_1 = x_1 + Direction[index_direction].dx;
          y_1 = y_1 + Direction[index_direction].dy;

          if (map[x_1][y_1] == WALL) {

            int x_2 = x_1;
            int y_2 = y_1 - 1;
            for (int j = x_1; j < num_rows - 1; j++) {

              x_2 = x_2 + Direction[(index_direction + 1) % 4].dx;
              y_2 = y_2 + Direction[(index_direction + 1) % 4].dy;
              if (map[x_2][y_2] == WALL) {

                add_block++;
              }
            }
          }
        }
        break;
      case 2:
        for (int j = x_1; j < num_rows - 1; j++) {

          x_1 = x_1 + Direction[index_direction].dx;
          y_1 = y_1 + Direction[index_direction].dy;

          if (map[x_1][y_1] == WALL) {
            int x_2 = x_1 - 1;
            int y_2 = y_1;
            for (int i = x_1; i >= 0; i--) {
              x_2 = x_2 + Direction[(index_direction + 1) % 4].dx;
              y_2 = y_2 + Direction[(index_direction + 1) % 4].dy;
              if (map[x_2][y_2] == WALL) {
                add_block++;
              }
            }
          }
        }

        break;
      case 3:

        for (int i = x_1; i >= 0; i--) {

          x_1 = x_1 + Direction[index_direction].dx;
          y_1 = y_1 + Direction[index_direction].dy;

          if (map[x_1][y_1] == WALL) {
            int x_2 = x_1;
            int y_2 = y_1 + 1;
            for (int j = y_1; j >= 0; j--) {
              x_2 = x_2 + Direction[(index_direction + 1) % 4].dx;
              y_2 = y_2 + Direction[(index_direction + 1) % 4].dy;
              if (map[x_2][y_2] == WALL) {

                add_block++;
              }
            }
          }
        }
        break;
      case 0:
        for (int j = y_1; j >= 0; j--) {

          x_1 = x_1 + Direction[index_direction].dx;
          y_1 = y_1 + Direction[index_direction].dy;

          if (map[x_1][y_1] == WALL) {
            int x_2 = x_1 + 1;
            int y_2 = y_1;
            for (int i = x; i < num_cols; i++) {

              x_2 = x_2 + Direction[(index_direction + 1) % 4].dx;
              y_2 = y_2 + Direction[(index_direction + 1) % 4].dy;
              if (map[x_2][y_2] == WALL) {
                add_block++;
              }
            }
          }
        }
        break;
      default:
        break;
      }

    } else {

      x = next_x;
      y = next_y;

      if (!visited[x][y]) {
        visited[x][y] = true;
        add_guard_position++;
      }
    }

    // printf("Position actuelle : (%d, %d), direction : %d\n", x, y,
    //        index_direction);
  }
  printf("Circle :%d\n", add_block);

  for (int i = 0; i < num_rows; i++) {
    free(visited[i]);
  }
  free(visited);

  return add_guard_position;
}

int main() {
  FILE *file = NULL;
  file = fopen("../input.txt", "r");
  char line[2024];
  int num_rows = 0;
  int num_cols = 0;
  char **map = NULL;

  if (file != NULL) {
    while (fgets(line, sizeof(line), file)) {
      num_cols = strlen(line);
      map = (char **)realloc(map, (num_rows + 1) * sizeof(char *));
      map[num_rows] = strdup(line);
      num_rows++;
    }
    fclose(file);
  } else {
    printf("Error opening file.\n");
    return 1;
  }

  int result = guard_path(map, num_rows, num_cols);
  printf("Result : %d\n", result);

  for (int i = 0; i < num_rows; i++) {
    free(map[i]);
  }
  free(map);

  return 0;
}