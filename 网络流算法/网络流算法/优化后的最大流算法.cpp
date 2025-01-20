#include <vector>
#include <climits>
#include <queue>
#include <iostream>

// 基类的前向声明
class FlowNetwork {
protected:
    int numVertices;
public:
    FlowNetwork(int n, int s, int t) : numVertices(n) {}
};

// 边的信息结构体
struct EdgeInfo {
    int start;
    int end;
    int capacity;
    EdgeInfo(int s, int e, int c) : start(s), end(e), capacity(c) {}
};

class Optimized : public FlowNetwork {
private:
    std::vector<std::vector<int>> capacity;  // 容量矩阵
    std::vector<std::vector<int>> flow;      // 流量矩阵
    std::vector<int> previous;               // 前驱节点信息
    std::vector<int> visited;                // 访问状态
    const int QUEUE_SIZE;                    // 队列的最大大小
    std::vector<int> queue;                  // 循环队列

protected:
    int processPath(int source, int sink) {
        // 计算可以增加的流量
        int increment = INT_MAX;
        int v = sink;

        while (previous[v] != -1) {
            int unit = capacity[previous[v]][v] - flow[previous[v]][v];
            if (unit < increment) {
                increment = unit;
            }
            v = previous[v];
        }

        // 增加流量
        v = sink;
        while (previous[v] != -1) {
            flow[previous[v]][v] += increment;    // 正向边
            flow[v][previous[v]] -= increment;    // 反向边
            v = previous[v];
        }

        return increment;
    }

public:
    // 构造函数
    Optimized(int n, int s, int t, const std::vector<EdgeInfo>& edges)
        : FlowNetwork(n, s, t),
        QUEUE_SIZE(n),
        capacity(n, std::vector<int>(n, 0)),
        flow(n, std::vector<int>(n, 0)),
        previous(n, -1),
        visited(n, 0),
        queue(n, 0) {

        // 初始化流量为0并设置容量
        for (const auto& ei : edges) {
            capacity[ei.start][ei.end] = ei.capacity;
        }
    }

    int compute(int source, int sink) {
        int maxFlow = 0;
        while (search(source, sink)) {
            maxFlow += processPath(source, sink);
        }
        return maxFlow;
    }

    bool search(int source, int sink) {
        // 清除访问状态：0-未访问，1-在队列中，2-已访问
        std::fill(visited.begin(), visited.end(), 0);

        // 创建循环队列
        queue[0] = source;
        int head = 0, tail = 1;
        previous[source] = -1;
        visited[source] = 1;

        while (head != tail) {
            int u = queue[head];
            head = (head + 1) % QUEUE_SIZE;
            visited[u] = 2;

            // 将具有剩余容量的相邻节点加入队列
            for (int v = 0; v < numVertices; v++) {
                if (visited[v] == 0 && capacity[u][v] > flow[u][v]) {
                    queue[tail] = v;
                    tail = (tail + 1) % QUEUE_SIZE;
                    visited[v] = 1;  // 标记为在队列中
                    previous[v] = u;
                }
            }
        }

        return visited[sink] != 0;  // 是否访问了汇点？
    }
};

int main() {
    // 示例用法
    const int n = 6;  // 顶点数量
    const int source = 0;
    const int sink = 5;

    // 创建示例边及其容量
    std::vector<EdgeInfo> edges = {
        EdgeInfo(0, 1, 10),
        EdgeInfo(0, 2, 8),
        EdgeInfo(1, 2, 5),
        EdgeInfo(1, 3, 5),
        EdgeInfo(2, 4, 10),
        EdgeInfo(3, 4, 7),
        EdgeInfo(3, 5, 8),
        EdgeInfo(4, 5, 10)
    };

    // 创建优化的流网络
    Optimized network(n, source, sink, edges);

    // 计算最大流
    int maxFlow = network.compute(source, sink);

    std::cout << "从源点 " << source
        << " 到汇点 " << sink << " 的最大流: "
        << maxFlow << std::endl;

    return 0;
}