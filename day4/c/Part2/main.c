#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int find_XMAS(char **data, int num_rows, int num_cols) {
  int xmas = 0;

  for (int i = 1; i < num_rows - 1; i++) {
    for (int j = 1; j < num_cols - 1; j++) {

      if (data[i][j] == 'A') {

        if (data[i - 1][j - 1] == 'S' && data[i - 1][j + 1] == 'S' &&
            data[i + 1][j - 1] == 'M' && data[i + 1][j + 1] == 'M') {
          xmas++;
        }

        else if (data[i - 1][j - 1] == 'M' && data[i - 1][j + 1] == 'M' &&
                 data[i + 1][j - 1] == 'S' && data[i + 1][j + 1] == 'S') {
          xmas++;
        }

        else if (data[i - 1][j - 1] == 'M' && data[i - 1][j + 1] == 'S' &&
                 data[i + 1][j - 1] == 'M' && data[i + 1][j + 1] == 'S') {
          xmas++;
        }

        else if (data[i - 1][j - 1] == 'S' && data[i - 1][j + 1] == 'M' &&
                 data[i + 1][j - 1] == 'S' && data[i + 1][j + 1] == 'M') {
          xmas++;
        }
      }
    }
  }

  return xmas;
}

int main() {
  FILE *file = NULL;
  file = fopen("../../input.txt", "r");
  char line[2024];
  int num_rows = 0;
  int num_cols = 0;
  char **data = NULL;

  if (file != NULL) {
    while (fgets(line, sizeof(line), file)) {
      num_cols = strlen(line) - 1;
      data = (char **)realloc(data, (num_rows + 1) * sizeof(char *));
      data[num_rows] = strdup(line);
      num_rows++;
    }
    fclose(file);
  } else {
    printf("Error opening file.\n");
    return 1;
  }

  int result = find_XMAS(data, num_rows, num_cols);
  printf("%d\n", result);

  for (int i = 0; i < num_rows; i++) {
    free(data[i]);
  }
  free(data);

  return 0;
}
