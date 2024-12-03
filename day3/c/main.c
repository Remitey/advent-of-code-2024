#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *fopen(const char *FileName, const char *OpenMode);

int is_digit(char c) { return c >= '0' && c <= '9'; }

int take_number(const char **str) {
  int num = 0, count = 0;
  while (is_digit(**str) && count < 4) {
    num = num * 10 + (**str - '0');
    (*str)++;
    count++;
  }
  if (count == 0 || is_digit(**str)) {
    return -1;
  }
  return num;
}
int is_enable(const char *line, const char *current_ptr, int *enable) {
  const char *ptr = line;

  while (ptr < current_ptr) {
    const char *next_do = strstr(ptr, "do()");
    const char *next_dont = strstr(ptr, "don't()");

    if (next_do && next_do < current_ptr &&
        (!next_dont || next_do < next_dont)) {
      *enable = 1;
      ptr = next_do + 4;
    } else if (next_dont && next_dont < current_ptr) {
      *enable = 0;
      ptr = next_dont + 7;
    } else {
      break;
    }
  }

  return *enable;
}

int parse_and_calcul(const char *line, int *enable) {
  int score_mul_line = 0;
  const char *ptr = line;
  int enabled = *enable;
  int temp = 1;

  while (((ptr = strstr(ptr, "mul(")) != NULL)) {
    temp = is_enable(line, ptr, &enabled);
    printf("%d\n", temp);

    if (!temp) {
      ptr += 4;
      continue;
    }
    ptr += 4;
    int x = take_number(&ptr);

    if (x == -1 || *ptr != ',') {
      continue;
    }
    ptr++;

    int y = take_number(&ptr);
    if (y == -1 || *ptr != ')') {
      continue;
    }
    ptr++;
    score_mul_line += x * y;
  }
  *enable = temp;
  return score_mul_line;
}

int main(void) {

  FILE *file = NULL;
  char line[20000];
  int result = 0;
  int enable = 1;

  file = fopen("../input.txt", "r");
  if (file != NULL) {
    while (fgets(line, sizeof(line), file)) {
      result += parse_and_calcul(line, &enable);
    }
    fclose(file);
  } else {
    printf("error file");
  }
  printf("Result : %d\n ", result);
  return 0;
}
