#include <iostream>
#include <vector>
#include <algorithm> // for sort
#include <tuple>    // for std::tuple

using namespace std;

struct Edge {
    int src, dest, weight;
    Edge(int s, int d, int w) : src(s), dest(d), weight(w) {}
};

// 比较函数，用于按边权重排序
bool compareEdges(const Edge& a, const Edge& b) {
    return a.weight < b.weight;
}

class DisjointSet {
public:
    DisjointSet(int n) : parent(n), rank(n, 0) {
        for (int i = 0; i < n; ++i) {
            parent[i] = i; // 每个节点初始为自己父亲
        }
    }

    int find(int u) {
        if (parent[u] != u) {
            parent[u] = find(parent[u]); // 路径压缩
        }
        return parent[u];
    }

    void unionSets(int u, int v) {
        int rootU = find(u);
        int rootV = find(v);
        if (rootU != rootV) {
            // 按秩合并
            if (rank[rootU] < rank[rootV]) {
                parent[rootU] = rootV;
            }
            else if (rank[rootU] > rank[rootV]) {
                parent[rootV] = rootU;
            }
            else {
                parent[rootV] = rootU;
                rank[rootU]++;
            }
        }
    }

private:
    vector<int> parent; // 存储父节点
    vector<int> rank;   // 存储树的秩
};

class Graph {
public:
    Graph(int vertices) : V(vertices) {}

    void addEdge(int src, int dest, int weight) {
        edges.emplace_back(src, dest, weight);
    }

    void kruskalMST() {
        // 1. 按边的权重排序
        sort(edges.begin(), edges.end(), compareEdges);

        DisjointSet ds(V);
        vector<Edge> mst; // 存储最小生成树的边

        // 2. 遍历所有边，选择权重最小且不形成环的边
        for (const auto& edge : edges) {
            int u = edge.src;
            int v = edge.dest;
            if (ds.find(u) != ds.find(v)) {
                ds.unionSets(u, v); // 合并两个集合
                mst.push_back(edge); // 加入到最小生成树
            }
        }

        // 打印最小生成树
        cout << "最小生成树的边：\n";
        for (const auto& edge : mst) {
            cout << edge.src << " -- " << edge.dest << " (权重: " << edge.weight << ")\n";
        }
    }

private:
    int V; // 顶点数
    vector<Edge> edges; // 边的集合
};

int main() {
    Graph graph(5); // 创建一个包含 5 个顶点的图

    // 添加边
    graph.addEdge(0, 1, 10);
    graph.addEdge(0, 2, 6);
    graph.addEdge(0, 3, 5);
    graph.addEdge(1, 3, 15);
    graph.addEdge(2, 3, 4);

    // 调用 Kruskal 算法
    graph.kruskalMST();

    return 0;
}
