#include <iostream>
#include <fstream>
#include <string>
#include "solver.hpp"

#define NUM_TESTS 100
#define MAX_DEPTH 8

struct test_result {
    double time;
    size_t memory;
    test_result(double t, size_t h) : time(t), memory(h) {};
};

class Benchmark {
  private:
    size_t get_current_rss();
    int current_test = 0;
  public:
    void select_test();
    void run();
    
    template <typename Func>
    void single_function_test(std::string name, Func func);

    template <typename Func>
    test_result benchmark(Func func);

    template <typename Func>
    void test_IDAstar(Func func);
};

/// @brief Выполняет один тест функции
/// @param name Название функции
/// @param func Тестируемая функция
template <typename Func>
void Benchmark::single_function_test(std::string name, Func func) {
    SCube cube(Cube(3));

    std:: cout << "\ntesting " << name << std::endl << std::endl;

    std::vector<double> times;
    std::vector<size_t> memories;
    test_result res(0, 0);

    for (int depth = 1; depth <= MAX_DEPTH; depth++) {
        std::cout << "\ndepth: " << depth << std::endl;
        
        times.clear();
        memories.clear();

        for (int i = 0; i < NUM_TESTS; i++) {
            SCube work_cube = cube;
            work_cube.scramble(depth, true);

            res = benchmark([work_cube, &func]() {
                func(work_cube);
            });

            times.push_back(res.time);
            memories.push_back(res.memory);
        }


        auto [min_time, max_time] = std::minmax_element(times.begin(), times.end());
        auto [min_mem, max_mem] = std::minmax_element(memories.begin(), memories.end());

        double sum_time = std::accumulate(times.begin(), times.end(), 0.0);
        size_t sum_mem = std::accumulate(memories.begin(), memories.end(), size_t(0));

        std::cout << "Time (sec):\nmin = " << *min_time
                  << "; avg = " << sum_time / times.size()
                  << "; max = " << *max_time << std::endl;

        std::cout << "Memory (KB):\nmin = " << *min_mem
                  << "; avg = " << sum_mem / memories.size()
                  << "; max = " << *max_mem << std::endl;
    }
}

/// @brief Тестирует IDA*
template <typename Func>
void Benchmark::test_IDAstar(Func func) {
    SCube cube(Cube(3));

    std:: cout << "\ntesting IDA*"  << std::endl << std::endl;

    test_result res(0, 0);

    int depth = 0;
    while (1) {
        std::cout << "\nenter depth: ";
        std::cin >> depth;
        SCube work_cube = cube;
        work_cube.scramble(depth, true);

        res = benchmark([work_cube, &func]() {
            func(work_cube);
        });

        std::cout << "Time (sec): " << res.time << std::endl;
        std::cout << "Memory (KB): " << res.memory << std::endl;
    }
}

/// @brief Замеряет время и используемую память при выполнении функции
/// @param func Исполняемая функция 
/// @return Время и используемую память
template <typename Func>
test_result Benchmark::benchmark(Func func) {
    size_t rss_before = get_current_rss();
    auto start = std::chrono::high_resolution_clock::now();

    func();

    auto end = std::chrono::high_resolution_clock::now();
    size_t rss_after = get_current_rss();

    std::chrono::duration<double> elapsed = end - start;

    return test_result(elapsed.count(), rss_after);
}
