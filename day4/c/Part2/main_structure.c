#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define A 'A'
#define M 'M'
#define S 'S'
#define NUMBER_DIRECTION 4

typedef struct {
  int dx;
  int dy;

} direction;

direction directions[4] = {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}};

int is_valid(int i, int j, int num_rows, int num_cols) {
    return i > 0 && i < num_rows - 1 && j > 0 && j < num_cols - 1;
}

int check_xmas(char **data, int i, int j) {
    int *ni = (int *)malloc(NUMBER_DIRECTION*sizeof(int));
    int *nj = (int *)malloc(NUMBER_DIRECTION*sizeof(int));

    
    for (int k = 0; k < NUMBER_DIRECTION; k++) {
  
        ni[k] = i + directions[k].dx;
        nj[k] = j + directions[k].dy;
    }

    if ((data[ni[0]][nj[0]] == M && data[ni[3]][nj[3]] == S) ||
        (data[ni[0]][nj[0]] == S && data[ni[3]][nj[3]] == M)) {
        if ((data[ni[1]][nj[1]] == M && data[ni[2]][nj[2]] == S) ||
            (data[ni[1]][nj[1]] == S && data[ni[2]][nj[2]] == M)) {
            return 1;  
        }
    }
    free(ni);
    free(nj);

    return 0;  
}

int find_XMAS(char **data, int num_rows, int num_cols) {
    int xmas = 0;

    for (int i = 1; i < num_rows - 1; i++) {
        for (int j = 1; j < num_cols - 1; j++) {
            if (data[i][j] == A && is_valid(i, j, num_rows, num_cols)) {
                xmas += check_xmas(data, i, j);
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
