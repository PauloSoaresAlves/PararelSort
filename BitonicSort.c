#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SMALL 65536
#define MEDIUM 131072
#define LARGE 524288

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

    bubbleSort(vector, size);

    printVector(vector, size);

    return 0;
}
