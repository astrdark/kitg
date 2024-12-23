#include <vector>
#include <algorithm>
#include <ranges>
#include <print>

using u32 = uint32_t;
using i32 = int32_t;
using usize = size_t;

struct DSU {
    explicit DSU(usize len) {
        data.resize(len, 0);
        for (u32 i : std::views::iota(0u, u32(len))) {
            data[i] = i;
        }
    }

    u32 find(this const DSU& self, u32 a) {
        u32 root = a;
        while (root != self.data[root])
            root = self.data[root];

        return root;
    }

    void set(this DSU& self, u32 root, u32 child) {
        self.data[child] = root;
    }
private:
    std::vector<u32> data;
};

struct Edge {
    u32 vtx_a;
    u32 vtx_b;
    usize weight;
};

using TGraph = std::vector<Edge>;

TGraph kruskal(TGraph in) {
    TGraph out;
    out.reserve(in.size());
    DSU dsu(in.size());

    std::ranges::sort(
        in,
        [] (const Edge& a, const Edge& b) { return a.weight < b.weight; }
    );

    for (const Edge& edge : in) {
        const auto a_set = dsu.find(edge.vtx_a);
        const auto b_set = dsu.find(edge.vtx_b);

        if (a_set != b_set) {
            out.emplace_back(edge);
            dsu.set(a_set, b_set);
        }
    }

    return out;
}

int main() {
    TGraph input = {
        {  1,  2,    1  },
        {  1,  3,    8  },
        {  2,  3,    5  },
        {  2,  4,  100  },
        {  3,  5,   25  },
        {  4,  5,    6  },
        {  4,  6,   53  },
        {  5,  6,   11  }
    };

    TGraph out = kruskal(std::move(input));
    for (auto[a,b,w] : out) {
        std::println("({}, {}) = {}", a, b, w);
    }
}