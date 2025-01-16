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


void oddEvenSort(int *vector, int size, int nThreads) {
    int chunkSize = size/100;
    for (int k = 0; k < size-1; k++)
    {
        if (k % 2 == 0) {
            #pragma omp parallel for schedule(static,chunkSize)
            for (int i = 0; i <= size-2; i += 2) {
                if (vector[i] > vector[i+1]) {
                    int temp = vector[i];
                    vector[i] = vector[i+1];
                    vector[i+1] = temp;
                }
            }
        } else {
            #pragma omp parallel for schedule(static,chunkSize)
            for (int i = 1; i <= size-2; i += 2) {
                if (vector[i] > vector[i+1]) {
                    int temp = vector[i];
                    vector[i] = vector[i+1];
                    vector[i+1] = temp;
                }
            }
        }

    }
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

    omp_set_num_threads(nThreads);

    double start = omp_get_wtime(); 
    
    //vector = paralelBubbleSort(vector,size,nThreads);
    oddEvenSort(vector,size,nThreads);

    double end = omp_get_wtime();

    double time_taken = end - start;

    printf("Tempo de %f segundos\n", time_taken);

    return 0;
}
