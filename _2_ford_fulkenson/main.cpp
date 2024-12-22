#include <vector>
#include <algorithm>
#include <ranges>
#include <unordered_map>
#include <print>

using u32 = uint32_t;
using i32 = int32_t;
using usize = size_t;

struct Edge {
    u32 vtx_a;
    u32 vtx_b;

    i32 fwd = 0;
    i32 rev = 0;

    i32& flow_towards(u32 vtx) {
        if (vtx_a == vtx) { // b -> a
            return rev;
        } else if (vtx_b == vtx) { // a -> b
            return fwd;
        }

        __builtin_unreachable();
    }

    i32& flow_from(u32 vtx) {
        if (vtx_a == vtx) { // a -> b
            return fwd;
        } else if (vtx_b == vtx) { // b -> a
            return rev;
        }

        __builtin_unreachable();
    }

    u32 vtx_not(u32 vtx) {
        if (vtx_a == vtx) {
            return vtx_b;
        }
        if (vtx_b == vtx) {
            return vtx_a;
        }

        __builtin_unreachable();
    }
};

struct Graph {
    std::vector<Edge> graph;
    std::unordered_map<u32, u32> mark;
    u32 source;
    u32 drain;

    u32& mark_for(u32 vtx) {
        if (!mark.contains(vtx)) {
            mark.insert({vtx, 0u});
        }

        return mark[vtx];
    }

    auto iter_neighbours(u32 current_vtx) {
        auto pred = [current_vtx](Edge& e) {
            return e.vtx_a == current_vtx || e.vtx_b == current_vtx;
        };

        return graph | std::views::filter(pred);
    }

    void add_edge(u32 a, u32 b, i32 capacity) {
        graph.push_back(Edge { .vtx_a = a, .vtx_b = b, .fwd = capacity, .rev = 0 });
    }
};

u32 search(Graph& graph, u32 cur_vtx, i32 min_flow, u32 mark) {
    if (cur_vtx == graph.drain) {
        return min_flow;
    }

    graph.mark_for(cur_vtx) = mark;

    for (Edge& e : graph.iter_neighbours(cur_vtx)) {
        if (e.flow_from(cur_vtx) == 0) {
            continue;
        }

        const u32 the_vtx = e.vtx_not(cur_vtx);
        if (graph.mark_for(the_vtx) == mark) {
            continue;
        }

        i32 val = search(graph,
            the_vtx,
            std::min(min_flow, e.flow_from(cur_vtx)), mark
        );
        if (val != 0) {
            e.flow_from(the_vtx) += val;
            e.flow_towards(the_vtx) -= val;
            return val;
        }
    }

    return 0;
}

u32 ford_fulkenson(Graph graph) {
    constexpr i32 INF = 0x7FFFFFFF;

    u32 mark = 1;
    u32 rez = 0;
    u32 sum = 0;
    do {
        rez = search(graph, graph.source, INF, mark++);
        sum += rez;
        std::println("rez = {}", rez);
    } while (rez != 0);

    return sum;
}

int main() {
    Graph graph;
    graph.add_edge(0, 1, 100);
    graph.add_edge(0, 2, 1000);
    graph.add_edge(1, 3, 150);
    graph.add_edge(2, 3, 100);
    graph.add_edge(2, 4, 500);
    graph.add_edge(3, 4, 250);

    graph.source = 0;
    graph.drain = 4;

    u32 result = ford_fulkenson(graph);
    std::println("res = {}", result);
}
