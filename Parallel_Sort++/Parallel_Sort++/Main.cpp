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
    int thread_count = 2;
#pragma omp parallel num_threads (thread_count) default(none) shared(arr) 
    for (int phase = 0; phase < SIZE; ++phase) {
        if (phase % 2 == 0) {
#pragma omp for 
            for (int i = 1; i < SIZE; i += 2) {
                if (arr[i - 1] > arr[i]) {
                    swap(arr[i - 1], arr[i]);
                }
            }
        }
        else {
#pragma omp for 
            for (int i = 1; i < SIZE - 1; i += 2) {
                if (arr[i] > arr[i + 1]) {
                    swap(arr[i + 1], arr[i]);
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

void ParallelShellSort(int* array, int n)
{

#pragma omp parallel firstprivate(n)
    {
        int m = omp_get_thread_num();
        int step = n / 2;//�������������� ���. 
        while (step > 0)//���� ��� �� 0 
        {
#pragma omp parallel for
            for (int i = 0; i < (n - step); i++)
#pragma omp critical 
            {
                int j = i;
                //����� ���� ������� � i-�� �������� 
                while (j >= 0 && array[j] > array[j + step])
                    //���� �� ������ � ������ ������� 
                    //� ���� ��������������� ������� ������ 
                    //��� ������� ����������� �� ���������� ���� 
                {
                    //������ �� ������� 
                    int temp = array[j];
                    array[j] = array[j + step];
                    array[j + step] = temp;
                    // cout << "����� " << m << " ������ ������� �������� � �������� " << j << " � " << j + step << "\n";
                    j--;
                }
            }
            step = step / 2;//��������� ��� 
        }
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
    int* e;
    int* f;


    a = (int*)malloc(sizeof(int*) * SIZE);
   
    b = (int*)malloc(sizeof(int*) * SIZE);

    c = (int*)malloc(sizeof(int*) * SIZE);

    d = (int*)malloc(sizeof(int*) * SIZE);

    e = (int*)malloc(sizeof(int*) * SIZE);
    
    f = (int*)malloc(sizeof(int*) * SIZE);
    setlocale(LC_ALL, "Russian");

    // ���������� ������� ���������� �������
    for (int i = 0; i < SIZE; i++) {
        a[i] = SIZE - i;
        b[i] = a[i];
        c[i] = a[i];
        d[i] = a[i];
        e[i] = a[i];
        f[i] = a[i];
    }
   
    
    double t1 = omp_get_wtime();
    QuickSort(a, 0, SIZE - 1); 
              
    cout << "����� ��������� ������� ���������� " << omp_get_wtime() - t1 << endl;

    t1 = omp_get_wtime();
    QuickSort_Parallel(b, SIZE);
    cout << "����� ������������� ������� ���������� " << omp_get_wtime() - t1 << endl<<endl;

    t1 = omp_get_wtime();
    BubbleSort(c);
    cout << "����� ��������� ���������� �������� " << omp_get_wtime() - t1 << endl;

    t1 = omp_get_wtime();
    BubbleParallelSort(d);
    cout << "����� ������������� ���������� �������� " << omp_get_wtime() - t1 << endl<<endl;

    t1 = omp_get_wtime();
    Shell(e);
    cout << "����� ��������� ���������� ����� " << omp_get_wtime() - t1 << endl;

    t1 = omp_get_wtime();
    ParallelShellSort(f, SIZE);
    cout << "����� ������������� ��� ����� " << omp_get_wtime() - t1 << endl;

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
    for (int i = 0; i < SIZE; i++)
        if (a[i] != d[i]) {
            cout << "�� �����, ����� �� �����" << endl;
            break;
        }
    for (int i = 0; i < SIZE; i++)
        if (a[i] != e[i]) {
            cout << "�� �����, ����� �� �����" << endl;
            break;
        }
    for (int i = 0; i < SIZE; i++)
        if (a[i] != f[i]) {
            cout << "�� �����, ����� �� �����" << endl;
            break;
        }

    system("pause");
}