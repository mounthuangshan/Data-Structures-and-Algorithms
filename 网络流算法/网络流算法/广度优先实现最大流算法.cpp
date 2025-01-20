#include <vector>
#include <list>
#include <memory>
#include <iostream>

// 边的信息结构体
struct EdgeInfo {
    int start;  // 起点
    int end;    // 终点
    int capacity; // 容量
    int flow;    // 当前流量

    EdgeInfo(int s, int e, int c, int f = 0)
        : start(s), end(e), capacity(c), flow(f) {
    }
};

// 顶点信息结构体
struct VertexInfo {
    int previous;  // 前驱节点
    bool forward;  // 是否是前向边

    VertexInfo(int prev = -1, bool fwd = true)
        : previous(prev), forward(fwd) {
    }
};

// 顶点的边结构类
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

        // 使用双向链表存储路径
        std::list<int> path;
        path.push_back(sourceIndex);

        // 当路径不为空时继续搜索
        while (!path.empty()) {
            int u = path.front();
            path.pop_front();

            // 处理前向边
            for (auto it = network[u].forward(); it != network[u].forward_end(); ++it) {
                int v = it->end;

                // 如果顶点未访问且有剩余容量，准备增加流
                if (!vertices[v] && it->capacity > it->flow) {
                    vertices[v] = std::make_unique<VertexInfo>(u, FORWARD);
                    if (v == sinkIndex) {
                        return true;  // 找到增广路
                    }
                    path.push_back(v);  // 加入路径尾部
                }
            }

            // 处理后向边
            for (auto it = network[u].backward(); it != network[u].backward_end(); ++it) {
                int v = it->start;

                // 如果顶点未访问且可以减少流
                if (!vertices[v] && it->flow > 0) {
                    vertices[v] = std::make_unique<VertexInfo>(u, BACKWARD);
                    path.push_back(v);  // 加入路径尾部
                }
            }
        }

        return false;  // 未找到增广路
    }
};

int main() {
    // 创建示例网络
    const int n = 4;  // 顶点数量
    std::vector<VertexStructure> network(n);

    // 添加边
    // 示例：添加一些边到网络中
    network[0].addForwardEdge(EdgeInfo(0, 1, 10));
    network[0].addForwardEdge(EdgeInfo(0, 2, 8));
    network[1].addForwardEdge(EdgeInfo(1, 2, 5));
    network[1].addForwardEdge(EdgeInfo(1, 3, 7));
    network[2].addForwardEdge(EdgeInfo(2, 3, 10));

    // 创建顶点信息数组
    std::vector<std::unique_ptr<VertexInfo>> vertices(n);

    // 创建路径查找器
    PathFinder pathFinder(network, 0, 3);  // 源点为0，汇点为3

    // 查找增广路
    bool found = pathFinder.findAugmentingPath(vertices);

    // 输出结果
    if (found) {
        std::cout << "找到增广路！" << std::endl;

        // 打印路径
        int v = 3;  // 从汇点开始
        std::cout << "路径: ";
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