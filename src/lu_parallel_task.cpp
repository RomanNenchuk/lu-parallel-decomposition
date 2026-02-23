#include "../include/matrix.hpp"
#include <omp.h>

void luParallelTask(const Matrix &A, Matrix &L, Matrix &U, int n) {
    // Розмір блоку для однієї задачі
    const int blockSize = 32; 

    #pragma omp parallel
    {
        // Тільки один потік створює задачі (менеджер), інші їх підхоплюють (виконавці)
        #pragma omp single
        {
            for (int k = 0; k < n; k++) {

                // 1. Обчислення рядка U через завдання (Tasks)
                for (int start = k; start < n; start += blockSize) {
                    int end = std::min(start + blockSize, n);

                    // Створюємо окрему задачу для обробки блоку стовпців
                    #pragma omp task shared(A, L, U) firstprivate(start, end, k)
                    {
                        for (int i = start; i < end; i++) {
                            double sum = 0.0;
                            for (int j = 0; j < k; j++)
                                sum += L[k][j] * U[j][i];
                            U[k][i] = A[k][i] - sum;
                        }
                    }
                }
                // Чекаємо завершення всіх задач обчислення рядка U перед переходом до L
                #pragma omp taskwait

                L[k][k] = 1.0;

                // 2. Обчислення стовпця L через завдання
                for (int start = k + 1; start < n; start += blockSize) {
                    int end = std::min(start + blockSize, n);

                    #pragma omp task shared(A, L, U) firstprivate(start, end, k)
                    {
                        for (int i = start; i < end; i++) {
                            double sum = 0.0;
                            for (int j = 0; j < k; j++)
                                sum += L[i][j] * U[j][k];
                            L[i][k] = (A[i][k] - sum) / U[k][k];
                        }
                    }
                }
                // Бар'єр: чекаємо завершення стовпця L перед наступною ітерацією k
                #pragma omp taskwait
            }
        }
    }
}