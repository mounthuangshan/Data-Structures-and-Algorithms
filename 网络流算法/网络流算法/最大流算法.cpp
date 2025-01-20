#include <vector>
#include <climits>
#include <algorithm>
#include <iostream>

// 前向声明
class FlowNetwork;
class Search;
class VertexInfo;

// 表示流网络边的结构体
struct Edge {
    int capacity;  // 容量
    int flow;      // 流量
    Edge(int cap = 0) : capacity(cap), flow(0) {}
};

class VertexInfo {
public:
    int previous;  // 前驱节点
    bool forward;  // 是否为正向边
    VertexInfo() : previous(-1), forward(true) {}
};

class FlowNetwork {
public:
    std::vector<std::vector<Edge>> edges;  // 边的矩阵
    std::vector<VertexInfo> vertices;      // 顶点信息
    int sourceIndex;                       // 源点索引
    int sinkIndex;                         // 汇点索引

    FlowNetwork(int n, int source, int sink)
        : edges(n, std::vector<Edge>(n)),
        vertices(n),
        sourceIndex(source),
        sinkIndex(sink) {
    }

    Edge& edge(int u, int v) {
        return edges[u][v];
    }
};

class Search {
public:
    virtual bool findAugmentingPath(std::vector<VertexInfo>& vertices) = 0;  // 寻找增广路径
    virtual ~Search() {}
};

class FordFulkerson {
private:
    FlowNetwork& network;  // 流网络
    Search& searchMethod;  // 搜索方法

protected:
    void processPath(std::vector<VertexInfo>& vertices) {
        int v = network.sinkIndex;
        int delta = INT_MAX;  // 寻找最小流量

        // 第一遍：找到最小的剩余容量
        while (v != network.sourceIndex) {
            int u = vertices[v].previous;
            int flow;

            if (vertices[v].forward) {
                // 使用剩余容量调整正向边
                flow = network.edge(u, v).capacity - network.edge(u, v).flow;
            }
            else {
                // 减少反向边的流量
                flow = network.edge(v, u).flow;
            }

            if (flow < delta) {
                delta = flow;
            }
            v = u;  // 回溯路径
        }

        // 第二遍：更新流量值
        v = network.sinkIndex;
        while (v != network.sourceIndex) {
            int u = vertices[v].previous;
            if (vertices[v].forward) {
                network.edge(u, v).flow += delta;
            }
            else {
                network.edge(v, u).flow -= delta;
            }
            v = u;  // 回溯路径
        }

        // 清除中间结果，为下一次迭代做准备
        std::fill(vertices.begin(), vertices.end(), VertexInfo());
    }

public:
    FordFulkerson(FlowNetwork& network, Search& method)
        : network(network), searchMethod(method) {
    }

    bool compute() {
        bool augmented = false;
        while (searchMethod.findAugmentingPath(network.vertices)) {
            processPath(network.vertices);
            augmented = true;
        }
        return augmented;
    }
};

// 用于测试的 Search 实现示例
class SimpleSearch : public Search {
public:
    bool findAugmentingPath(std::vector<VertexInfo>& vertices) override {
        // 简化的实现，仅用于演示
        // 实际使用时，应在此处实现 BFS 或 DFS
        return false;
    }
};

int main() {
    // 创建一个包含 4 个顶点的流网络
    // 源点: 0, 汇点: 3
    FlowNetwork network(4, 0, 3);

    // 设置一些边的容量
    network.edge(0, 1).capacity = 10;
    network.edge(1, 2).capacity = 5;
    network.edge(2, 3).capacity = 8;

    // 创建搜索方法
    SimpleSearch search;

    // 创建 Ford-Fulkerson 实例
    FordFulkerson ff(network, search);

    // 计算最大流
    bool success = ff.compute();

    if (success) {
        std::cout << "成功计算最大流\n";
    }
    else {
        std::cout << "未找到增广路径\n";
    }

    return 0;
}