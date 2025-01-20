#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <omp.h>

void printVector(int *vector, int size){
    for (int i = 0; i < size; i++)
    {
        printf("%d ", vector[i]);
    }
}

int *generateVector(int size){
    int *vector = (int *)malloc(size * sizeof(int));

    srand(time(0));

    for (int i = 0; i < size; i++)
    {
        vector[i] = rand() % 100;
    }

    return vector;
}

void oddEvenSort(int *vector, int size, int nThreads) {
    int chunkSize = (size / 100 < 1) ? 1 : size / 100;

    int changedInEven = 0, changedInOdd = 0;

    do {
        changedInEven = 0;
        changedInOdd = 0;

        #pragma omp parallel for reduction(||:changedInEven) schedule(static,chunkSize)
        for (int i = 0; i <= size - 2; i += 2) {
            if (vector[i] > vector[i + 1]) {
                int temp = vector[i];
                vector[i] = vector[i + 1];
                vector[i + 1] = temp;
                changedInEven = 1;
            } 
        }

        #pragma omp parallel for reduction(||:changedInOdd) schedule(static,chunkSize)
        for (int i = 1; i <= size - 2; i += 2) {
            if (vector[i] > vector[i + 1]) {
                int temp = vector[i];
                vector[i] = vector[i + 1];
                vector[i + 1] = temp;
                changedInOdd = 1;
            }
        }

    } while (changedInEven || changedInOdd);
}


int main(int argc, char const *argv[])
{
    int size = atoi(argv[1]);
    int nThreads = atoi(argv[2]);

    for (int i = 0; i < 1; i++){
        int *vector = generateVector(size);

        omp_set_num_threads(nThreads);

        double start = omp_get_wtime(); 
        
        oddEvenSort(vector,size,nThreads);

        double end = omp_get_wtime();

        double time_taken = end - start;

        printf("%.3f\n", time_taken);
    }

    return 0;
}
