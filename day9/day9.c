#include <stdio.h>
#include <stdlib.h>

void compact_disk(int *block_disk, int size) {

  for ( int i = size - 1; i >= 0; i--) {
    if (block_disk[i] != 0) {
       int target = -1;

      for (int j = 0; j < i; j++) {
        if (block_disk[j] == 0) {
          target = j;
          break;
        }
      }

      if (target != -1) {
        block_disk[target] = block_disk[i];
        block_disk[i] = 0;
      }
    }
  }
}

long long int calculate_checksum( int *block_disk, int size) {
  long long int checksum = 0;
  for (int i = 0; i < size; i++) {
    if (block_disk[i] != 0) {
      checksum += i * (block_disk[i] - 1);
    }
  }
  return checksum;
}

void sort_disk(int *disk_map,int number) {
  int ID = 1;
  int sizeofblockdisk = 0;
  int *block_disk = NULL;

  for (int i = 0; i < number; i++) {
    block_disk = (int *)realloc(block_disk,
                                (sizeofblockdisk + disk_map[i]) * sizeof(int));

    for ( int j = 0; j < disk_map[i]; j++) {
      if (i % 2 == 0) {
        block_disk[sizeofblockdisk + j] = ID;
      } else {
        block_disk[sizeofblockdisk + j] = 0;
      }
    }

    if (i % 2 == 0) {
      ID++;
    }

    sizeofblockdisk += disk_map[i];
  }

  compact_disk(block_disk, sizeofblockdisk);

    // for (long long int i = 0; i < sizeofblockdisk; i++) {
    //   printf("%d ", block_disk[i]);
    // }
    // printf("\n");

long long int checksum =
      calculate_checksum(block_disk, sizeofblockdisk);
  printf("Checksum  : %lld\n", checksum);

  free(block_disk);
}

int main(int argc, char *argv[]) {
  FILE *file = NULL;
  int *disk_map = NULL;
  int number = 0;
  int temp_value = 0;

  file = fopen("input.txt", "r");
  if (file == NULL) {
    printf("Error\n");
    return 1;
  }

  while ((temp_value = fgetc(file)) != EOF) {
    if (temp_value >= '0' && temp_value <= '9') {
      disk_map = ( int *)realloc(disk_map, (number + 1) * sizeof(int));
      if (disk_map == NULL) {
        printf("Erreur de réallocation\n");
        fclose(file);
        return 1;
      }
      disk_map[number] = temp_value - '0';
      number++;
    }
  }

  fclose(file);
  sort_disk(disk_map, number);

  free(disk_map);

  return 0;
}