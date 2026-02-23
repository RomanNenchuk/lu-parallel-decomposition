#include "../include/matrix.hpp"

// Послідовний LU-розклад (еталон для вимірювання прискорення)
void luSequential(const Matrix &A, Matrix &L, Matrix &U, int n) {
    for (int k = 0; k < n; k++) {
        // Рахуємо рядок U
        for (int i = k; i < n; i++) {
            double sum = 0;
            for (int j = 0; j < k; j++) sum += L[k][j] * U[j][i];
            U[k][i] = A[k][i] - sum;
        }
        
        L[k][k] = 1.0;

        // Рахуємо стовпець L
        for (int i = k + 1; i < n; i++) {
            double sum = 0;
            for (int j = 0; j < k; j++) sum += L[i][j] * U[j][k];
            L[i][k] = (A[i][k] - sum) / U[k][k];
        }
    }
}