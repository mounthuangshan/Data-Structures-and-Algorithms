#include <iostream>
#include <vector>
#include <limits>
#include <list>
#include <utility> // for std::pair
#include <stdexcept>

// ����һ���ڽ��б������
using VertexList = std::vector<std::list<std::pair<int, int>>>; // pair<�ڽӶ���, Ȩ��>

// Graph �ඨ��
class Graph {
private:
    VertexList adjList; // �ڽ��б�

public:
    // ���캯��
    Graph(int numVertices) : adjList(numVertices) {}

    // ��ӱߵķ���
    void addEdge(int u, int v, int weight) {
        adjList[u].emplace_back(v, weight); // ��ӱ� (u, v) ����Ȩ��
    }

    // ��ȡ��������
    int numVertices() const {
        return adjList.size();
    }

    // ����ָ��������ڽ��б�ĵ�����
    auto begin(int u) const {
        return adjList[u].begin();
    }

    auto end(int u) const {
        return adjList[u].end();
    }
};

// ��Դ���·���㷨��Bellman-Ford�㷨ʵ�֣�
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
                long newLen = dist[u] + ci->second; // ��·������
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

// ������
int main() {
    // ����ͼʵ����������5������
    Graph graph(5);

    // ���һЩ�� (u, v, weight)
    graph.addEdge(0, 1, 6);
    graph.addEdge(0, 2, 7);
    graph.addEdge(1, 2, 8);
    graph.addEdge(1, 3, 5);
    graph.addEdge(1, 4, -4);
    graph.addEdge(2, 3, -3);
    graph.addEdge(3, 4, 9);
    graph.addEdge(4, 1, 2);

    int sourceVertex = 0; // ��ʼ����
    std::vector<int> dist; // �洢��̾���
    std::vector<int> pred; // �洢ǰ��ڵ�

    try {
        singleSourceShortest(graph, sourceVertex, dist, pred);

        // ������
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

