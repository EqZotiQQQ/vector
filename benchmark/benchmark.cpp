#include <A5_mock.h>
#include <vector.h>

#include <benchmark/benchmark.h>

void vector_push_back_no_std(benchmark::State& state)
{
    no_std::Vector<A5> vector_a5;
    while (state.KeepRunning()) {
        for (int i = 0; i < 2000000; i++) {
            std::string arg = "ass";
            vector_a5.push_back(arg.c_str());
        }
    }
}

void vector_push_back_std(benchmark::State& state)
{
    std::vector<A5> vector_a5;
    while (state.KeepRunning()) {
        for (int i = 0; i < 2000000; i++) {
            std::string arg = "ass";
            vector_a5.push_back(arg.c_str());
        }
    }
}

void vector_emplace_back_no_std(benchmark::State& state)
{
    no_std::Vector<A5> vector_a5;
    while (state.KeepRunning()) {
        for (int i = 0; i < 2000000; i++) {
            vector_a5.emplace_back("Ass");
        }
    }
}

void vector_emplace_back_std(benchmark::State& state)
{
    std::vector<A5> vector_a5;
    while (state.KeepRunning()) {
        for (int i = 0; i < 2000000; i++) {
            vector_a5.emplace_back("Ass");
        }
    }
}



BENCHMARK(vector_push_back_no_std)->Unit(benchmark::kMicrosecond);
BENCHMARK(vector_push_back_std)->Unit(benchmark::kMicrosecond);
BENCHMARK(vector_emplace_back_no_std)->Unit(benchmark::kMicrosecond);
BENCHMARK(vector_emplace_back_std)->Unit(benchmark::kMicrosecond);

BENCHMARK_MAIN();
