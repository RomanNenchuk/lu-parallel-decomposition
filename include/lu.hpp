#ifndef LU_HPP
#define LU_HPP

#include "matrix.hpp"

void luSequential(const Matrix &A, Matrix &L, Matrix &U, int n);
void luParallelFor(const Matrix &A, Matrix &L, Matrix &U, int n);
void luParallelTask(const Matrix &A, Matrix &L, Matrix &U, int n);

#endif