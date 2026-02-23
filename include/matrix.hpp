#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>

typedef std::vector<std::vector<double>> Matrix;

void fillRandom(Matrix &A, int n, double minVal, double maxVal);
void fillByFunction(Matrix &A, int n);
bool verifyLU(const Matrix &A, const Matrix &L, const Matrix &U, int n);

#endif