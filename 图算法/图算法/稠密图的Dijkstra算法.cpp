#include <iostream>
#include <vector>
#include <limits>
#include <utility>
#include <map>

using namespace std;

// 定义一个简单的图结构
class Graph {
public:
    using VertexList = map<int, int>; // 邻接列表表示
    vector<VertexList> adj; // 每个顶点的邻接列表

    Graph(int n) : adj(n) {}

    void addEdge(int u, int v, int weight) {
        adj[u][v] = weight; // 添加带权边
    }

    int numVertices() const {
        return adj.size();
    }

    VertexList::const_iterator begin(int u) const {
        return adj[u].begin();
    }

    VertexList::const_iterator end(int u) const {
        return adj[u].end();
    }

    bool hasEdge(int u, int v) const {
        return adj[u].find(v) != adj[u].end();
    }
};

// 从源顶点s找到最短路径的函数
void singleSourceShortest(Graph& graph, int s, vector<int>& dist, vector<int>& pred) {
    const int n = graph.numVertices();
    pred.assign(n, -1);
    dist.assign(n, numeric_limits<int>::max());
    vector<bool> visited(n);
    dist[s] = 0;

    for (int i = 0; i < n; i++) {
        int u = -1;
        int sd = numeric_limits<int>::max(); // 假设不可达

        // 在未处理的顶点中找到距离最小的顶点
        for (int j = 0; j < n; j++) {
            if (!visited[j] && dist[j] < sd) {
                sd = dist[j];
                u = j;
            }
        }

        // 如果没有可达的顶点，退出
        if (u == -1) {
            break;
        }

        // 将顶点标记为已访问
        visited[u] = true;

        // 更新所有邻接顶点的距离
        for (auto ci = graph.begin(u); ci != graph.end(u); ++ci) {
            int v = ci->first; // 邻接节点v
            long newLen = dist[u] + ci->second; // 通过u的新路径长度
            if (newLen < dist[v]) {
                dist[v] = newLen;
                pred[v] = u;
            }
        }
    }
}

// 主函数
int main() {
    Graph graph(5); // 创建一个有5个顶点的图
    graph.addEdge(0, 1, 10);
    graph.addEdge(0, 2, 3);
    graph.addEdge(1, 2, 1);
    graph.addEdge(1, 3, 2);
    graph.addEdge(2, 1, 4);
    graph.addEdge(2, 3, 8);
    graph.addEdge(2, 4, 2);
    graph.addEdge(3, 4, 7);
    graph.addEdge(4, 3, 9);

    vector<int> dist, pred;
    int source = 0; // 起始顶点

    singleSourceShortest(graph, source, dist, pred);

    // 打印结果
    cout << "从源顶点 " << source << " 到各顶点的距离:\n";
    for (int i = 0; i < dist.size(); ++i) {
        cout << "顶点 " << i << ": " << dist[i] << ", 前驱: " << pred[i] << endl;
    }

    return 0;
}

