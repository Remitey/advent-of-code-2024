#include <stdio.h>
#include <stdlib.h>

#define ROWS 2
#define COLS 1000

FILE *fopen(const char *FileName, const char *OpenMode);

int SimilarityValue(int input[ROWS][COLS]) {
  int i, j, counter = 0, totalsimilarty = 0;
  for (i = 0; i < COLS; i++) {
    for (j = 0; j < COLS; j++) {
      if (input[0][i] == input[1][j]) {
        counter++;
      }
    }
    totalsimilarty += input[0][i]*counter;
    counter =0;
  }
  return totalsimilarty;
}


int main(void) {

  FILE *file = NULL;
  int input[ROWS][COLS] = {0};
  int i = 0;

  file = fopen("../input.txt", "r");
  if (file != NULL) {
    while (i < COLS && fscanf(file, "%d %d", &input[0][i], &input[1][i]) == 2) {
      i++;
    }
    fclose(file);
  } else {
    printf("error");
  }

  int similarity = SimilarityValue(input);
  printf("Result = %d\n", similarity);

  return 0;
}
