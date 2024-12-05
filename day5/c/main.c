#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_SIZE 1000
#define NUMBERS_PAGES 99

void inverse_number(int **unvalid_update, int indiceline, int indice1,
                    int indice2)
{
  int temp = unvalid_update[indiceline][indice1];
  unvalid_update[indiceline][indice1] = unvalid_update[indiceline][indice2];
  unvalid_update[indiceline][indice2] = temp;
}

void check_before_after(int **update, int *row_sizes, int **rule_before,
                        int *sizes_before, int **rule_after, int *sizes_after,
                        int i, int *valid, int ***unvalid_update_ptr,
                        int **row_sizes_unvalid_update_ptr,
                        int *num_rows_unvalid_ptr, int part, int *z)
{
  int **unvalid_update = *unvalid_update_ptr;
  int *row_sizes_unvalid_update = *row_sizes_unvalid_update_ptr;
  int num_rows_unvalid = *num_rows_unvalid_ptr;

  for (int j = 0; j < row_sizes[i]; j++)
  {
    for (int k = 0; k < sizes_before[update[i][j]]; k++)
    {
      for (int v = row_sizes[i] - 1; v >= 0; v--)
      {
        if (rule_before[update[i][j]][k] == update[i][v] && j < v)
        {
          *valid = 0;
          if (part == 1)
          {
            unvalid_update =
                realloc(unvalid_update, (num_rows_unvalid + 1) * sizeof(int *));

            unvalid_update[num_rows_unvalid] = update[i];
            row_sizes_unvalid_update = realloc(
                row_sizes_unvalid_update, (num_rows_unvalid + 1) * sizeof(int));
            row_sizes_unvalid_update[num_rows_unvalid] = row_sizes[i];
            num_rows_unvalid++;
          }
          else if (part == 2)
          {
            *z = 0;
            inverse_number(unvalid_update, i, j, v);
          }
          break;
        }
      }
      if (*valid == 0)
        break;
    }
    if (*valid == 0)
      break;
  }
  for (int j = 0; j < row_sizes[i]; j++)
  {
    for (int k = 0; k < sizes_after[update[i][j]]; k++)
    {
      for (int v = 0; v < row_sizes[i]; v++)
      {
        if (rule_after[update[i][j]][k] == update[i][v] && j > v)
        {
          if (part == 1)
          {
            unvalid_update =
                realloc(unvalid_update, (num_rows_unvalid + 1) * sizeof(int *));

            unvalid_update[num_rows_unvalid] = update[i];
            row_sizes_unvalid_update = realloc(
                row_sizes_unvalid_update, (num_rows_unvalid + 1) * sizeof(int));
            row_sizes_unvalid_update[num_rows_unvalid] = row_sizes[i];
            num_rows_unvalid++;
          }
          else if (part == 2)
          {
            *z = 0;
            inverse_number(unvalid_update, i, j, v);
          }
          break;
        }
      }
      if (valid == 0)
        break;
    }
    if (*valid == 0)
    {
      if (part == 2)
      {
        *valid = 1;
      }
      break;
    }
  }

  *unvalid_update_ptr = unvalid_update;
  *row_sizes_unvalid_update_ptr = row_sizes_unvalid_update;
  *num_rows_unvalid_ptr = num_rows_unvalid;
}

int find_unvalid_update(int **unvalid_update, int *row_sizes_unvalid_update,
                        int num_rows_unvalid, int **rule_before,
                        int *sizes_before, int **rule_after, int *sizes_after)
{
  int valid = 1, mid_number = 0;

  for (int i = 0; i < num_rows_unvalid; i++)
  {
    valid = 1;
    for (int z = 0; z < 2; z++)
    {
      check_before_after(unvalid_update, row_sizes_unvalid_update, rule_before,
                         sizes_before, rule_after, sizes_after, i, &valid,
                         &unvalid_update, &row_sizes_unvalid_update,
                         &num_rows_unvalid, 2, &z);
    }

    if (valid == 1)
    {
      int mid_index = row_sizes_unvalid_update[i] / 2;
      mid_number += unvalid_update[i][mid_index];
    }
  }
  // error double free detected avec cette -->à investiguer dessus
  // for (int i = 0; i < num_rows_unvalid; i++) {
  //   free(unvalid_update[i]);
  // }
  free(unvalid_update);
  free(row_sizes_unvalid_update);
  return mid_number;
}

