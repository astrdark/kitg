#include <iostream>
#include <print>
#include <vector>
#include <cstdint>
#include <optional>
#include <chrono>
#include <ranges>

#include "../minibench.hpp"

using i32 = int32_t;
using usize = size_t;

struct BinaryHeap {
	BinaryHeap() = default;

	template<typename Self>
	void push(this Self& self, i32 value) {
		self.data.push_back(value);
		const usize len = self.data.size();

		if (len > 1) {
			usize cur_idx = len - 1;
			usize parent;

			do {
				parent = (cur_idx - 1) / 2;

				if (self.data[parent] > self.data[cur_idx]) {
					std::swap(*&self.data[parent], *&self.data[cur_idx]);
					cur_idx = parent;
				}
				else break;
			} while (parent != 0);
		}
	}

	template<typename Self>
	std::optional<i32> pop(this Self& self) {
		if (self.data.empty()) {
			return std::nullopt;
		}

		const i32 out_val = self.data[0];
		std::swap(*&self.data[0], *&self.data[self.data.size()-1]);
		self.data.pop_back();

		const usize len = self.data.size();
		usize i = 0;
		while (i < len) {
			const usize left_idx = (2 * i) + 1;
			const usize right_idx = (2 * i) + 2;

			if ((right_idx < len) && (self.data[i] > self.data[right_idx])) {
				std::swap(*&self.data[i], *&self.data[right_idx]);
				i = right_idx;
				continue;
			}

			if ((left_idx < len) && (self.data[i] > self.data[left_idx])) {
				std::swap(*&self.data[i], *&self.data[left_idx]);
				i = left_idx;
				continue;
			}

			break;
		}
		return out_val;
	}

private:
	std::vector<i32> data;
};

struct NaiveVec {
	NaiveVec() = default;
	template<typename Self>
	void push(this Self& self, i32 value) {
		self.data.push_back(value);
	}

	template<typename Self>
	std::optional<i32> pop(this Self& self) {
		if (self.data.empty()) {
			return std::nullopt;
		}

		i32 min = std::bit_cast<i32>(0x7FFFFFFF);
		for (i32 v : self.data) {
			min = std::min(min, v);
		}

		return min;
	}

private:
	std::vector<i32> data;
};


int main() {
	constexpr usize SIZE = 1024u;

	BinaryHeap heap;
	for (usize _ : std::views::iota(0u, SIZE)) {
		heap.push(std::rand());
	}

	const float bheap_avg_pop = MiniBenchmark::benchmark([&heap] () {
		heap.pop();
	});

	NaiveVec vec;
	for (usize _ : std::views::iota(0u, SIZE)) {
		vec.push(std::rand());
	}

	const float vec_avg_pop = MiniBenchmark::benchmark([&vec] () {
		vec.pop();
	});

	std::println("Binary heap avg pop: {} us", bheap_avg_pop);
	std::println("NaiveVec avg pop: {} us", vec_avg_pop);
	std::println("binary heap is {} times faster", float(vec_avg_pop)/float(bheap_avg_pop));

	return 0;
}