#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_SIZE 1000

FILE *fopen(const char *FileName, const char *OpenMode);

int is_safe(int *line, int size) {
  int direction = 0;
  for (int j = 0; j < size - 1; j++) {
    int diff = line[j + 1] - line[j];

    if (diff != 1 && diff != 2 && diff != 3 && diff != -1 && diff != -2 &&
        diff != -3) {
      return 0;
    }

    if (direction == 0) {
      direction = (diff > 0) ? 1 : (diff < 0) ? -1 : 0;
    } else if ((direction == 1 && diff < 0) || (direction == -1 && diff > 0)) {
      return 0;
    }
  }
  return 1;
}

int safe_line(int **table, int *size, int num_rows) {
  int i = 0, j = 0, safe_line = 0;
  for (i = 0; i < num_rows; i++) {
    if (is_safe(table[i], size[i])) {
      safe_line++;
    } else {
      int safe = 0;
      for (j = 0; j < size[i]; j++) {
        int *modified_line = malloc((size[i]) * sizeof(int));
        int a = 0;

        for (int b = 0; b < size[i]; b++) {
          if (b != j) {
            modified_line[a++] = table[i][b];
          }
        }
        if (is_safe(modified_line, size[i] - 1)) {
          safe = 1;
          free(modified_line);
          break;
        }

        free(modified_line);
      }
      if (safe) {
        safe_line++;
      }
    }
  }
  return safe_line;
}

int main(void) {

  FILE *file = NULL;
  int i = 0;

  char line[MAX_LINE_SIZE];
  int **table = NULL;
  int *size = NULL;
  int num_rows = 0;

  file = fopen("../input.txt", "r");
  if (file != NULL) {
    while (fgets(line, sizeof(line), file)) {
      int count = 0;
      char *temp = strdup(line);
      char *token = strtok(temp, " ");
      while (token != NULL) {
        count++;
        token = strtok(NULL, " ");
      }
      free(temp);

      size = (int *)realloc(size, (num_rows + 1) * sizeof(int));
      table = (int **)realloc(table, (num_rows + 1) * sizeof(int *));

      if (size == NULL || table == NULL) {
        printf("Error memory allocation");
        return 1;
      }

      size[num_rows] = count;
      table[num_rows] = (int *)malloc(count * sizeof(int));
      if (table[num_rows] == NULL) {
        printf("Error memory allocation");
        return 1;
      }
      int i = 0;
      token = strtok(line, " ");
      while (token != NULL) {
        table[num_rows][i++] = atoi(token);
        token = strtok(NULL, " ");
      }
      num_rows++;
    }
    fclose(file);
  } else {
    printf("error file");
  }

  int safe_line_total = safe_line(table, size, num_rows);
  printf("%d\n", safe_line_total);

  for (int i = 0; i < num_rows; i++) {
    free(table[i]);
  }
  free(table);
  free(size);

  return 0;
}
