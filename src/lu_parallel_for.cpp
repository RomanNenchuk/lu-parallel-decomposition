#include "../include/matrix.hpp"

// Реалізація LU-розкладу з використанням паралельних циклів (Fork-Join модель)
void luParallelFor(const Matrix &A, Matrix &L, Matrix &U, int n) {
    // Зовнішній цикл k не можна паралелізувати, оскільки кожна наступна 
    // ітерація k+1 залежить від результатів поточної ітерації k
    for (int k = 0; k < n; k++) {
        
        // Паралельно обчислюємо рядок матриці U
        #pragma omp parallel for
        for (int i = k; i < n; i++) {
            double sum = 0;
            // Обчислення суми добутків L[k][j] * U[j][i] для поточної комірки
            for (int j = 0; j < k; j++) sum += L[k][j] * U[j][i];
            U[k][i] = A[k][i] - sum;
        }
        
        // Діагональні елементи матриці L в алгоритмі завжди 1.0
        L[k][k] = 1.0;

        // Паралельно обчислюємо стовпець матриці L
        #pragma omp parallel for
        for (int i = k + 1; i < n; i++) {
            double sum = 0;
            for (int j = 0; j < k; j++) sum += L[i][j] * U[j][k];
            // Значення ділиться на діагональний елемент U для нормалізації
            L[i][k] = (A[i][k] - sum) / U[k][k];
        }
    }
}