#ifndef IO_HPP
#define IO_HPP

#include "matrix.hpp"
#include <string>

bool readMatrixFromFile(Matrix &A, int &n, const std::string &filename);
void writeResults(const std::string &filename,
                  int n,
                  int threads,
                  double tSeq,
                  double tFor,
                  double tTask,
                  double speedupFor,
                  double speedupTask);

#endif