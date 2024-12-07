#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"

#define MAX_LINE_SIZE 1000

unsigned long long concatenate(unsigned long long a, unsigned long long b) {
    unsigned long long power = 1;
    while (power <= b) {
        power *= 10;
    }
    return a * power + b;
}

unsigned long long evaluate(unsigned long long *operands, char *operators, int num_operators) {
    unsigned long long result = operands[0];
    for (int i = 0; i < num_operators; i++) {
        if (operators[i] == '+') {
            result += operands[i + 1];
        } else if (operators[i] == '*') {
            result *= operands[i + 1];
        } else if (operators[i] == '|') {  
            result = concatenate(result, operands[i + 1]);
        }
    }
    return result;
}

unsigned long long find_true_test(unsigned long long **calcul, int *row_sizes, int num_rows) {
    unsigned long long true_test = 0;

    for (int i = 0; i < num_rows; i++) {
        unsigned long long target = calcul[i][0];  
        unsigned long long *operands = &calcul[i][1];
        int num_operands = row_sizes[i] - 1;

        int num_operators = num_operands - 1;
        int total_combinations = pow(3, num_operators); 
        int valid = 0;

        for (int comb = 0; comb < total_combinations; comb++) {
            char operators[num_operators];
            int temp = comb;
            for (int j = 0; j < num_operators; j++) {
                int op = temp % 3;
                if (op == 0) {
                    operators[j] = '+';
                } else if (op == 1) {
                    operators[j] = '*';
                } else {
                    operators[j] = '|'; 
                }
                temp /= 3;
            }

            unsigned long long result = evaluate(operands, operators, num_operators);
            if (result == target) {
                valid = 1;
                break; 
            }
        }

        if (valid) {
            true_test += target;
        }
    }

    return true_test;
}

int main() {
    FILE *file = NULL;
    char line[MAX_LINE_SIZE];
    unsigned long long **calcul = NULL;

    int *row_sizes = NULL;
    int num_rows = 0;

    file = fopen("../input.txt", "r");
    if (file == NULL) {
        perror("Error file");
        return 0;
    }

    while (fgets(line, sizeof(line), file)) {
        if (strcmp(line, "\n") == 0 || strcmp(line, "") == 0) {
            continue;
        }

        char *token = strtok(line, " ");
        unsigned long long *row = NULL;
        int row_size = 0;

        while (token != NULL) {
            unsigned long long value = strtoull(token, NULL, 10);
            row = (unsigned long long *)realloc(row, (row_size + 1) * sizeof(unsigned long long));
            row[row_size++] = value;
            token = strtok(NULL, " ");
        }

        calcul = (unsigned long long **)realloc(calcul, (num_rows + 1) * sizeof(unsigned long long *));
        calcul[num_rows] = row;

        row_sizes = (int *)realloc(row_sizes, (num_rows + 1) * sizeof(int));
        row_sizes[num_rows++] = row_size;
    }
    fclose(file);

    unsigned long long result = find_true_test(calcul, row_sizes, num_rows);

    // for (int i = 0; i < num_rows; i++) {
    //     for (int j = 0; j < row_sizes[i]; j++) {
    //         printf("%llu ", calcul[i][j]);
    //     }
    //     printf("\n");
    // }
    printf("Result is %llu\n", result);

    for (int i = 0; i < num_rows; i++) {
        free(calcul[i]);
    }
    free(calcul);
    free(row_sizes);
}