#include <thread>
#include <cmath>
#include <vector>
#include <numeric>
#include <iomanip>
#include <cstdio>

#include <iostream>
#include <ctime> // в ней функция time
#include <random>

using namespace std;

// с 4 потоками ~ 250 секунд
// с 1 потоком ~ 600 секунд
static const long MATRIX_SIZE = 8;
static const int THREADS_NUMBER = 4;
int step = MATRIX_SIZE / THREADS_NUMBER;
int** generation_matrix(int n)
{
    srand(time(NULL)); // Инициализируем генератор случайных чисел. 
    int** a = new int* [n]; // Создаем массив указателей
    for (long long i = 0; i < n; i++)
    {
        a[i] = new int[n]; // Создаем элементы
    }

    std::random_device random_device; 
    std::mt19937 generator(random_device()); // Генератор случайных чисел.

    // А дальше работа как с обычным массивом. 
    for (long long i = 0; i < n; i++)
    {
        for (long long j = 0; j < n; j++)
        {
            std::uniform_int_distribution<> distribution(0, 5);
            long long x = distribution(generator); // Случайное число.
            a[i][j] = x; // Каждый элемент случайному числу от 0 до 9
        }
    }
    return a;
}

int** generation_zero_matrix(int n)
{
    int** a = new int* [n];
    for (int i = 0; i < n; i++)
    {
        a[i] = new int[n]; // Создаем элементы
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            a[i][j] = 0; 
        }
    }
    return a;
}

void multiply_matrix(long start_op, long end_op, int** arr3, int** arr1, int** arr2)
{
    for (long long i = start_op; i < end_op; i++)
        for (long long j = 0; j < MATRIX_SIZE; j++)
        {
            //arr3[i][j] = 0;
            for (long long z = 0; z < MATRIX_SIZE; ++z)
                arr3[i][j] = arr3[i][j] + arr1[i][z] * arr2[z][j];
        }

}

void multiply_matrix_threading(long start_op, long end_op, int** result, const int thread_number, int** m1, int** m2)
{
    const long long n_elements = (MATRIX_SIZE * MATRIX_SIZE);
    const long long n_operations = n_elements / MATRIX_SIZE;
    const long long rest_operations = MATRIX_SIZE % THREADS_NUMBER;

    //long long start_op, end_op;

    //if (thread_number == 0) {
    //    // First thread does more job
    //    start_op = MATRIX_SIZE * thread_number;
    //    end_op = (MATRIX_SIZE * (thread_number + 1)) + rest_operations;
    //}
    //else {
    //    start_op = MATRIX_SIZE * thread_number + rest_operations + 1;
    //    end_op = (MATRIX_SIZE * (thread_number + 1)) + rest_operations;
    //}
    cout << "START_OP: " << start_op << "\nEND_OP: " << end_op << "\n";
    multiply_matrix(start_op, end_op, result, m1, m2);
   /* for (long long op = start_op; op < end_op; ++op) {
        const long long row = op % MATRIX_SIZE;
        const long long col = op / MATRIX_SIZE;
        long long r = 0.0f;
        for (long long i = 0; i < MATRIX_SIZE; ++i) {
            const long long e1 = m1[row][i];
            const long long e2 = m2[i][col];
            r += e1 * e2;
        }

        result[row][col] = r;
    } */
    
    /*
    |0 1 2 3 4|     |5 6 7 8 9|
    |0 1 2 3 4|     |5 6 7 8 9|
    |0 1 2 3 4| X   |5 6 7 8 9|
    |0 1 2 3 4|     |5 6 7 8 9|
    |0 1 2 3 4|     |5 6 7 8 9|

    для каждого потока должно браться по 5 элементов
    thread1 = |0,1,2,3,4,0| x |5,5,5,5,5,6|
    start_op = 0; end_op = 4
    */
}

void multithreading_execution(int** r, int** m1, int** m2, long start_op=0, long end_op=0) {
    double start = clock(); 
    std::thread threads[THREADS_NUMBER];
    
    for (int i = 0; i < THREADS_NUMBER; i++) {
        start_op = i * step;
        end_op = (i + 1) * step;
        //std::cout << "Starting thread " << i << std::endl;
        threads[i] = std::thread(multiply_matrix_threading, std::ref(start_op), std::ref(end_op), std::ref(r), i, std::ref(m1), std::ref(m2));   
    }

    //std::cout << "Calculating...." << std::endl;

    for (int i = 0; i < THREADS_NUMBER; ++i) {
        //std::cout << "Joining thread " << i << std::endl;
        threads[i].join();
    }
    
    printf("\nResult time: %.4lf\n", (clock() - start) / CLOCKS_PER_SEC);
}

