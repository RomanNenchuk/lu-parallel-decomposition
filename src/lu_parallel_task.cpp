#include "../include/matrix.hpp"
#include <omp.h>

void luParallelTask(const Matrix &A, Matrix &L, Matrix &U, int n) {

    const int blockSize = 32;  // можна 16 / 32 / 64

    #pragma omp parallel
    {
        // тільки один потік виконує код, що нижче
        #pragma omp single
        {
            for (int k = 0; k < n; k++) {

                // 1. Обчислення рядка U
                for (int start = k; start < n; start += blockSize) {

                    int end = std::min(start + blockSize, n);

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

                #pragma omp taskwait

                L[k][k] = 1.0;

                // 2. Обчислення стовпця L
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

                #pragma omp taskwait
            }
        }
    }
}