#include "../include/matrix.hpp"
#include <cstdlib>
#include <cmath>

void fillRandom(Matrix &A, int n, double minVal, double maxVal) {
    A.assign(n, std::vector<double>(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            A[i][j] = minVal + (double)rand() / RAND_MAX * (maxVal - minVal);
}

void fillByFunction(Matrix &A, int n) {
    A.assign(n, std::vector<double>(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            A[i][j] = sin(i + j) + 5;
}

bool verifyLU(const Matrix &A, const Matrix &L, const Matrix &U, int n) {
    double eps = 1e-6;
    for (int i = 0; i < n; i++)
        for (int k = 0; k < n; k++) {
            double sum = 0;
            for (int j = 0; j < n; j++)
                sum += L[i][j] * U[j][k];
            if (fabs(A[i][k] - sum) > eps) return false;
        }
    return true;
}