int main()
{
    
   // const int n = 10; // размерность
    cout << "RAND_MAX = " << RAND_MAX << endl;
    cout << "THREADS_NUMBER = " << THREADS_NUMBER << endl;
    cout << "MATRIX_SIZE = " << MATRIX_SIZE << endl;
    int** arr1 = generation_matrix(MATRIX_SIZE);
    printf("Arr1: \n");
    for (long long i = 0; i < MATRIX_SIZE; i++)
    {
        for (long long j = 0; j < MATRIX_SIZE; j++)
        {
            //cout << arr1[i][j] << " "; // Вывести элементы на консоль
            printf("%10d", arr1[i][j]);
        }
        cout << endl; // Двумерный массив. Строка кончилась, переводим строку и на консоли
    }
    printf("\n");
    int** arr2 = generation_matrix(MATRIX_SIZE);
    printf("Arr2: \n");
    for (long long i = 0; i < MATRIX_SIZE; i++)
    {
        for (long long j = 0; j < MATRIX_SIZE; j++)
        {
            //cout << arr2[i][j] << " "; // Вывести элементы на консольку
            printf("%10d", arr2[i][j]);
        }
        cout << endl; // Двумерный массив. Строка кончилась, переводим строку и на консоли
    }
    int** arr3 = generation_zero_matrix(MATRIX_SIZE);
    int** st_matrix = generation_zero_matrix(MATRIX_SIZE);
    int** mt_matrix = generation_zero_matrix(MATRIX_SIZE);

    multiply_matrix(0, MATRIX_SIZE, arr3, arr1, arr2);
    //cout << "\nsingle thread\n";
    for (long long i = 0; i < MATRIX_SIZE; i++)
    {
        for (long long j = 0; j < MATRIX_SIZE; j++)
        {
            st_matrix[i][j] = arr3[i][j];
           // cout << arr3[i][j] << "\t";
        }
        //cout << endl;
    }

    for (long long i = 0; i < MATRIX_SIZE; i++)
        {
            for (long long j = 0; j < MATRIX_SIZE; j++)
            {

                cout << st_matrix[i][j] << "\t";
            }
            cout << endl;
        }

    arr3 = generation_zero_matrix(MATRIX_SIZE);
    cout << endl;

   multithreading_execution(arr3, arr1, arr2);
    

   // cout << "\nmulty thread\n";
   for (long long i = 0; i < MATRIX_SIZE; i++)
       for (long long j = 0; j < MATRIX_SIZE; j++)
       {

           mt_matrix[i][j] = arr3[i][j];
       }
   //cout << endl;
   //int** mt_matrix = ref(arr3);
   // cout << "\n\n>>>>> CHECK <<<<<\n\n";
   // //int** checkMatrix = generation_zero_matrix(MATRIX_SIZE);
   // cout << "\nResult:\n";
   // cout << "\single thread\n";
   // for (long long i = 0; i < MATRIX_SIZE; i++)
   // {
   //     for (long long j = 0; j < MATRIX_SIZE; j++)
   //     {

   //         cout << st_matrix[i][j] << "\t";
   //     }
   //     cout << endl;
   // }
   cout << "\nmulty thread\n";
   for (long long i = 0; i < MATRIX_SIZE; i++)
   {
       for (long long j = 0; j < MATRIX_SIZE; j++)
       {

           cout << mt_matrix[i][j] << "\t";
       }
       cout << endl;
   }

    cout << "\n <<<<<<<<<<<<<<<<<<<<<<<<<Checking>>>>>>>>>>>>>>>>>>>>>>>>>>>\n";
    bool flag = 0;
    for (long long i = 0; i < MATRIX_SIZE; i++)
    {
        for (long long j = 0; j < MATRIX_SIZE; j++)
        {
            if (st_matrix[i][j] != mt_matrix[i][j])
            {
                flag = 1;
                break;
            }
                //cout <<  << "\t";
        }
        if (flag) {
            break;
        }
        //cout << endl;
    }
    if (flag) {
        cout <<"Matrixes are not equal" << "\t";
    }
    else {
        cout << "Matrixes are equal" << "\t";
    }


}