#include <stdio.h>
#include <stdlib.h>

#define ROWS 2
#define COLS 1000

FILE *fopen(const char *FileName, const char *OpenMode);

void SortValue(int input[ROWS][COLS]) {
  int i, j, c, v;
  for (v = 0; v < 2; v++) {
    for (i = 0; i < COLS - 1; i++) {
      for (j = i + 1; j < COLS; j++) {
        if (input[v][i] > input[v][j]) {
          c = input[v][i];
          input[v][i] = input[v][j];
          input[v][j] = c;
        }
      }
    }
  }
}

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

int DistanceCalulation(int input[ROWS][COLS]) {
  int i = 0,totaldistance = 0;
  for (i = 0; i < COLS; i++) {
    totaldistance += abs(input[0][i] - input[1][i]);
  }
  return totaldistance;
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

  SortValue(input);
  int total = DistanceCalulation(input);
  printf("Total Distance = %d\n", total);
  
  int similarity = SimilarityValue(input);
  printf("Total Similarity = %d\n", similarity);
  return 0;
}
