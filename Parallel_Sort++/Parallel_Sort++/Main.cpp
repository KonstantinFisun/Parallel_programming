#include <stdio.h>
#include <omp.h>
#include <iostream>
#define SIZE 10

using namespace std;

//������� ���������� ���������
void BubbleSort(int* arr) {
    for (int i = 0; i < SIZE - 1; i++) {
        for (int j = 0; j < SIZE - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void BubbleParallelSort(int* arr) {
#pragma omp parallel shared(arr)
    {
        for (int i = 0; i < SIZE - 1; i++) {
            for (int j = 0; j < SIZE - i - 1; j++) {
                if (arr[j] > arr[j + 1]) {
                    swap(arr[j], arr[j + 1]);
                }
            }
        }
    }
}

void Shell(int* arr) //���������� �����
{
    int d = SIZE;
    d = d / 2;
    while (d > 0)
    {
        for (int i = 0; i < SIZE - d; i++)
        {
            int j = i;
            while (j >= 0 && arr[j] > arr[j + d])
            {
                swap(arr[j], arr[j + d]);
                j--;
            }
        }
        d = d / 2;
    }
}

// ������� ������� ����������
void QuickSort(int* array, int left, int right)
{
    int pivot; // ����������� �������
    int l_hold = left; //����� �������
    int r_hold = right; // ������ �������
    pivot = array[left];
    while (left < right) // ���� ������� �� ���������
    {
        while ((array[right] >= pivot) && (left < right))
            right--; // �������� ������ ������� ���� ������� [right] ������ [pivot]
        if (left != right) // ���� ������� �� ����������
        {
            array[left] = array[right]; // ���������� ������� [right] �� ����� ������������
            left++; // �������� ����� ������� ������
        }
        while ((array[left] <= pivot) && (left < right))
            left++; // �������� ����� ������� ���� ������� [left] ������ [pivot]
        if (left != right) // ���� ������� �� ����������
        {
            array[right] = array[left]; // ���������� ������� [left] �� ����� [right]
            right--; // �������� ������ ������� ������
        }
    }
    array[left] = pivot; // ������ ����������� ������� �� �����
    pivot = left;
    left = l_hold;
    right = r_hold;
    if (left < pivot) // ���������� �������� ���������� ��� ����� � ������ ����� �������
        QuickSort(array, left, pivot - 1);
    if (right > pivot)
        QuickSort(array, pivot + 1, right);
}

void QuickSort_Parallel(int* a, int n) {
    int i = 0, j = n - 1;
    int pivot = a[n / 2];

    do {
        while (a[i] < pivot) {
            i++;
        }
        while (a[j] > pivot) {
            j--;
        }
        if (i <= j) {
            swap(a[i], a[j]);
            i++; j--;
        }
    } while (i <= j);

#pragma omp parallel shared(a)
    {
        if (j > 0) {
            QuickSort_Parallel(a, (j + 1));
        }
    }
#pragma omp parallel shared(a)
    {
        if (n > i) {
            QuickSort_Parallel(a + i, (n - i));
        }
    }

}

void main()
{
    int* a;
    int* b;
    int* c;
    int* d;


    a = (int*)malloc(sizeof(int*) * SIZE);
   
    b = (int*)malloc(sizeof(int*) * SIZE);

    c = (int*)malloc(sizeof(int*) * SIZE);

    d = (int*)malloc(sizeof(int*) * SIZE);
    
    setlocale(LC_ALL, "Russian");

    // ���������� ������� ���������� �������
    for (int i = 0; i < SIZE; i++) {
        a[i] = SIZE - i;
        b[i] = a[i];
        c[i] = a[i];
        d[i] = a[i];
    }
   
    
    double t1 = omp_get_wtime();
    QuickSort(a, 0, SIZE - 1); 
              
    cout << "����� ��������� ������� ���������� " << omp_get_wtime() - t1 << endl;

    t1 = omp_get_wtime();
    QuickSort_Parallel(b, SIZE);
    cout << "����� ������������� ������� ���������� " << omp_get_wtime() - t1 << endl;

    t1 = omp_get_wtime();
    BubbleSort(c);
    cout << "����� ��������� ���������� �������� " << omp_get_wtime() - t1 << endl;

    t1 = omp_get_wtime();
    BubbleParallelSort(c);
    cout << "����� ������������� ���������� �������� " << omp_get_wtime() - t1 << endl;

    t1 = omp_get_wtime();
    Shell(d);
    cout << "����� ��������� ���������� ����� " << omp_get_wtime() - t1 << endl;

    // ����� ��������� ������� �� ����������
    /*
    for (int i = 0; i < SIZE; i++)
        cout << a[i] << " ";
    cout << endl;

    for (int i = 0; i < SIZE; i++)
        cout << b[i] << " ";
    cout << endl;
    */
    //�������� �� ����������
    for(int i = 0; i < SIZE; i++)
        if (a[i] != b[i]) {
            cout << "�� �����, ����� �� �����" << endl;
            break;
        }
    for (int i = 0; i < SIZE; i++)
        if (a[i] != c[i]) {
            cout << "�� �����, ����� �� �����" << endl;
            break;
        }
   

    system("pause");
}