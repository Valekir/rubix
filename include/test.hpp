#pragma once
#include <iostream>
#include <fstream>
#include <functional>
#include <algorithm>
#include <numeric>
#include <chrono>
#include <vector>
#include <string>
#include <map>
#include <unistd.h>


struct test_result {
    double time;
    size_t memory;
    test_result(double t, size_t h) : time(t), memory(h) {};
};


template <typename Func>
test_result benchmark(Func func);

template <typename Func>
void single_function_test(std::string name, Func func);

void run_test(int test);
size_t get_current_rss();

