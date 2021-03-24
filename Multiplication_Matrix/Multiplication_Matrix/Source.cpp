#include <iostream>
#include <omp.h>
#include <time.h>


using namespace std;
#define SIZE 50

void stat()
{
    srand(time(NULL));


    int matrix1[SIZE][SIZE];
   
    int matrix2[SIZE][SIZE];

   
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            matrix1[i][j] = 1;

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            matrix2[i][j] = 2;

    int result[SIZE][SIZE];

    double t1Linear = omp_get_wtime();
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            result[i][j] = 0;
            for (int k = 0; k < SIZE; k++) {
                result[i][j] += (matrix1[i][k] * matrix2[k][j]);
            }
        }
    }
    double t2Linear = omp_get_wtime();
    cout << t2Linear - t1Linear << " - TIME of LINEAR";

    
    int result_parallel[SIZE][SIZE];

    //Устанавливаем число потоков
    int threadsNum = 8;
    omp_set_num_threads(threadsNum);
    int i, j, k;
    double t1 = omp_get_wtime();
#pragma omp parallel for shared(matrix1, matrix2, result_parallel) private(i, j, k)
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            result_parallel[i][j] = 0;
            for (k = 0; k < SIZE; k++) {
                result_parallel[i][j] += (matrix1[i][k] * matrix2[k][j]);
            }
        }
    }
    double t2 = omp_get_wtime();
    cout << endl << t2 - t1 << " - TIME of PARALLEL\n";

    system("pause");
}