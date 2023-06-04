#include <cuda.h>
#include <stdio.h>
#include <iostream>
#include <float.h>
using namespace std;

float host_MulMatrixAndMatrix(int *, int *, int *);
int CheckRezult(int *, int *);
void InitMatrix(int *);
void PrintM(int *);

// Test data
// WIDHT: 500  800  1000  1200  1500  1800  2000  2500

#define WIDHT 2500      // Размер матрицы: WIDHT х WIDHT
#define THREADS 16      // Нитей на блок:  THREADS х THREADS
#define NUM_STREAMS 30  // Количество потоков

__global__ void MulMatrixAndMatrix(int *A, int *B, int *C, int Widht, int Height){

    int Tx = threadIdx.x;                                  // Номер столбца нити в блоке
    int Ty = threadIdx.y;                                  // Номер строки нити в блоке

    int Glob_y = (blockDim.y * blockIdx.y + Ty) * Widht;   // Индекс начала строки матрицы
    int Glob_x = blockDim.x * blockIdx.x + Tx;             // Индекс столбца матрицы

    int sum = 0;
    __shared__ int shared_A[THREADS][THREADS];
    __shared__ int shared_B[THREADS][THREADS];

    for(int i = 0; i < gridDim.x; i++){                 // Цикл по фрагментам матрицы

        int dy = i * blockDim.y * Widht;                // Смещение по столбцам и строкам
        int dx = i * blockDim.x;                        // в зависимости от фрагмента
        
        if(dx + Tx < Widht && Glob_y < Widht * Height)  // Если не выходим за границы
            shared_A[Ty][Tx] = A[Glob_y + dx + Tx];     // то пишем в разделяемую память

        if(Glob_x < Widht)
            shared_B[Ty][Tx] = B[Glob_x + dy + Ty * Widht];
        
        __syncthreads();                                // Ждем пока все нити загрузят данные

        if(Glob_x < Widht && Glob_y < Widht * Height)
            for(int k = 0; k < blockDim.x; k++){        // shared_A * shared_B
                int a = shared_A[Ty][k];
                int b = shared_B[k][Tx];
                sum += a * b;
            }

        __syncthreads();                                // Ждем пока все нити выполнят расчет
    }
    if(Glob_x < Widht && Glob_y < Widht * Height)       
        C[Glob_y + Glob_x] = sum;                       // Сохраняем результат
}

float MulMatrixOnMatrix(int *A, int *B, int *C, int *A_dev, int *B_dev, int *C_dev){
    
    cudaStream_t *streams = new cudaStream_t[NUM_STREAMS];
    for(int i = 0; i < NUM_STREAMS; i++){
        cudaStreamCreate(&streams[i]);
    }

    int SIZE_CHUNK = (WIDHT / NUM_STREAMS) * WIDHT;
    int STEP = SIZE_CHUNK;

    cudaEvent_t start, stop;
    cudaEventCreate(&start); 
    cudaEventCreate(&stop);
    cudaEventRecord(start, 0);

    // Копируем матрицы С и В на устройство целиком
    cudaMemcpy(C_dev, C, WIDHT * WIDHT * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(B_dev, B, WIDHT * WIDHT * sizeof(int), cudaMemcpyHostToDevice);

    for(int i = 0; i < NUM_STREAMS; i++){
        if(i == NUM_STREAMS - 1){
            SIZE_CHUNK = (WIDHT - WIDHT / NUM_STREAMS * (NUM_STREAMS - 1)) * WIDHT;
        }
        // Копируем часть матриц A на устройство
        cudaMemcpyAsync(A_dev + i * STEP, A + i * STEP, sizeof(int) * SIZE_CHUNK,
                        cudaMemcpyHostToDevice, streams[i] );

        dim3 Threads(THREADS, THREADS);
        // Рассчитываем Ширину и Высоту сетки блоков
        dim3 Blocks(WIDHT / THREADS + 1, SIZE_CHUNK / WIDHT / THREADS + 1);

        MulMatrixAndMatrix<<< Blocks, Threads, 0 , streams[i] >>>
                                (A_dev + i * STEP, 
                                B_dev, 
                                C_dev + i * STEP,
                                WIDHT, SIZE_CHUNK / WIDHT);

        // Копируем часть результата с девайса на хост
        cudaMemcpyAsync(C + i * STEP, C_dev + i * STEP, sizeof(int) * SIZE_CHUNK,
                        cudaMemcpyDeviceToHost, streams[i] );
    }
    cudaDeviceSynchronize();

    cudaEventRecord(stop, 0);
    cudaEventSynchronize(stop);
    float elapsedTime;
    cudaEventElapsedTime(&elapsedTime, start, stop);

    cudaEventDestroy(start);
    cudaEventDestroy(stop);

    delete [] streams;

    return elapsedTime;
}

int main () {
    
    int *A, *B, *C;
    int *A_dev, *B_dev, *C_dev;

    cudaMallocHost((void**)&A, WIDHT * WIDHT * sizeof(int));
    cudaMallocHost((void**)&B, WIDHT * WIDHT * sizeof(int));
    cudaMallocHost((void**)&C, WIDHT * WIDHT * sizeof(int));

    cudaMalloc(&A_dev, WIDHT * WIDHT * sizeof(int));
    cudaMalloc(&B_dev, WIDHT * WIDHT * sizeof(int));
    cudaMalloc(&C_dev, WIDHT * WIDHT * sizeof(int));
    
    srand(1); InitMatrix(A);
    srand(2); InitMatrix(B);
    memset(C, 0, WIDHT * WIDHT * sizeof(int));

    float dev_Time = MulMatrixOnMatrix(A, B, C, A_dev, B_dev, C_dev);

    int *host_C = (int*)malloc(WIDHT * WIDHT * sizeof(int));

    float host_Time = host_MulMatrixAndMatrix(A, B, host_C);

    if(!CheckRezult(C, host_C)) 
        printf("\n ERROR CALCULATION !\n\n");
    else
        printf("\nMatrix size %d elem:\n\tDevice Time\t%.2f\n\tHost Time\t%.2f\n\tAcceleration\t%.2f\n\n", 
                   WIDHT * WIDHT, dev_Time, host_Time, host_Time / dev_Time);

    cudaFree(A);    cudaFree(A_dev);
    cudaFree(B);    cudaFree(B_dev);
    cudaFree(C);    cudaFree(C_dev);
    free(host_C);

	return 0;
}

int CheckRezult(int *C_dev, int *C_host){
    int check = 1;
    for(int i = 0; i < WIDHT * WIDHT; i++)
        if(C_dev[i] != C_host[i]){
            check = 0;
            break;
        }
    
    return check;
}

float host_MulMatrixAndMatrix(int *A, int *B, int *C){

    cudaEvent_t start, stop;
    cudaEventCreate(&start); 
    cudaEventCreate(&stop);

    cudaEventRecord(start, 0);
    for(int i = 0; i < WIDHT; i++)
        for(int j = 0; j < WIDHT; j++){
            int sum = 0;
            for(int k = 0; k < WIDHT; k++)
                sum += A[i * WIDHT + k] * B[k * WIDHT + j];
            C[i * WIDHT + j] = sum;
        }
    cudaEventRecord(stop, 0);
    cudaEventSynchronize(stop);

    float elapsedTime;
    cudaEventElapsedTime(&elapsedTime, start, stop);

    cudaEventDestroy(start);
    cudaEventDestroy(stop);

    return elapsedTime;
}

void InitMatrix(int *A){
    for(int i=0; i < WIDHT; i++)
        for(int j=0; j < WIDHT; j++)
            A[i * WIDHT +j] = rand() % 5 +1;
}