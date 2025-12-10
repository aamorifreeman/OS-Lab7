#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX 20
#define THREAD_COUNT 10

int matA[MAX][MAX];
int matB[MAX][MAX];

int matSumResult[MAX][MAX];
int matDiffResult[MAX][MAX];
int matProductResult[MAX][MAX];

typedef struct {
    int startIndex;
    int endIndex;
} ThreadData;

// Each thread computes addition, subtraction, product for its chunk
void* computeAll(void* args) {
    ThreadData* data = (ThreadData*)args;

    for (int idx = data->startIndex; idx < data->endIndex; idx++) {
        int r = idx / MAX;
        int c = idx % MAX;

        matSumResult[r][c] = matA[r][c] + matB[r][c];
        matDiffResult[r][c] = matA[r][c] - matB[r][c];

        int sum = 0;
        for (int k = 0; k < MAX; k++) {
            sum += matA[r][k] * matB[k][c];
        }
        matProductResult[r][c] = sum;
    }

    return NULL;
}

void fillMatrix(int matrix[MAX][MAX]) {
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            matrix[i][j] = rand() % 10 + 1;
        }
    }
}

void printMatrix(int matrix[MAX][MAX]) {
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            printf("%5d", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
    srand(time(0));

    // 1. Fill input matrices
    fillMatrix(matA);
    fillMatrix(matB);

    printf("Matrix A:\n");
    printMatrix(matA);
    printf("Matrix B:\n");
    printMatrix(matB);

    pthread_t threads[THREAD_COUNT];
    ThreadData threadData[THREAD_COUNT];

    int totalCells = MAX * MAX;          // 400 cells
    int chunk = totalCells / THREAD_COUNT; // 40 cells per thread

    // Spawn 10 threads, each handling a chunk of cells
    for (int t = 0; t < THREAD_COUNT; t++) {
        threadData[t].startIndex = t * chunk;
        threadData[t].endIndex =
            (t == THREAD_COUNT - 1) ? totalCells : (t + 1) * chunk;

        pthread_create(&threads[t], NULL, computeAll, &threadData[t]);
    }

    // Join all threads
    for (int t = 0; t < THREAD_COUNT; t++) {
        pthread_join(threads[t], NULL);
    }

    // Print results
    printf("Results:\n");
    printf("Sum:\n");
    printMatrix(matSumResult);

    printf("Difference:\n");
    printMatrix(matDiffResult);

    printf("Product:\n");
    printMatrix(matProductResult);

    return 0;
}