#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <time.h>

void matrixmult(int** m1, int m1_row, int m1_col, int** m2, int m2_row, int m2_col, int** m3, int m3_row, int m3_col);

void matrixfill(int** m, int row, int col)
{
    for (int i = 0; i < row; ++i)
        for (int j = 0; j < col; ++j)
        {
            std::cout << "Enter [ " << i << " ][ " << j << " ] element: ";
            std::cin >> m[i][j];
        }
}

void print_matr(int** m, int row, int col)
{
    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < col; ++j)
            std::cout << std::setw(3) << m[i][j] << ' ';

        std::cout << std::endl;
    }
}

int man()
{
    int row1 = 0;
    int col1 = 0;

    clock_t start, stop;

    std::cout << "Enter number of rows of the first matrix: ";
    std::cin >> row1;

    std::cout << "Enter number of columns of the first matrix: ";
    std::cin >> col1;

    int** arr1 = new int* [row1];

    for (int i = 0; i < row1; ++i)
        arr1[i] = new int[col1];

    matrixfill(arr1, row1, col1);

    int row2 = 0;
    int col2 = 0;

    std::cout << "Enter number of rows of the second matrix: ";
    std::cin >> row2;

    std::cout << "Enter number of columns of the second matrix: ";
    std::cin >> col2;

    int** arr2 = new int* [row2];
    for (int i = 0; i < row2; ++i)
        arr2[i] = new int[col2];

    matrixfill(arr2, row2, col2);

    int row3 = row1;
    int col3 = col2;

    int** arr3 = new int* [row3];
    for (int i = 0; i < row3; ++i)
        arr3[i] = new int[col3];

    for (int i = 0; i < row3; ++i)
        for (int j = 0; j < col3; ++j)
            arr3[i][j] = 0;

    std::cout << "\nFirst matrix: \n";
    print_matr(arr1, row1, col1);

    std::cout << "\nSecond matrix: \n";
    print_matr(arr2, row2, col2);

    start = clock();

    //если число столбцов первой матрицы равно числу строк второй матрицы
    if (col1 == row2)
        matrixmult(arr1, row1, col1, arr2, row2, col2, arr3, row3, col3);

    else
    {
        std::cerr << " Error! " << std::endl;
        return 1;
    }

    std::cout << "\nResult matrix:" << std::endl;
    print_matr(arr3, row3, col3);



    for (int i = 0; i < row1; i++)
        delete[] arr1[i];
    delete[] arr1;
    for (int i = 0; i < row2; i++)
        delete[] arr2[i];
    delete[] arr2;
    for (int i = 0; i < row3; i++)
        delete[] arr3[i];
    delete[] arr3;

    stop = clock();
    printf("Time=%f sec.\n", ((double)(stop - start) / 1000.0));
    return 0;
}



void matrixmult(int** m1, int m1_row, int m1_col, int** m2, int m2_row, int m2_col, int** m3, int m3_row, int m3_col)
{

    omp_set_num_threads(12);
    int temp = 0;
    int i = 0, j = 0, k = 0;

#pragma omp parallel 
    {
        int num = omp_get_thread_num();

#pragma omp parallel for schedule(static)  private(k) 
        for (k = 0; k < m2_col; ++k)
#pragma omp parallel for schedule(static)  private(i)
            for (i = 0; i < m1_row; ++i)
#pragma omp parallel for schedule(static) shared(m1, m2, m3) private(j) reduction(+:temp)	
                for (j = 0; j < m1_col; ++j)
                {
                    temp += m1[i][j] * m2[j][k];
                    m3[i][k] = temp;
                    //printf("[%d]: m[%d][%d] += m1[%d][%d] * m2[%d][%d] = %d * %d = %d \n", num, i, k, i, j, j, k, m1[i][j], m2[j][k], m3[i][k]);
                }
    }

}