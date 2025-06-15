#include "benchmark.hpp"

void Benchmark::select_test() {
    int test = 0;
    std::cout << "Choose algorithm to benchmark \n(1): BFS\n(2): DFS\n(3): IDA*" << std::endl;
    while (test <= 0 || test > 3) {
        std::cin >> test;
        if (test <= 0 || test > 3) {
            std::cout << "Chhoose 1 (BFS) 2 (DFS) or 3 (IDA*)" << std::endl;
        }
    }
    current_test = test;
}

void Benchmark::run() {
    Solver solver;

    switch (current_test) {
        case 1:
        {
            single_function_test("BFS", [&](const SCube& cube_to_solve) {
                return solver.solve_cube_bfs(cube_to_solve);
            });
            break;
        }
        case 2:
        {
            single_function_test("DFS", [&](const SCube& cube_to_solve) {
                return solver.solve_cube_dfs(cube_to_solve);
            });
            break;
        }
        case 3:
        {
            test_IDAstar([&](const SCube& cube_to_solve) {
                return solver.solve_cube_IDAstar(cube_to_solve);
            });
            break;
        }
    }
}

size_t Benchmark::get_current_rss() {
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
