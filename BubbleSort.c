#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <omp.h>

int *generateVector(int size){
    int *vector = (int *)malloc(size * sizeof(int));

    srand(time(0));

    for (int i = 0; i < size; i++)
    {
        vector[i] = rand() % 100;
    }

    return vector;
}


void bubbleSort(int *vector, int size){
    int aux;

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size - 1; j++)
        {
            if (vector[j] > vector[j + 1])
            {
                aux = vector[j];
                vector[j] = vector[j + 1];
                vector[j + 1] = aux;
            }
        }
    }
}


int *paralelBubbleSort(int *vector, int size, int nThreads){
    int chunkSize = size / nThreads;

    int *returnVector = (int *)malloc(size * sizeof(int));

    #pragma omp parallel for schedule(static,chunkSize)
    for (int i = 0; i < nThreads; i++) {
        bubbleSort(&vector[i*chunkSize], chunkSize);
    }
    
    int currentMin = INT_MAX, pos = 0;

    int *indices = (int *)calloc(nThreads, sizeof(int));

    for (int i = 0; i < size; i++) {
        currentMin = INT_MAX;
        int minThread = -1;

        for (int j = 0; j < nThreads; j++) {
            int idx = j * chunkSize + indices[j];
            if (indices[j] < chunkSize && vector[idx] < currentMin) {
                currentMin = vector[idx];
                minThread = j;
            }
        }

        returnVector[i] = currentMin;
        indices[minThread]++;
    }

    free(indices);
    free(vector);

    return returnVector;
}


void printVector(int *vector, int size){
    for (int i = 0; i < size; i++)
    {
        printf("%d\n", vector[i]);
    }
}



int main(int argc, char const *argv[])
{
    int size = atoi(argv[1]);
    int nThreads = atoi(argv[2]);

    int *vector = generateVector(size);

    int start = clock();

    omp_set_num_threads(nThreads);
    
    vector = paralelBubbleSort(vector, size, nThreads);

    int end = clock();

    double time_taken = (double)(end - start) / (double)CLOCKS_PER_SEC;

    printf("Tempo de %f segundos\n", time_taken);

    return 0;
}
