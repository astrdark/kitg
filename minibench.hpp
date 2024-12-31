#include <cstdint>
#include <chrono>
#include <ranges>

struct MiniBenchmark {
    template<typename Fn>
    static float benchmark(Fn&& fn) {
        constexpr size_t NUM_ITERATIONS = 16;

        const auto start = std::chrono::high_resolution_clock::now();
        for (size_t i : std::views::iota(0u, NUM_ITERATIONS)) {
            fn();
        }
        const auto end = std::chrono::high_resolution_clock::now();

        const float count = float(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
        return count / float(NUM_ITERATIONS);
    }
};