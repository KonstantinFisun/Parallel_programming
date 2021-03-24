#include <iostream>
#include <omp.h>
#include <time.h>

using namespace std;

#define SIZE 1000

void FillMatrix(int** matrix, int n, int m,int l) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            matrix[i][j] = l;
    return;
}


int** matrixMulti(int** matrix1, int n1, int m1, int** matrix2, int n2, int m2) {
    
    //Выделяем память под результат умножения
    int** result;
    result = (int**)malloc(sizeof(int*) * n1);
    for (int i = 0; i < n1; i++) {
        result[i] = (int*)malloc(sizeof(int) * m2);
    }

    //Умножение по формуле
    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < m2; j++) {
            result[i][j] = 0;
            for (int k = 0; k < m1; k++) {
                result[i][j] += (matrix1[i][k] * matrix2[k][j]);
            }
        }
    }

    return result;
}


void main()
{

    setlocale(LC_ALL, "Russian");
    int n1 = SIZE;
    int m1 = SIZE;
    int n2 = SIZE;
    int m2 = SIZE;

    //Матрица n1 x m1
    int** matrix1;
    //Матрица n2 x m2
    int** matrix2;

    matrix1 = (int**)malloc(sizeof(int*) * n1);
    for (int i = 0; i < n1; i++) {
        matrix1[i] = (int*)malloc(sizeof(int) * m1);
    }
    matrix2 = (int**)malloc(sizeof(int*) * n2);
    for (int i = 0; i < n2; i++) {
        matrix2[i] = (int*)malloc(sizeof(int) * m2);
    }

    
    //Генерируем  матрицы для умножения
    int l1 = 3, l2 = 2;
    FillMatrix(matrix1, n1, m1,l1);
    FillMatrix(matrix2, n2, m2,l2);


    
    int** resultLinear = (int**)malloc(sizeof(int*) * n1);;
    for (int i = 0; i < n1; i++) {
        resultLinear[i] = (int*)malloc(sizeof(int) * m2);
    }

    double t1Linear = omp_get_wtime();
    resultLinear = matrixMulti(matrix1, n1, m1, matrix2, n2, m2);
    double t2Linear = omp_get_wtime();
    cout << t2Linear - t1Linear << " - Время линейного";

    int** resultParallel = (int**)malloc(sizeof(int*) * n1);;
    for (int i = 0; i < n1; i++) {
        resultParallel[i] = (int*)malloc(sizeof(int) * m2);
    }

    
    //Устанавливаем число потоков
    //int threadsNum = 8;
    //omp_set_num_threads(threadsNum);
    int i, j, k;
    double t1 = omp_get_wtime();
#pragma omp parallel for shared(matrix1, matrix2, resultParallel) private(i, j, k)
    for (i = 0; i < n1; i++) {
        for (j = 0; j < m2; j++) {
            resultParallel[i][j] = 0;
            for (k = 0; k < m1; k++) {
                resultParallel[i][j] += (matrix1[i][k] * matrix2[k][j]);
            }
        }
    }
    double t2 = omp_get_wtime();
    cout << endl << t2 - t1 << " - время параллельного\n";


    int** resultlentochka = (int**)malloc(sizeof(int*) * n1);;
    for (int i = 0; i < n1; i++) {
        resultlentochka[i] = (int*)malloc(sizeof(int) * m2);
    }
    

    t1 = omp_get_wtime();
    int threads1Num = 8;
    int m = SIZE / threads1Num;
    omp_set_num_threads(threads1Num);
#pragma omp parallel shared(matrix1,matrix2,resultlentochka,m) private(i,j,k)
    {
        int numThread = omp_get_thread_num();
        for (i = 0; i < m; i++) {
            for (j = 0; j < SIZE; j++) {
                resultlentochka[i + (numThread * m)][j] = 0;
                for (k = 0; k < SIZE; k++) {
                    resultlentochka[i+(numThread*m)][j] += (matrix1[i+(numThread*m)][k] * matrix2[k][j]);
                }                              
            }
            
        }
    }
    t2 = omp_get_wtime();
    cout << endl << t2 - t1 << " - время ленточного\n";


    //Проверка на совпадение значений
    for (i = 0; i < SIZE; i++) 
        for (j = 0; j < SIZE; j++) {
            if (resultlentochka[i][j] != resultLinear[i][j]) {
                cout << "Значение не совпадает" << endl;
                break;
            }
            
        }
    
    
    
   
    



    system("pause");
}