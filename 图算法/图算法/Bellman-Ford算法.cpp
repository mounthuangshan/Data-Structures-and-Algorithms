#include <iostream>
#include <vector>
#include <limits>
#include <list>
#include <utility> // for std::pair
#include <stdexcept>

// 定义一个邻接列表的类型
using VertexList = std::vector<std::list<std::pair<int, int>>>; // pair<邻接顶点, 权重>

// Graph 类定义
class Graph {
private:
    VertexList adjList; // 邻接列表

public:
    // 构造函数
    Graph(int numVertices) : adjList(numVertices) {}

    // 添加边的方法
    void addEdge(int u, int v, int weight) {
        adjList[u].emplace_back(v, weight); // 添加边 (u, v) 及其权重
    }

    // 获取顶点数量
    int numVertices() const {
        return adjList.size();
    }

    // 返回指定顶点的邻接列表的迭代器
    auto begin(int u) const {
        return adjList[u].begin();
    }

    auto end(int u) const {
        return adjList[u].end();
    }
};

// 单源最短路径算法（Bellman-Ford算法实现）
void singleSourceShortest(Graph const& graph, int s, std::vector<int>& dist, std::vector<int>& pred) {
    const int n = graph.numVertices();
    pred.assign(n, -1);
    dist.assign(n, std::numeric_limits<int>::max());
    dist[s] = 0;

    for (int i = 1; i <= n; i++) {
        bool failOnUpdate = (i == n);
        bool leaveEarly = true;

        for (int u = 0; u < n; u++) {
            for (auto ci = graph.begin(u); ci != graph.end(u); ++ci) {
                int v = ci->first;
                long newLen = dist[u] + ci->second; // 新路径长度
                if (newLen < dist[v]) {
                    if (failOnUpdate) {
                        throw "Graph has negative cycle";
                    }
                    dist[v] = newLen;
                    pred[v] = u;
                    leaveEarly = false;
                }
            }
        }
        if (leaveEarly) {
            break;
        }
    }
}

// 主函数
int main() {
    // 创建图实例，假设有5个顶点
    Graph graph(5);

    // 添加一些边 (u, v, weight)
    graph.addEdge(0, 1, 6);
    graph.addEdge(0, 2, 7);
    graph.addEdge(1, 2, 8);
    graph.addEdge(1, 3, 5);
    graph.addEdge(1, 4, -4);
    graph.addEdge(2, 3, -3);
    graph.addEdge(3, 4, 9);
    graph.addEdge(4, 1, 2);

    int sourceVertex = 0; // 起始顶点
    std::vector<int> dist; // 存储最短距离
    std::vector<int> pred; // 存储前向节点

    try {
        singleSourceShortest(graph, sourceVertex, dist, pred);

        // 输出结果
        for (int i = 0; i < graph.numVertices(); ++i) {
            if (dist[i] == std::numeric_limits<int>::max()) {
                std::cout << "Vertex " << i << " is not reachable from source " << sourceVertex << ".\n";
            }
            else {
                std::cout << "Distance from source " << sourceVertex << " to vertex " << i << " is " << dist[i] << ".\n";
                std::cout << "Path: ";
                for (int v = i; v != -1; v = pred[v]) {
                    std::cout << v << " ";
                }
                std::cout << std::endl;
            }
        }
    }
    catch (const char* msg) {
        std::cerr << msg << std::endl;
    }

    return 0;
}

