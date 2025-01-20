#include <vector>
#include <unordered_map>
#include <iterator>
#include <string>
#include <memory>
#include <iostream>
#include <algorithm>
#include <cstring>

// Edge information structure
struct EdgeInfo {
    int start;
    int end;
    int capacity;
    int flow;
    int reverseIndex;

    EdgeInfo(int s, int e, int c, int rev) : start(s), end(e), capacity(c), flow(0), reverseIndex(rev) {}

    int getFlow() const { return flow; }
};

// Pair structure for results
struct Pair {
    std::string first;
    std::string second;

    Pair(const std::string& f, const std::string& s) : first(f), second(s) {}
};

// FlowNetworkArray class
class FlowNetworkArray {
public:
    int numVertices;
    int src;
    int tgt;
    std::vector<std::vector<EdgeInfo>> adj;

    FlowNetworkArray(int n, int source, int target, const std::vector<EdgeInfo>& edges)
        : numVertices(n), src(source), tgt(target), adj(n, std::vector<EdgeInfo>()) {
        for (const auto& edge : edges) {
            adj[edge.start].emplace_back(edge.start, edge.end, edge.capacity, adj[edge.end].size());
            adj[edge.end].emplace_back(edge.end, edge.start, 0, adj[edge.start].size() - 1);
        }
    }

    void addEdge(int from, int to, int capacity) {
        adj[from].emplace_back(from, to, capacity, adj[to].size());
        adj[to].emplace_back(to, from, 0, adj[from].size() - 1);
    }
};

// DFS_SearchArray class
class DFS_SearchArray {
public:
    FlowNetworkArray* network;
    std::vector<bool> visited;

    DFS_SearchArray(FlowNetworkArray* net) : network(net), visited(net->numVertices, false) {}

    bool dfs(int u, int t, std::vector<int>& parent) {
        std::fill(visited.begin(), visited.end(), false);
        std::vector<int> stack;
        stack.push_back(u);
        visited[u] = true;

        while (!stack.empty()) {
            int current = stack.back();
            stack.pop_back();

            for (size_t i = 0; i < network->adj[current].size(); ++i) {
                EdgeInfo& edge = network->adj[current][i];
                if (!visited[edge.end] && edge.flow < edge.capacity) {
                    stack.push_back(edge.end);
                    visited[edge.end] = true;
                    parent[edge.end] = current;
                    if (edge.end == t) return true;
                }
            }
        }
        return false;
    }
};

// FordFulkerson class
class FordFulkerson {
public:
    FlowNetworkArray* network;
    DFS_SearchArray* search;
    int maxFlow;

    FordFulkerson(FlowNetworkArray* net, DFS_SearchArray* se) : network(net), search(se), maxFlow(0) {}

    void compute() {
        std::vector<int> parent(network->numVertices, -1);
        while (search->dfs(network->src, network->tgt, parent)) {
            int pathFlow = INT32_MAX;
            int s = network->tgt;
            while (s != network->src) {
                int p = parent[s];
                // Find the edge from p to s
                for (auto& edge : network->adj[p]) {
                    if (edge.end == s && edge.flow < edge.capacity) {
                        pathFlow = std::min(pathFlow, edge.capacity - edge.flow);
                        break;
                    }
                }
                s = p;
            }

            // Update flow
            s = network->tgt;
            while (s != network->src) {
                int p = parent[s];
                for (auto& edge : network->adj[p]) {
                    if (edge.end == s) {
                        edge.flow += pathFlow;
                        network->adj[s][edge.reverseIndex].flow -= pathFlow;
                        break;
                    }
                }
                s = p;
            }

            maxFlow += pathFlow;
        }
    }
};

// BipartiteMatching class
class BipartiteMatching {
private:
    std::vector<EdgeInfo> edges;  // Edges between S and T
    int ctr = 0;  // ID counter

    // Mappings between problem instances
    std::unordered_map<std::string, int> map;
    std::unordered_map<int, std::string> reverse;

    int srcIndex;  // Source index
    int tgtIndex;  // Target index
    int numVertices;  // Number of vertices
public:
    BipartiteMatching(const std::vector<std::string>& setS,
        const std::vector<std::string>& setT,
        const std::vector<std::pair<std::string, std::string>>& pairs) {
        // Assign indices
        srcIndex = 0;
        ctr = 1;
        for (const auto& s : setS) {
            map[s] = ctr;
            reverse[ctr] = s;
            ctr++;
        }
        for (const auto& t : setT) {
            map[t] = ctr;
            reverse[ctr] = t;
            ctr++;
        }
        tgtIndex = ctr;
        numVertices = tgtIndex + 1;

        // Initialize network edges
        for (const auto& pair : pairs) {
            int u = map[pair.first];
            int v = map[pair.second];
            edges.emplace_back(u, v, 1, 0);
        }

        // Connect source to setS
        for (const auto& s : setS) {
            edges.emplace_back(srcIndex, map[s], 1, 0);
        }

        // Connect setT to sink
        for (const auto& t : setT) {
            edges.emplace_back(map[t], tgtIndex, 1, 0);
        }
    }

    std::vector<Pair> compute() {
        auto network = std::make_unique<FlowNetworkArray>(numVertices, srcIndex, tgtIndex, edges);
        auto search = std::make_unique<DFS_SearchArray>(network.get());
        auto solver = std::make_unique<FordFulkerson>(network.get(), search.get());

        solver->compute();

        // Extract matching
        std::vector<Pair> pairs;
        for (const auto& edgeList : network->adj) {
            for (const auto& edge : edgeList) {
                if (edge.start != srcIndex && edge.end != tgtIndex && edge.capacity == 1 && edge.flow == 1) {
                    pairs.emplace_back(reverse[edge.start], reverse[edge.end]);
                }
            }
        }

        return pairs;
    }
};

int main() {
    // Example usage
    std::vector<std::string> setS = { "s1", "s2", "s3" };
    std::vector<std::string> setT = { "t1", "t2", "t3" };

    std::vector<std::pair<std::string, std::string>> pairs = {
        {"s1", "t1"},
        {"s2", "t2"},
        {"s3", "t3"},
        {"s1", "t2"}
    };

    BipartiteMatching matching(setS, setT, pairs);
    std::vector<Pair> result = matching.compute();

    // Print results
    for (const auto& pair : result) {
        std::cout << pair.first << " -> " << pair.second << std::endl;
    }

    return 0;
}