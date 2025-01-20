#include <iostream>
#include <vector>
#include <limits>
#include <utility> // for std::pair
#include <algorithm> // for std::swap


class Graph {
public:
    using Vertexlist = std::vector<std::pair<int, int>>; // pair<destination, weight>

    // 默认构造函数
    Graph() : adjList(0) {}

    // 带参数的构造函数
    Graph(int vertices) : adjList(vertices) {}

    void addEdge(int src, int dest, int weight) {
        adjList[src].emplace_back(dest, weight);
    }

    int numVertices() const {
        return adjList.size();
    }

    Vertexlist::const_iterator begin(int u) const {
        return adjList[u].begin();
    }

    Vertexlist::const_iterator end(int u) const {
        return adjList[u].end();
    }

private:
    std::vector<Vertexlist> adjList; // adjacency list
};


class BinaryHeap {
public:
    BinaryHeap(int capacity) : heap(capacity), size(0) {}

    void insert(int vertex, int key) {
        if (size < heap.size()) {
            heap[size] = { key, vertex };
            size++;
            bubbleUp(size - 1);
        }
    }

    void decreaseKey(int vertex, long newKey) {
        for (int i = 0; i < size; ++i) {
            if (heap[i].second == vertex) {
                heap[i].first = newKey;
                bubbleUp(i);
                break;
            }
        }
    }

    int smallest() {
        return heap[0].second;
    }

    void removeMin() {
        if (size > 0) {
            heap[0] = heap[size - 1];
            size--;
            bubbleDown(0);
        }
    }

    bool isEmpty() const {
        return size == 0;
    }

private:
    std::vector<std::pair<long, int>> heap; // pair<key, vertex>
    int size;

    void bubbleUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (heap[index].first < heap[parent].first) {
                std::swap(heap[index], heap[parent]);
                index = parent;
            }
            else {
                break;
            }
        }
    }

    void bubbleDown(int index) {
        while (index * 2 + 1 < size) {
            int left = index * 2 + 1;
            int right = left + 1;
            int smallest = left;

            if (right < size && heap[right].first < heap[left].first) {
                smallest = right;
            }

            if (heap[index].first > heap[smallest].first) {
                std::swap(heap[index], heap[smallest]);
                index = smallest;
            }
            else {
                break;
            }
        }
    }
};


void singleSourceShortest(Graph const& g, int s, std::vector<int>& dist, std::vector<int>& pred) {
    const int n = g.numVertices();
    pred.assign(n, -1);
    dist.assign(n, std::numeric_limits<int>::max());
    dist[s] = 0;
    BinaryHeap pq(n);

    for (int u = 0; u < n; u++) {
        pq.insert(u, dist[u]);
    }

    while (!pq.isEmpty()) {
        int u = pq.smallest();
        pq.removeMin(); // 移除最小元素，确保它不会再次被处理

        for (Graph::Vertexlist::const_iterator ci = g.begin(u); ci != g.end(u); ++ci) {
            int v = ci->first;
            long newLen = dist[u] + ci->second;

            if (newLen < dist[v]) {
                pq.decreaseKey(v, newLen);
                dist[v] = newLen;
                pred[v] = u;
            }
        }
    }
}

int main() {
    // 创建图的实例
    Graph g(5); // 创建包含 5 个顶点的图

    // 添加边
    g.addEdge(0, 1, 4);
    g.addEdge(0, 2, 1);
    g.addEdge(1, 2, 2);
    g.addEdge(1, 3, 1);
    g.addEdge(2, 1, 2);
    g.addEdge(2, 3, 5);
    g.addEdge(3, 4, 3);

    int source = 0;  // 选择源顶点
    std::vector<int> dist;
    std::vector<int> pred;

    // 调用单源最短路径算法
    singleSourceShortest(g, source, dist, pred);

    // 打印结果
    std::cout << "Vertex\tDistance from Source\tPredecessor\n";
    for (int i = 0; i < dist.size(); ++i) {
        std::cout << i << "\t" << dist[i] << "\t\t\t" << pred[i] << "\n";
    }

    return 0;
}

