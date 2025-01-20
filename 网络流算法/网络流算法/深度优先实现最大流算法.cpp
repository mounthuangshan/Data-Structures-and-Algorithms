#include <vector>
#include <stack>
#include <memory>
#include <iostream>

// 边信息结构体
struct EdgeInfo {
    int start;    // 起点
    int end;      // 终点
    int capacity; // 容量
    int flow;     // 当前流量

    EdgeInfo(int s, int e, int c, int f = 0)
        : start(s), end(e), capacity(c), flow(f) {
    }
};

// 顶点信息结构体
struct VertexInfo {
    int previous;  // 前驱节点
    bool forward;  // 是否为前向边

    VertexInfo(int prev = -1, bool fwd = true)
        : previous(prev), forward(fwd) {
    }
};

// 顶点边结构类
class VertexStructure {
private:
    std::vector<EdgeInfo> forwardEdges;   // 前向边列表
    std::vector<EdgeInfo> backwardEdges;  // 后向边列表

public:
    // 获取前向边迭代器
    auto forward() { return forwardEdges.begin(); }
    auto forward_end() { return forwardEdges.end(); }

    // 获取后向边迭代器
    auto backward() { return backwardEdges.begin(); }
    auto backward_end() { return backwardEdges.end(); }

    // 添加边
    void addForwardEdge(const EdgeInfo& edge) {
        forwardEdges.push_back(edge);
    }

    void addBackwardEdge(const EdgeInfo& edge) {
        backwardEdges.push_back(edge);
    }
};

class PathFinder {
private:
    std::vector<VertexStructure>& network;  // 网络结构
    int sourceIndex;  // 源点索引
    int sinkIndex;   // 汇点索引
    const bool FORWARD = true;    // 前向边标志
    const bool BACKWARD = false;  // 后向边标志

public:
    PathFinder(std::vector<VertexStructure>& net, int source, int sink)
        : network(net), sourceIndex(source), sinkIndex(sink) {
    }

    bool findAugmentingPath(std::vector<std::unique_ptr<VertexInfo>>& vertices) {
        // 初始化源点信息
        vertices[sourceIndex] = std::make_unique<VertexInfo>(-1);

        // 使用栈存储路径
        std::stack<int> path;
        path.push(sourceIndex);

        // 当栈不为空时继续搜索
        while (!path.empty()) {
            int u = path.top();
            path.pop();

            // 处理前向边
            for (auto it = network[u].forward(); it != network[u].forward_end(); ++it) {
                int v = it->end;

                // 如果顶点未访问且有剩余容量，准备增加流
                if (!vertices[v] && it->capacity > it->flow) {
                    vertices[v] = std::make_unique<VertexInfo>(u, FORWARD);
                    if (v == sinkIndex) {
                        return true;  // 找到增广路
                    }
                    path.push(v);
                }
            }

            // 处理后向边
            for (auto it = network[u].backward(); it != network[u].backward_end(); ++it) {
                int v = it->start;

                // 如果顶点未访问且可以减少流
                if (!vertices[v] && it->flow > 0) {
                    vertices[v] = std::make_unique<VertexInfo>(u, BACKWARD);
                    path.push(v);
                }
            }
        }

        return false;  // 未找到增广路
    }
};

int main() {
    // 创建示例网络
    const int n = 6;  // 顶点数量
    std::vector<VertexStructure> network(n);

    // 添加示例边
    // 向前向边列表添加边
    network[0].addForwardEdge(EdgeInfo(0, 1, 10));
    network[0].addForwardEdge(EdgeInfo(0, 2, 8));
    network[1].addForwardEdge(EdgeInfo(1, 2, 5));
    network[1].addForwardEdge(EdgeInfo(1, 3, 5));
    network[2].addForwardEdge(EdgeInfo(2, 4, 10));
    network[3].addForwardEdge(EdgeInfo(3, 4, 7));
    network[3].addForwardEdge(EdgeInfo(3, 5, 8));
    network[4].addForwardEdge(EdgeInfo(4, 5, 10));

    // 向后向边列表添加对应的反向边
    network[1].addBackwardEdge(EdgeInfo(1, 0, 10));
    network[2].addBackwardEdge(EdgeInfo(2, 0, 8));
    network[2].addBackwardEdge(EdgeInfo(2, 1, 5));
    network[3].addBackwardEdge(EdgeInfo(3, 1, 5));
    network[4].addBackwardEdge(EdgeInfo(4, 2, 10));
    network[4].addBackwardEdge(EdgeInfo(4, 3, 7));
    network[5].addBackwardEdge(EdgeInfo(5, 3, 8));
    network[5].addBackwardEdge(EdgeInfo(5, 4, 10));

    // 创建顶点信息数组
    std::vector<std::unique_ptr<VertexInfo>> vertices(n);

    // 创建路径查找器（源点为0，汇点为5）
    PathFinder pathFinder(network, 0, 5);

    // 查找增广路并输出结果
    if (pathFinder.findAugmentingPath(vertices)) {
        std::cout << "找到增广路！" << std::endl;

        // 打印增广路径
        std::cout << "增广路径: ";
        int v = 5;  // 从汇点开始
        while (v != -1) {
            std::cout << v;
            if (vertices[v] && vertices[v]->previous != -1) {
                std::cout << " <- ";
            }
            v = vertices[v] ? vertices[v]->previous : -1;
        }
        std::cout << std::endl;
    }
    else {
        std::cout << "未找到增广路。" << std::endl;
    }

    return 0;
}