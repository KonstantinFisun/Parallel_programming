#include <stdio.h>
#include <omp.h>
#include <iostream>
#define SIZE 10

using namespace std;

//Функция сортировки пузырьком
void BubbleSort(int* arr) {
    for (int i = 0; i < SIZE - 1; i++) {
        for (int j = 0; j < SIZE - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}


void Shell(int* arr) //сортировка Шелла
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

// Функция быстрой сортировки
void QuickSort(int* array, int left, int right)
{
    int pivot; // разрешающий элемент
    int l_hold = left; //левая граница
    int r_hold = right; // правая граница
    pivot = array[left];
    while (left < right) // пока границы не сомкнутся
    {
        while ((array[right] >= pivot) && (left < right))
            right--; // сдвигаем правую границу пока элемент [right] больше [pivot]
        if (left != right) // если границы не сомкнулись
        {
            array[left] = array[right]; // перемещаем элемент [right] на место разрешающего
            left++; // сдвигаем левую границу вправо
        }
        while ((array[left] <= pivot) && (left < right))
            left++; // сдвигаем левую границу пока элемент [left] меньше [pivot]
        if (left != right) // если границы не сомкнулись
        {
            array[right] = array[left]; // перемещаем элемент [left] на место [right]
            right--; // сдвигаем правую границу вправо
        }
    }
    array[left] = pivot; // ставим разрешающий элемент на место
    pivot = left;
    left = l_hold;
    right = r_hold;
    if (left < pivot) // Рекурсивно вызываем сортировку для левой и правой части массива
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

    // Заполнение массива случайными числами
    for (int i = 0; i < SIZE; i++) {
        a[i] = SIZE - i;
        b[i] = a[i];
        c[i] = a[i];
        d[i] = a[i];
    }
   
    
    double t1 = omp_get_wtime();
    QuickSort(a, 0, SIZE - 1); 
              
    cout << "Время линейного быстрая сортировка " << omp_get_wtime() - t1 << endl;

    t1 = omp_get_wtime();
    QuickSort_Parallel(b, SIZE);
    cout << "Время параллельного быстрая сортировка " << omp_get_wtime() - t1 << endl;

    t1 = omp_get_wtime();
    BubbleSort(c);
    cout << "Время линейного сортировка пузырька " << omp_get_wtime() - t1 << endl;

    t1 = omp_get_wtime();
    Shell(d);
    cout << "Время линейного сортировка Шелла " << omp_get_wtime() - t1 << endl;

    // Вывод элементов массива до сортировки
    /*
    for (int i = 0; i < SIZE; i++)
        cout << a[i] << " ";
    cout << endl;

    for (int i = 0; i < SIZE; i++)
        cout << b[i] << " ";
    cout << endl;
    */
    //Проверка на совпадение
    for(int i = 0; i < SIZE; i++)
        if (a[i] != b[i]) {
            cout << "Всё плохо, давай по новой" << endl;
            break;
        }
    for (int i = 0; i < SIZE; i++)
        if (a[i] != c[i]) {
            cout << "Всё плохо, давай по новой" << endl;
            break;
        }
   

    system("pause");
}