int find_valid_update(int **update, int *row_sizes, int num_rows,
                      int **rule_before, int *sizes_before, int **rule_after,
                      int *sizes_after)
{
  int add_mid_number = 0;
  int valid = 1;
  int **unvalid_update = NULL;
  int *row_sizes_unvalid_update = NULL;
  int num_rows_unvalid = 0;

  for (int i = 0; i < num_rows; i++)
  {
    valid = 1;

    check_before_after(update, row_sizes, rule_before, sizes_before, rule_after,
                       sizes_after, i, &valid, &unvalid_update,
                       &row_sizes_unvalid_update, &num_rows_unvalid, 1, 0);

    if (valid == 1)
    {
      int mid_index = row_sizes[i] / 2;
      add_mid_number += update[i][mid_index];
    }
  }
  int result_unvalid_update = find_unvalid_update(
      unvalid_update, row_sizes_unvalid_update, num_rows_unvalid, rule_before,
      sizes_before, rule_after, sizes_after);
  printf("Result unvalid: %d\n", result_unvalid_update);

  return add_mid_number;
}

int main(void)
{
  FILE *file = NULL;
  char line[MAX_LINE_SIZE];
  int **update = NULL;
  int **rule_before = (int **)calloc(NUMBERS_PAGES, sizeof(int *));
  int **rule_after = (int **)calloc(NUMBERS_PAGES, sizeof(int *));
  int sizes_before[NUMBERS_PAGES] = {0};
  int sizes_after[NUMBERS_PAGES] = {0};

  int first = 0, second = 0;
  int *row_sizes = NULL;
  int num_rows = 0;

  file = fopen("../input.txt", "r");
  if (file == NULL)
  {
    perror("Erreur lors de l'ouverture du fichier");
    return 0;
  }

  while (fgets(line, sizeof(line), file))
  {
    if (strcmp(line, "\n") == 0)
    {
      break;
    }
    if (sscanf(line, "%d|%d", &first, &second) == 2)
    {
      if (first < NUMBERS_PAGES)
      {
        rule_after[first] = (int *)realloc(
            rule_after[first], (sizes_after[first] + 1) * sizeof(int));
        rule_after[first][sizes_after[first]++] = second;
      }
      if (second < NUMBERS_PAGES)
      {
        rule_before[second] = (int *)realloc(
            rule_before[second], (sizes_before[second] + 1) * sizeof(int));
        rule_before[second][sizes_before[second]++] = first;
      }
    }
    else
    {
      fprintf(stderr, "Format invalide : %s", line);
    }
  }

  while (fgets(line, sizeof(line), file))
  {
    if (strcmp(line, "\n") == 0 || strcmp(line, "") == 0)
    {
      continue;
    }

    char *token = strtok(line, ",");
    int *row = NULL;
    int row_size = 0;

    while (token != NULL)
    {
      row = (int *)realloc(row, (row_size + 1) * sizeof(int));
      row[row_size++] = atoi(token);
      token = strtok(NULL, ",");
    }

    update = (int **)realloc(update, (num_rows + 1) * sizeof(int *));
    update[num_rows] = row;

    row_sizes = (int *)realloc(row_sizes, (num_rows + 1) * sizeof(int));
    row_sizes[num_rows++] = row_size;
  }
  fclose(file);

  int result = find_valid_update(update, row_sizes, num_rows, rule_before,
                                 sizes_before, rule_after, sizes_after);
  printf("Result : %d\n", result);

  for (int i = 0; i < NUMBERS_PAGES; i++)
  {
    free(rule_before[i]);
    free(rule_after[i]);
  }
  free(rule_before);
  free(rule_after);

  for (int i = 0; i < num_rows; i++)
  {
    free(update[i]);
  }
  free(update);
  free(row_sizes);

  return 0;
}