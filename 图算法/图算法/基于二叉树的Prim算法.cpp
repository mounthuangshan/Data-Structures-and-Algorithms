#include <iostream>
#include <vector>
#include <limits>
#include <queue>
#include <utility>
#include <unordered_map>

using namespace std;

// 邻接表的定义
using VertexList = unordered_map<int, int>; // int: 邻接顶点, int: 边权重
using AdjacencyList = vector<VertexList>;

// Graph 类定义
class Graph {
public:
    Graph(int vertices) : adjList(vertices) {}

    void addEdge(int u, int v, int weight) {
        adjList[u][v] = weight;
        adjList[v][u] = weight; // 无向图
    }

    int numVertices() const {
        return adjList.size();
    }

    VertexList::const_iterator begin(int u) const {
        return adjList[u].begin();
    }

    VertexList::const_iterator end(int u) const {
        return adjList[u].end();
    }

private:
    AdjacencyList adjList;
};

// BinaryMeap 类定义
class BinaryMeap {
public:
    BinaryMeap(int size) : size(size), heap(size), position(size, -1) {}

    void insert(int v, int key) {
        heap.push_back({ key, v });
        position[v] = heap.size() - 1;
        siftUp(heap.size() - 1);
    }

    bool isEmpty() const {
        return heap.size() == 1; // 只有哨兵元素
    }

    int samallest() {
        if (isEmpty()) return -1;
        int smallest = heap[1].second;
        swap(heap[1], heap.back());
        position[smallest] = -1;
        heap.pop_back();
        siftDown(1);
        return smallest;
    }

    void decreaseKey(int v, int newKey) {
        int index = position[v];
        if (index != -1) {
            heap[index].first = newKey;
            siftUp(index);
        }
    }

private:
    struct Node {
        int first; // key
        int second; // vertex
    };

    vector<Node> heap = { {0, -1} }; // 哨兵
    vector<int> position; // 存储顶点位置
    int size;

    void siftUp(int index) {
        while (index > 1) {
            int parent = index / 2;
            if (heap[index].first < heap[parent].first) {
                swap(heap[index], heap[parent]);
                position[heap[index].second] = index;
                position[heap[parent].second] = parent;
                index = parent;
            }
            else {
                break;
            }
        }
    }

    void siftDown(int index) {
        int size = heap.size();
        while (index * 2 < size) {
            int child = index * 2;
            if (child + 1 < size && heap[child + 1].first < heap[child].first) {
                child++;
            }
            if (heap[index].first > heap[child].first) {
                swap(heap[index], heap[child]);
                position[heap[index].second] = index;
                position[heap[child].second] = child;
                index = child;
            }
            else {
                break;
            }
        }
    }
};

//给定无向图，从随机选择的一个顶点开始构造最小生成树，我们适用pred来记录MST
// mst_prim 函数
void mst_prim(Graph const& graph, vector<int>& pred) {
    //初始化pred[]和key[]数组，优先队列PQ包含图G中所有的顶点
    const int n = graph.numVertices();
    pred.assign(n, -1);
    vector<int> key(n, numeric_limits<int>::max());
    key[0] = 0;
    BinaryMeap pq(n);
    vector<bool> inQueue(n, true);
    for (int v = 0; v < n; v++) {
        pq.insert(v, key[v]);
    }
    while (!pq.isEmpty()) {
        int u = pq.samallest();
        inQueue[u] = false;
        //处理u的所有邻接节点，检查是否存在一条边能够更新最短路径
        for (VertexList::const_iterator ci = graph.begin(u); ci != graph.end(u); ++ci) {
            int v = ci->first;
            if (inQueue[v]) {
                int w = ci->second;
                if (w < key[v]) {
                    pred[v] = u;
                    key[v] = w;
                    pq.decreaseKey(v, w);
                }
            }
        }
    }
}

// main 函数
int main() {
    // 创建图的实例
    Graph graph(5); // 例如 5 个顶点

    // 添加边
    graph.addEdge(0, 1, 2);
    graph.addEdge(0, 3, 6);
    graph.addEdge(1, 2, 3);
    graph.addEdge(1, 3, 8);
    graph.addEdge(1, 4, 5);
    graph.addEdge(2, 4, 7);
    graph.addEdge(3, 4, 9);

    // 存储最小生成树的前驱数组
    vector<int> pred;

    // 调用 Prim 算法
    mst_prim(graph, pred);

    // 打印结果
    cout << "最小生成树的边：\n";
    for (int i = 1; i < pred.size(); i++) {
        if (pred[i] != -1) {
            cout << "边: (" << pred[i] << ", " << i << ")\n";
        }
    }

    return 0;
}
