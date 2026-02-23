#include "../include/io.hpp"
#include <fstream>

bool readMatrixFromFile(Matrix &A, int &n, const std::string &filename) {
    std::ifstream in(filename);
    if (!in.is_open()) return false;

    in >> n;
    A.assign(n, std::vector<double>(n));

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            in >> A[i][j];

    in.close();
    return true;
}

void writeResults(const std::string &filename,
                  int n,
                  int threads,
                  double tSeq,
                  double tFor,
                  double tTask,
                  double speedupFor,
                  double speedupTask) {

    std::ofstream out(filename, std::ios::app);

    out << n << "\t"
        << threads << "\t"
        << tSeq << "\t"
        << tFor << "\t"
        << tTask << "\t"
        << speedupFor << "\t"
        << speedupTask << "\n";

    out.close();
}