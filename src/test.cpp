#include "test.hpp"
#include "solver.hpp"

#define NUM_TESTS 100
#define MAX_DEPTH 8

void run_test(int test) {
    switch (test) {
        case 1:
        {
            single_function_test("BFS", solve_cube_bfs);
            break;
        
        }
        case 2:
        {
            single_function_test("DFS", solve_cube_dfs);
            break;
        }
        case 3:
        {
            // single_function_test("IDA*", solve_cube_IDAstar);
            test_IDAstar(solve_cube_IDAstar);
            break;
        }
    }
}


// Функция для чтения VmRSS из /proc/<pid>/status
size_t get_current_rss() {
    std::ifstream ifs("/proc/self/status");
    std::string line;
    long rss = 0L;

    if (!ifs.is_open()) {
        return 0;
    }

    while (std::getline(ifs, line)) {
        if (line.rfind("VmRSS:", 0) == 0) {
            size_t colon_pos = line.find(':');
            if (colon_pos != std::string::npos) {
                try {
                    rss = std::stol(line.substr(colon_pos + 1));
                } catch (const std::exception& e) {
                    std::cerr << "Error parsing VmRSS: " << e.what() << std::endl;
                    rss = 0;
                }
            }
            break;
        }
    }
    return rss;
}


template <typename Func>
void single_function_test(std::string name, Func func) {
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

template <typename Func>
void test_IDAstar(Func func) {
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


template <typename Func>
test_result benchmark(Func func) {
    size_t rss_before = get_current_rss();
    auto start = std::chrono::high_resolution_clock::now();

    func();

    auto end = std::chrono::high_resolution_clock::now();
    size_t rss_after = get_current_rss();

    std::chrono::duration<double> elapsed = end - start;

    return test_result(elapsed.count(), rss_after);
}
