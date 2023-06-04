#include <cuda.h>
#include <stdio.h>
#include <iostream>
#include <float.h>
using namespace std;

int N = 2500,                 // Height (количество строк)
    M = 2500;                 // Widht  (количество столбцов)

enum mode{
    MATRIX_MUL_VECTOR,
    VECTOR_MUL_MATRIX
} 
MODE = MATRIX_MUL_VECTOR;
//MODE = VECTOR_MUL_MATRIX;

void PrintM(int *);
void InitMatrix(int *);
void TranspMatrix(int *);
void PrintVector(int *, int);
int* InitVector(int, int NotNull = 1);

__global__ void MulMatrixAndVector(int *A, int *V, int *C, int Widht, int Height){

    int i = threadIdx.x + blockIdx.x * blockDim.x;

    if(i < Height){

        int *Row_A = (int*)((char*)A + i * Widht * sizeof(int));

        int Sum  = 0;
        for(int k = 0; k < Widht; k++){
            Sum += Row_A[k] * V[k];
        }
        C[i] = Sum;
    }
}

void TimeCalculation(int *A, int *V, int *C, int NUM_STREAMS, float &MinTime, int &NumStreams){

    int *A_dev, *V_dev, *C_dev;

    cudaMalloc(&V_dev, M * sizeof(int));
    cudaMalloc(&C_dev, N * sizeof(int));
    cudaMalloc(&A_dev, N * M * sizeof(int));

    cudaMemcpy(V_dev, V, M * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(C_dev, C, N * sizeof(int), cudaMemcpyHostToDevice);

    cudaStream_t *streams = new cudaStream_t[NUM_STREAMS];

    cudaEvent_t start, stop;
    cudaEventCreate(&start); 
    cudaEventCreate(&stop);

    for(int i = 0; i < NUM_STREAMS; i++){
        cudaStreamCreate(&streams[i]);
    }

    int SIZE_CHUNK = (N / NUM_STREAMS) * M;             // Количество элментов в части матрицы
    int STEP = SIZE_CHUNK;

    cudaEventRecord(start, 0);                          // Фиксируем время начала расчетов
    for(int i = 0; i < NUM_STREAMS; i++){

        if(i == NUM_STREAMS - 1){
            SIZE_CHUNK = (N - N / NUM_STREAMS * (NUM_STREAMS - 1)) * M;
        }

        // Копируем часть матрицы A на устройство
        cudaMemcpyAsync(A_dev + i * STEP,               // Указатель на часть матрицы на устройстве 
                        A  +    i * STEP,               // Указатель на часть матрицы на хосте
                        sizeof(int) * SIZE_CHUNK,       // Количество копируемых байт 
                        cudaMemcpyHostToDevice,         // Копируем с хоста на устройство
                        streams[i] );                   // Номер очередного потока

        int Threads = 128;
        int Blocks = SIZE_CHUNK / M / Threads + 1;

        MulMatrixAndVector<<< Blocks, Threads, 0 , streams[i] >>>( 
                                A_dev + i * STEP,       // Указатель на часть матрицы на устройстве
                                V_dev,                  // Указатель на вектор V на устройстве
                                C_dev + i * STEP / M,   // Указатель на часть вектора C на устройстве
                                M, SIZE_CHUNK / M );    // Ширина и Высота части матрицы
        
        // Копируем часть вектора C на хост
        cudaMemcpyAsync(C + i * STEP / M,               // Указатель на часть вектора С на хосте
                        C_dev + i * STEP / M,           // Указатель на часть C_dev на устройстве
                        sizeof(int) * SIZE_CHUNK / M,   // Количество копируемых байт
                        cudaMemcpyDeviceToHost,         // Копируем с устройства на хост
                        streams[i] );                   // Номер очередного потока
    }
    cudaDeviceSynchronize();                            // Ожидаем завершение выполнения всех нитей

    cudaEventRecord(stop, 0);                           // Записываем время окончания расчетов
    cudaEventSynchronize(stop);
    float elapsedTime;
    cudaEventElapsedTime(&elapsedTime, start, stop);

    //printf( "%d: %0.3f\n", NUM_STREAMS, elapsedTime);

    if(elapsedTime < MinTime){
        MinTime = elapsedTime; 
        NumStreams = NUM_STREAMS;
    }

    cudaFree(V_dev);
    cudaFree(C_dev);
    cudaFree(A_dev);
    cudaEventDestroy(start);
    cudaEventDestroy(stop);

    for(int i = 0; i < NUM_STREAMS; i++){
        cudaStreamDestroy(streams[i]);
    }
    delete [] streams;

    memset(C, 0, N * sizeof(int));
}

int main () {
    
    int *A;                                 
    cudaMallocHost((void**)&A, N * M * sizeof(int));

    int *V = NULL, *C = NULL;               // указатели на исходные данные и результат

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

    float MinTime = FLT_MAX; int NumStreams;
    for(int Streams = 2; Streams < 500; Streams++){
        TimeCalculation(A, V, C, Streams, MinTime, NumStreams);
    }

    printf("\n MinTime: %.3f, NumStreams: %d\n\n", MinTime, NumStreams);

    cudaFree(A);
    cudaFree(V);
    cudaFree(C);

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
            A[i * M +j] = rand() % 5 +1;
}

int * InitVector(int CountElem, int NotNull){
    int *Vector;
    cudaMallocHost((void**)&Vector, CountElem * sizeof(int));
    if(NotNull)
        for(int i=0; i < CountElem; i++) { Vector[i] = 2; }
    else
        memset(Vector, 0, CountElem * sizeof(int));
    return Vector;
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