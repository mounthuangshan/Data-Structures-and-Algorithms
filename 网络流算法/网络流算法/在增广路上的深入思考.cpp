#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <iostream>

// 搜索方向枚举
enum class Search {
    FORWARD,
    BACKWARD
};

// 顶点信息结构
struct VertexInfo {
    int predecessor;
    Search direction;

    VertexInfo(int pred = -1, Search dir = Search::FORWARD)
        : predecessor(pred), direction(dir) {
    }
};

// 边信息结构
struct EdgeInfo {
    int flow;
    int capacity;
    int cost;

    EdgeInfo(int f = 0, int cap = 0, int c = 0)
        : flow(f), capacity(cap), cost(c) {
    }
};

// 二叉堆实现
class BinaryHeap {
private:
    struct HeapNode {
        int id;
        int key;
        int pos;  // 在堆中的位置

        HeapNode(int i = -1, int k = 0, int p = -1)
            : id(i), key(k), pos(p) {
        }
    };

    std::vector<HeapNode> nodes;
    std::vector<HeapNode*> heap;
    int size;

    void siftUp(int pos) {
        HeapNode* node = heap[pos];
        while (pos > 0) {
            int parent = (pos - 1) / 2;
            if (heap[parent]->key <= node->key) break;

            heap[pos] = heap[parent];
            heap[pos]->pos = pos;
            pos = parent;
        }
        heap[pos] = node;
        node->pos = pos;
    }

    void siftDown(int pos) {
        HeapNode* node = heap[pos];
        while (true) {
            int smallest = pos;
            int left = 2 * pos + 1;
            int right = 2 * pos + 2;

            if (left < size && heap[left]->key < heap[smallest]->key)
                smallest = left;
            if (right < size && heap[right]->key < heap[smallest]->key)
                smallest = right;

            if (smallest == pos) break;

            heap[pos] = heap[smallest];
            heap[pos]->pos = pos;
            pos = smallest;
        }
        heap[pos] = node;
        node->pos = pos;
    }

public:
    BinaryHeap(int capacity) {
        nodes.resize(capacity);
        heap.resize(capacity);
        size = 0;
    }

    void insert(int id, int key) {
        nodes[id] = HeapNode(id, key, size);
        heap[size] = &nodes[id];
        siftUp(size++);
    }

    int smallestID() {
        if (size == 0) return -1;
        int id = heap[0]->id;
        heap[0] = heap[--size];
        if (size > 0) {
            heap[0]->pos = 0;
            siftDown(0);
        }
        return id;
    }

    void decreaseKey(int id, int newKey) {
        int pos = nodes[id].pos;
        if (pos >= 0 && newKey < nodes[id].key) {
            nodes[id].key = newKey;
            siftUp(pos);
        }
    }

    bool isEmpty() const {
        return size == 0;
    }
};

class MinCostFlow {
private:
    std::vector<std::vector<EdgeInfo*>> info;  // 邻接矩阵
    std::vector<int> dist;  // 距离数组
    int sourceIndex;  // 源点
    int sinkIndex;   // 汇点
    int vertices;    // 顶点数

public:
    MinCostFlow(int n, int source, int sink)
        : vertices(n), sourceIndex(source), sinkIndex(sink) {
        info.resize(n, std::vector<EdgeInfo*>(n, nullptr));
        dist.resize(n);
    }

    bool findAugmentingPath(std::vector<VertexInfo>& vertices) {
        // 重置顶点信息
        std::fill(vertices.begin(), vertices.end(), VertexInfo());

        // 初始化二叉堆和访问数组
        BinaryHeap pq(vertices.size());
        std::vector<bool> inqueue(vertices.size(), false);

        // 初始化距离数组
        for (int u = 0; u < vertices.size(); u++) {
            if (u == sourceIndex) {
                dist[u] = 0;
                pq.insert(sourceIndex, 0);
                inqueue[u] = true;
            }
            else {
                dist[u] = std::numeric_limits<int>::max();
            }
        }

        while (!pq.isEmpty()) {
            int u = pq.smallestID();
            inqueue[u] = false;

            // 找到汇点，结束搜索
            if (u == sinkIndex) break;

            for (int v = 0; v < vertices.size(); v++) {
                if (v == sourceIndex || v == u) continue;

                // 处理前向边
                EdgeInfo* cei = info[u][v];
                if (cei != nullptr && cei->flow < cei->capacity) {
                    int newDist = dist[u] + cei->cost;
                    if (0 <= newDist && newDist < dist[v]) {
                        vertices[v] = VertexInfo(u, Search::FORWARD);
                        dist[v] = newDist;
                        if (inqueue[v]) {
                            pq.decreaseKey(v, newDist);
                        }
                        else {
                            pq.insert(v, newDist);
                            inqueue[v] = true;
                        }
                    }
                }

                // 处理后向边
                cei = info[v][u];
                if (cei != nullptr && cei->flow > 0) {
                    int newDist = dist[u] - cei->cost;
                    if (0 <= newDist && newDist < dist[v]) {
                        vertices[v] = VertexInfo(u, Search::BACKWARD);
                        dist[v] = newDist;
                        if (inqueue[v]) {
                            pq.decreaseKey(v, newDist);
                        }
                        else {
                            pq.insert(v, newDist);
                            inqueue[v] = true;
                        }
                    }
                }
            }
        }

        return dist[sinkIndex] != std::numeric_limits<int>::max();
    }

    // 添加边的方法
    void addEdge(int from, int to, int capacity, int cost) {
        info[from][to] = new EdgeInfo(0, capacity, cost);
    }
};

int main() {
    // 创建一个示例网络
    int n = 4;  // 顶点数
    int source = 0;  // 源点
    int sink = 3;    // 汇点

    MinCostFlow network(n, source, sink);

    // 添加边 (示例)
    network.addEdge(0, 1, 4, 1);  // (from, to, capacity, cost)
    network.addEdge(0, 2, 2, 2);
    network.addEdge(1, 2, 2, 1);
    network.addEdge(1, 3, 3, 3);
    network.addEdge(2, 3, 4, 2);

    // 寻找增广路径
    std::vector<VertexInfo> vertices(n);
    bool found = network.findAugmentingPath(vertices);

    if (found) {
        std::cout << "找到增广路径！" << std::endl;
        // 打印路径
        int current = sink;
        std::cout << "路径：";
        while (current != source) {
            std::cout << current << " <- ";
            current = vertices[current].predecessor;
        }
        std::cout << source << std::endl;
    }
    else {
        std::cout << "未找到增广路径" << std::endl;
    }

    return 0;
}