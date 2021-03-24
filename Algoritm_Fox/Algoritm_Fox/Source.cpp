#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <time.h>
#define SIZE 1000

using namespace std;

void matrixmult(int** col1, int m1_row, int m1_col, int** col2, int m2_row, int m2_col, int** m3, int m3_row, int m3_col);

void matrixfill(int** m, int row, int col, int l)
{
    for (int i = 0; i < row; ++i)
        for (int j = 0; j < col; ++j){
            m[i][j]=l;
        }
}
//Вывод матриц
void print_matr(int** m, int row, int col)
{
    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < col; ++j)
            cout << setw(3) << m[i][j] << ' ';

        cout << endl;
    }
}
//Линейный алгоритм
int** matrixMulti(int** arr1, int row1, int col1, int** arr2, int row2, int col2) {

    //Выделяем память под результат умножения
    int** result;
    result = (int**)malloc(sizeof(int*) * row1);
    for (int i = 0; i < row1; i++) {
        result[i] = (int*)malloc(sizeof(int) * col2);
    }

    //Умножение по формуле
    for (int i = 0; i < row1; i++) {
        for (int j = 0; j < col2; j++) {
            result[i][j] = 0;
            for (int k = 0; k < col1; k++) {
                result[i][j] += (arr1[i][k] * arr2[k][j]);
            }
        }
    }

    return result;
}

int main()
{
    setlocale(LC_ALL, "Russian");

    int row1 = SIZE;
    int col1 = SIZE;

    clock_t start, stop;

   

    int** arr1 = new int* [row1];

    for (int i = 0; i < row1; ++i)
        arr1[i] = new int[col1];


    matrixfill(arr1, row1, col1,1);

    int row2 = SIZE;
    int col2 = SIZE;


    int** arr2 = new int* [row2];
    for (int i = 0; i < row2; ++i)
        arr2[i] = new int[col2];

    matrixfill(arr2, row2, col2,2);

    int** resultLinear = (int**)malloc(sizeof(int*) * row1);;
    for (int i = 0; i < row1; i++) {
        resultLinear[i] = (int*)malloc(sizeof(int) * col2);
    }

    double t1Linear = omp_get_wtime();
    resultLinear = matrixMulti(arr1, row1, col1, arr2, row2, col2);
    double t2Linear = omp_get_wtime();
    cout << t2Linear - t1Linear << " - Время линейного";

    int** resultParallel = (int**)malloc(sizeof(int*) * row1);;
    for (int i = 0; i < row1; i++) {
        resultParallel[i] = (int*)malloc(sizeof(int) * col2);
    }


    //Устанавливаем число потоков
    //int threadsNum = 8;
    //omp_set_num_threads(threadsNum);
    int i, j, k;
    double t1 = omp_get_wtime();
#pragma omp parallel for shared(arr1, arr2, resultParallel) private(i, j, k)
    for (i = 0; i < row1; i++) {
        for (j = 0; j < col2; j++) {
            resultParallel[i][j] = 0;
            for (k = 0; k < col1; k++) {
                resultParallel[i][j] += (arr1[i][k] * arr2[k][j]);
            }
        }
    }
    double t2 = omp_get_wtime();
    cout << endl << t2 - t1 << " - время параллельного\n";


    int** resultlentochka = (int**)malloc(sizeof(int*) * row1);;
    for (int i = 0; i < row1; i++) {
        resultlentochka[i] = (int*)malloc(sizeof(int) * col2);
    }


    t1 = omp_get_wtime();
    int threads1Num = 4;
    int m = SIZE / threads1Num;
    omp_set_num_threads(threads1Num);
#pragma omp parallel shared(arr1,arr2,resultlentochka,m) private(i,j,k)
    {
        int numThread = omp_get_thread_num();
        for (i = 0; i < m; i++) {
            for (j = 0; j < SIZE; j++) {
                resultlentochka[i + (numThread * m)][j] = 0;
                for (k = 0; k < SIZE; k++) {
                    resultlentochka[i + (numThread * m)][j] += (arr1[i + (numThread * m)][k] * arr2[k][j]);
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


    int row3 = row1;
    int col3 = col2;

    int** arr3 = new int* [row3];
    for (int i = 0; i < row3; ++i)
        arr3[i] = new int[col3];

    for (int i = 0; i < row3; ++i)
        for (int j = 0; j < col3; ++j)
            arr3[i][j] = 0;

    t1 = omp_get_wtime();

    
    matrixmult(arr1, row1, col1, arr2, row2, col2, arr3, row3, col3);


    t2 = omp_get_wtime();
    cout << t2 - t1 << "- Время блочного" << endl;
    return 0;
}



void matrixmult(int** col1, int m1_row, int m1_col, int** col2, int m2_row, int m2_col, int** m3, int m3_row, int m3_col)
{

    omp_set_num_threads(4);
    int temp = 0;
    int i = 0, j = 0, k = 0;

#pragma omp parallel 
    {
        int num = omp_get_thread_num();

#pragma omp parallel for schedule(static)  private(k) 
        for (k = 0; k < m2_col; ++k)
#pragma omp parallel for schedule(static)  private(i)
            for (i = 0; i < m1_row; ++i)
#pragma omp parallel for schedule(static) shared(col1, col2, m3) private(j) reduction(+:temp)	
                for (j = 0; j < m1_col; ++j)
                {
                    temp += col1[i][j] * col2[j][k];
                    m3[i][k] = temp;
                }
    }

}