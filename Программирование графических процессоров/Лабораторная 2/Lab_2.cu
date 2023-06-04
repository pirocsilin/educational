#include <cuda.h>
#include <stdio.h>
#include <iostream>
#define THREAD_PER_BLOCK 128

using namespace std;

// Тестовые размеры:
// 1000 x 500   1000 x 1000     1500 x 1000     2000 x 1000
// 2000 x 1500  2500 x 1500     2500x2000       3000 x 2000

int N = 1000,               // Height (количество строк)
    M = 500;                // Widht  (количество столбцов)

enum mode{                  // Выбор режима умножения:
    MATRIX_MUL_VECTOR,      // 1. Матрица на вектор (вектор - столбец)
    VECTOR_MUL_MATRIX       // 2. Вектор на матрицу (вектор - строка)
} 
MODE = MATRIX_MUL_VECTOR;
//MODE = VECTOR_MUL_MATRIX;

void PrintM(int *);
void InitMatrix(int *);
void TranspMatrix(int *);
void PrintVector(int *, int);
int* InitVector(int, int NotNull = 1);

__shared__ int Part_v[THREAD_PER_BLOCK];                    // Части вектора
__shared__ int c[THREAD_PER_BLOCK];                         // Запись результатов 

__global__ void MulMatrixAndVector(int *A, int *V, int *C, int Widht, int Height, int pitch){

    int Global_i = threadIdx.x + blockIdx.x * blockDim.x;   // Получаем глобальный номер нити
    
    int *Row_A = (int*)((char*)A + Global_i * pitch);       // Получаем первый элемент соотв-й строки матрицы А

    c[threadIdx.x] = 0;                                     // Каждая нить блока инициализирует нулем "свой"
    __syncthreads();                                        // элемент результирующего вектора в разделяемой памяти                                       
        
        for(int k = 0; k < Widht / blockDim.x + 1; k++){    // Рзбиваем вектор V на k - частей
            
            int i = threadIdx.x + k * blockDim.x;           // Получаем номер элемента вектора,
            
            if(i < Widht){                                  // если он не выходит за границы вектора,
                Part_v[threadIdx.x] = V[i];                 // то загружаем его в разделяемую память
            }
            __syncthreads();                                // Ожидаем пока все нити блока загрузят "свой" элемент
                                                            
            if(Global_i < Height){                          // Если Global_i не больше высоты матрицы, то
                for(int j = 0; j < blockDim.x; j++){        // умножаем полученную часть вектора на часть матрицы  
                        c[threadIdx.x] += Part_v[j] * Row_A[k * blockDim.x + j];    
                }
            }
            __syncthreads();                                // Ожидаем пока все нити блока закончат вычисления 
        }                                                   // перед загрузкой следующей порции данных.

        if(Global_i < Height){                              // Проверка выхода номера нити за границы матрицы
            C[Global_i] = c[threadIdx.x];                   // Копируем результат в глобальную память                   
        }
        __syncthreads();                                    // Ждем пока все нити закончат копирование
}

int main () {
    
    int *A = (int*)malloc(N * M * sizeof(int));

    int *V = NULL, *C = NULL;               // указатели на исходные данные и результат
    int *A_dev, *V_dev, *C_dev;             // указатели для записи адресов в памяти GPU
    size_t pitch;                           // фактическая ширина строки, in Bytes

    InitMatrix(A);                          // Инициализация матрицы.

    switch (MODE){                          
        case MATRIX_MUL_VECTOR:             // Если умножаем матрицу на вектор, то 
            V = InitVector(M);              // вектор будем рассматривать как один столбец
            C = InitVector(N, 0);           // матрицы, тогда его длина = ширине матрицы,
            break;                          // а результирующий вектор = высоте матрицы. 

        case VECTOR_MUL_MATRIX:             // При умножении вектора на матрицу, вектор 
            TranspMatrix(A);                // рассматривают как однострочную матрицу, 
            swap(N, M);                     // тогда его следует умножать на столбцы матрицы A.
            V = InitVector(M);              // Но в этом случа доступ к памяти будет не
            C = InitVector(N, 0);           // эффективным. Поэтому транспонируем матрицу
            break;                          // и работаем с вектором как как в первом случае
    }

    //PrintM(A);
    //PrintVector(V, M);

    // Выделяем память на Девайсе для векторов и матрицы
    cudaMalloc(&V_dev, M * sizeof(int));
    cudaMalloc(&C_dev, N * sizeof(int));
    cudaMallocPitch(&A_dev, &pitch, M * sizeof(int), N);

    // Создание и инициализация объектов событий
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    // Запись события start в этой точке выполнения программы
    cudaEventRecord(start, 0);

    // Копируем данные с хоста на устройство
    cudaMemcpy(V_dev, V, M * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(C_dev, C, N * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy2D(A_dev, pitch, A, M * sizeof(int), M * sizeof(int), N, cudaMemcpyHostToDevice);
    
    int Threads = THREAD_PER_BLOCK;
    int Blocks = N / Threads + 1;
    
    MulMatrixAndVector<<< Blocks, Threads >>>(A_dev, V_dev, C_dev, M, N, pitch);
    
    cudaDeviceSynchronize();                        // ожидаем завершение выполнения всех нитей
    
    cudaMemcpy(C, C_dev, N * sizeof(int), cudaMemcpyDeviceToHost);  // копируем результат на хост
    
    cudaEventRecord(stop, 0);                       // запись события stop в точке программы
    cudaEventSynchronize(stop);                     // ожидание завершения всех асинхронных команд 
    float elapsedTime;
    cudaEventElapsedTime(&elapsedTime, start, stop);
    printf(" \n elapsedTime = %f\n", elapsedTime);

    // Очищаем память на устройстве
    cudaFree(V_dev);
    cudaFree(C_dev);
    cudaFree(A_dev);
    cudaEventDestroy(start);
    cudaEventDestroy(stop);

    //PrintVector(C, N);

	return 0;
}

void TranspMatrix(int *A){
    int *T = (int*)malloc(N * M * sizeof(int));
    for(int i=0; i < M; i++)
        for(int j=0; j < N; j++)
            T[i * N + j] = A[j * M + i];
    memcpy(A, T, sizeof(T));
}

void InitMatrix(int *A){
    for(int i=0; i < N; i++)
        for(int j=0; j < M; j++)
            A[i * M +j] = rand() % 5 + 1;
}

int * InitVector(int CountElem, int NotNull){
    int *V = (int*)malloc(CountElem * sizeof(int));
    if(NotNull)
        for(int i=0; i < CountElem; i++) { V[i] = 2; }
    else
        memset(V, 0, CountElem * sizeof(int));
    return V;
}

void PrintVector(int *V, int n){
    cout << endl;
    for(int i = 0; i < n; i++) 
        cout << " " << V[i]; 
    cout << endl;
}

void PrintM(int *Matrix){
    for(int i=0; i < N; i++){
        for(int j=0; j < M; j++)
            cout << " " << Matrix[i * M +j];
        cout << endl;
    }
}