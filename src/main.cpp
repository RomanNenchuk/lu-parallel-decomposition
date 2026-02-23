#include "../include/matrix.hpp"
#include "../include/lu.hpp"
#include "../include/io.hpp"

#include <iostream>
#include <fstream>
#include <omp.h>
#include <ctime>
#include <vector>

using namespace std;

/**
 * Головна програма для тестування продуктивності LU-розкладу.
 * Порівнює послідовний алгоритм, паралельний (omp for) та задачне моделювання (omp task).
 */
int main() {
    // Ініціалізація генератора випадкових чисел поточним часом
    srand(time(0));
    
    int choice;
    cout << "--- LU Decomposition Benchmarking ---\n";
    cout << "1. Випадкова генерація (200, 500, 800)\n";
    cout << "2. Зчитування з файлу (одиничний тест)\n";
    cout << "3. Генерація за функцією (200, 500, 800)\n";
    cout << "Ваш вибір: ";
    cin >> choice;

    // Стандартні розміри матриць для автоматичного тестування
    vector<int> sizes = {200, 500, 800};
    Matrix fileA; 
    int fileN;
    bool isFileMode = (choice == 2);

    // Сценарій 2: Робота з вхідним файлом
    if (isFileMode) {
        // Зчитуємо матрицю один раз перед початком вимірювань
        if (!readMatrixFromFile(fileA, fileN, "data/input.txt")) {
            cerr << "Помилка: файл data/input.txt не знайдено або він пошкоджений!\n";
            return 1;
        }
    }

    // Кількість потоків, які ми будемо перевіряти для кожного тесту
    int threadCounts[] = {1, 2, 4, 8};
    
    // Підготовка файлу звіту: записуємо заголовок стовпців
    string resultFile = "results/results.txt";
    ofstream clear(resultFile);
    if (clear.is_open()) {
        clear << "Size\tThreads\tSeq\tFor\tTask\tS_For\tS_Task\n";
        clear.close();
    }

    // Визначаємо кількість ітерацій по розмірах: 1 для файлу, або довжина вектора sizes
    int totalSizes = isFileMode ? 1 : sizes.size();

    // Тестування для різних розмірів матриць
    for (int i = 0; i < totalSizes; i++) {
        int n;
        Matrix A;

        // Підготовка вхідної матриці A залежно від вибору користувача
        if (choice == 1) {
            n = sizes[i];
            fillRandom(A, n, 1, 10); // Заповнення випадковими числами [1, 10]
        } else if (choice == 2) {
            n = fileN;
            A = fileA;
        } else {
            n = sizes[i];
            fillByFunction(A, n);    // Заповнення за математичною формулою sin(i+j)+5
        }

        cout << "\n>>> Тестування для n = " << n << " <<<\n";

        // Тестування для різної кількості потоків
        for (int threads : threadCounts) {
            // Встановлюємо кількість потоків для OpenMP на цей прогін
            omp_set_num_threads(threads);

            // Резервуємо пам'ять під матриці L (нижня трикутна) та U (верхня трикутна)
            // Ініціалізуємо нулями
            Matrix L(n, vector<double>(n, 0));
            Matrix U(n, vector<double>(n, 0));

            // --- 1. Послідовне обчислення (Еталон) ---
            double start = omp_get_wtime(); // Початок відліку часу
            luSequential(A, L, U, n);
            double tSeq = omp_get_wtime() - start; // Різниця в секундах

            // --- 2. Паралельне обчислення через #pragma omp for ---
            // Очищуємо L та U перед новим обчисленням
            L.assign(n, vector<double>(n, 0));
            U.assign(n, vector<double>(n, 0));
            
            start = omp_get_wtime();
            luParallelFor(A, L, U, n);
            double tFor = omp_get_wtime() - start;

            // --- 3. Паралельне обчислення через #pragma omp task ---
            L.assign(n, vector<double>(n, 0));
            U.assign(n, vector<double>(n, 0));
            
            start = omp_get_wtime();
            luParallelTask(A, L, U, n);
            double tTask = omp_get_wtime() - start;

            // Обчислення коефіцієнта прискорення (Speedup)
            // Speedup = Час_послідовний / Час_паралельний
            double speedupFor = tSeq / tFor;
            double speedupTask = tSeq / tTask;

            // Запис результатів у текстовий файл для аналізу/графіків
            writeResults(resultFile, n, threads, tSeq, tFor, tTask, speedupFor, speedupTask);

            // Виведення поточної статистики в консоль
            printf("Threads: %2d | Seq: %7.4fs | For: %7.4fs (x%.2f) | Task: %7.4fs (x%.2f)\n", 
                   threads, tSeq, tFor, speedupFor, tTask, speedupTask);
        }
    }

    cout << "\nТестування завершено. Результати збережено в " << resultFile << endl;
    return 0;
}