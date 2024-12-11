#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    int x, y;
} Position;

bool position_existe(Position *positions, int taille, Position p) {
    for (int i = 0; i < taille; i++) {
        if (positions[i].x == p.x && positions[i].y == p.y) {
            return true;
        }
    }
    return false;
}

int calcul_antinode_part1(char **map, int num_rows, int num_cols) {
    Position *antinodes = NULL;
    int antinode_count = 0;

    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            if (map[i][j] != '.') { 
                char freq = map[i][j];

                for (int x = 0; x < num_rows; x++) {
                    for (int y = 0; y < num_cols; y++) {
                        if ((x != i || y != j) && map[x][y] == freq) {
                            int dx = x - i;
                            int dy = y - j;

                            Position p1 = {i - dx, j - dy}; 
                            Position p2 = {x + dx, y + dy}; 

                            if (p1.x >= 0 && p1.x < num_rows && p1.y >= 0 && p1.y < num_cols) {
                                if (!position_existe(antinodes, antinode_count, p1)) {
                                    antinodes = realloc(antinodes, (antinode_count + 1) * sizeof(Position));
                                    antinodes[antinode_count++] = p1;
                                }
                            }

                            if (p2.x >= 0 && p2.x < num_rows && p2.y >= 0 && p2.y < num_cols) {
                                if (!position_existe(antinodes, antinode_count, p2)) {
                                    antinodes = realloc(antinodes, (antinode_count + 1) * sizeof(Position));
                                    antinodes[antinode_count++] = p2;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    free(antinodes);

    return antinode_count;
}

int calcul_antinode_part2(char **map, int num_rows, int num_cols) {
    Position *antinodes = NULL;
    int antinode_count = 0;

    char frequencies[62] = {0}; 
    int freq_count[62] = {0};   
    Position **antenne_positions = malloc(62 * sizeof(Position *));
    for (int i = 0; i < 62; i++) {
        antenne_positions[i] = NULL;
    }

    int freq_index = 0;
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            if (map[i][j] != '.') {
                char freq = map[i][j];
                int index = -1;

                for (int k = 0; k < freq_index; k++) {
                    if (frequencies[k] == freq) {
                        index = k;
                        break;
                    }
                }
                if (index == -1) {
                    index = freq_index;
                    frequencies[freq_index++] = freq;
                }

                freq_count[index]++;
                antenne_positions[index] = realloc(antenne_positions[index], freq_count[index] * sizeof(Position));
                antenne_positions[index][freq_count[index] - 1] = (Position){i, j};
            }
        }
    }

    for (int f = 0; f < freq_index; f++) {
        if (freq_count[f] < 2) {
            continue;
        }

        Position *positions = antenne_positions[f];
        int count = freq_count[f];

        for (int i = 0; i < count; i++) {
            for (int j = i + 1; j < count; j++) {
                int dx = positions[j].x - positions[i].x;
                int dy = positions[j].y - positions[i].y;

                for (int k = -1; k <= 1; k += 2) {
                    int x = positions[i].x + k * dx;
                    int y = positions[i].y + k * dy;
                    while (x >= 0 && x < num_rows && y >= 0 && y < num_cols) {
                        Position p = {x, y};
                        if (!position_existe(antinodes, antinode_count, p)) {
                            antinodes = realloc(antinodes, (antinode_count + 1) * sizeof(Position));
                            antinodes[antinode_count++] = p;
                        }
                        x += k * dx;
                        y += k * dy;
                    }
                }
            }

            if (!position_existe(antinodes, antinode_count, positions[i])) {
                antinodes = realloc(antinodes, (antinode_count + 1) * sizeof(Position));
                antinodes[antinode_count++] = positions[i];
            }
        }
    }

    for (int i = 0; i < 62; i++) {
        free(antenne_positions[i]);
    }
    free(antenne_positions);
    free(antinodes);

    return antinode_count;
}

int main() {
    FILE *file = fopen("../input.txt", "r");
    if (!file) {
        printf("Error file\n");
        return 1;
    }

    char line[2024];
    int num_rows = 0;
    int num_cols = 0;
    char **map = NULL;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';
        num_cols = strlen(line);
        map = (char **)realloc(map, (num_rows + 1) * sizeof(char *));
        map[num_rows] = strdup(line);
        num_rows++;
    }
    fclose(file);

    int result_part1 = calcul_antinode_part1(map, num_rows, num_cols);
    int result_part2 = calcul_antinode_part2(map, num_rows, num_cols);

    printf("Result part 1: %d\n", result_part1);
    printf("Result part 2: %d\n", result_part2);

    for (int i = 0; i < num_rows; i++) {
        free(map[i]);
    }
    free(map);
    return 0;
}
