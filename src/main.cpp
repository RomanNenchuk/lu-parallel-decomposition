#include "../include/matrix.hpp"
#include "../include/lu.hpp"
#include "../include/io.hpp"

#include <iostream>
#include <fstream>
#include <omp.h>
#include <ctime>
#include <vector>

using namespace std;


int main() {
    srand(time(0));
    int choice;
    
    cout << "--- LU Decomposition Benchmarking ---\n";
    cout << "1. Випадкова генерація (200, 500, 800)\n";
    cout << "2. Зчитування з файлу (одиничний тест)\n";
    cout << "3. Генерація за функцією (200, 500, 800)\n";
    cout << "Ваш вибір: ";
    cin >> choice;

    vector<int> sizes = {200, 500, 800};
    Matrix fileA; 
    int fileN;
    bool isFileMode = (choice == 2);

    // Якщо обрано файл, зчитуємо його один раз до початку циклів
    if (isFileMode) {
        if (!readMatrixFromFile(fileA, fileN, "data/input.txt")) {
            cerr << "Помилка: файл input.txt не знайдено!\n";
            return 1;
        }
    }

    int threadCounts[] = {1, 2, 4, 8};
    string resultFile = "results/results.txt";
    ofstream clear(resultFile);
    clear << "Size\tThreads\tSeq\tFor\tTask\tS_For\tS_Task\n";
    clear.close();

    // Головний цикл: якщо файл - 1 ітерація, інакше - по вектору sizes
    int totalSizes = isFileMode ? 1 : sizes.size();

    for (int i = 0; i < totalSizes; i++) {
        int n;
        Matrix A;

        // Визначаємо матрицю А для поточного прогону
        if (choice == 1) {
            n = sizes[i];
            fillRandom(A, n, 1, 10);
        } else if (choice == 2) {
            n = fileN;
            A = fileA;
        } else {
            n = sizes[i];
            fillByFunction(A, n);
        }

        cout << "\n>>> Тестування для n = " << n << " <<<\n";

        for (int threads : threadCounts) {
            omp_set_num_threads(threads);

            // Резервуємо пам'ять під L та U
            Matrix L(n, vector<double>(n, 0));
            Matrix U(n, vector<double>(n, 0));

            // 1. Sequential
            double start = omp_get_wtime();
            luSequential(A, L, U, n);
            double tSeq = omp_get_wtime() - start;

            // 2. Parallel For
            L.assign(n, vector<double>(n, 0));
            U.assign(n, vector<double>(n, 0));
            start = omp_get_wtime();
            luParallelFor(A, L, U, n);
            double tFor = omp_get_wtime() - start;

            // 3. Parallel Task
            L.assign(n, vector<double>(n, 0));
            U.assign(n, vector<double>(n, 0));
            start = omp_get_wtime();
            luParallelTask(A, L, U, n);
            double tTask = omp_get_wtime() - start;

            double speedupFor = tSeq / tFor;
            double speedupTask = tSeq / tTask;

            writeResults(resultFile, n, threads, tSeq, tFor, tTask, speedupFor, speedupTask);

            printf("Threads: %d | Seq: %.4fs | For: %.4fs (x%.2f) | Task: %.4fs (x%.2f)\n", 
                   threads, tSeq, tFor, speedupFor, tTask, speedupTask);
        }
    }

    return 0;
}