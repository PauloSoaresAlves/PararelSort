#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define SMALL 65536
#define MEDIUM 131072
#define LARGE 524288

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void bitonicMerge(int arr[], int start, int size, bool ascending) {
    if (size < 2) {
        return;
    }

    int mid = size / 2;

    #pragma omp parallel for
    for (int i = start; i < start + mid; i++) {
        bool condSwap = (arr[i] > arr[i + mid]);
        if (ascending == condSwap) {
            swap(&arr[i], &arr[i + mid]);
        }
    }

    bitonicMerge(arr, start,       mid, ascending);
    bitonicMerge(arr, start + mid, mid, ascending);
}

void bitonicSortRecursive(int arr[], int start, int size, bool ascending) {
    if (size < 2) {
        return;
    }
    
    int mid = size / 2;

    // recursão asc / desc em paralelo
    #pragma omp task
    {
        bitonicSortRecursive(arr, start, mid, true);
    }
    #pragma omp task
    {
        bitonicSortRecursive(arr, start + mid, mid, false);
    }

    #pragma omp taskwait

    bitonicMerge(arr, start, size, ascending);
}

void bitonicSort(int arr[], int n, bool direction) {
    bitonicSortRecursive(arr, 0, n, direction);
}

bool is_poten(int n) {
    return (n & (n - 1)) == 0;
}

int proxima_poten(int n) {
    if (is_poten(n)) {
        return n;
    }
    int power = 1;
    while (power < n) {
        power <<= 1;
    }
    return power;
}

int *generateVector(int size) {
    int newSize = proxima_poten(size);
    int *vector = (int *)malloc(newSize * sizeof(int));
    srand(time(0));
    for (int i = 0; i < size; i++) {
        vector[i] = rand() % 100;
    }
    for (int i = size; i < newSize; i++) {
        vector[i] = INT_MAX;
    }
    return vector;
}

void printVector(int *vector, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", vector[i]);
    }
    printf("\n");
}

int main(int argc, char const *argv[]) {
    if (argc < 3) {
        printf("Uso: %s <tamanho> <nThreads>\n", argv[0]);
        return 1;
    }

    int size = atoi(argv[1]);
    int nThreads = atoi(argv[2]);

    int *vector = generateVector(size);
    
    omp_set_num_threads(nThreads);

    double start = omp_get_wtime();
    // Cria a região paralela; dentro dela, apenas 1 thread inicia a recursão
    #pragma omp parallel
    {
        #pragma omp single
        {
            bitonicSortRecursive(vector, 0, size, true);
        }
    }

    double end = omp_get_wtime();
    double time_taken = end - start;

    printf("Tempo: %f segundos\n", time_taken);

    free(vector);
    return 0;
}
