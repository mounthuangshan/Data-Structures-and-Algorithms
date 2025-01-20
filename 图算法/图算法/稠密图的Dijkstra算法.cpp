#include <iostream>
#include <vector>
#include <limits>
#include <utility>
#include <map>

using namespace std;

// ����һ���򵥵�ͼ�ṹ
class Graph {
public:
    using VertexList = map<int, int>; // �ڽ��б��ʾ
    vector<VertexList> adj; // ÿ��������ڽ��б�

    Graph(int n) : adj(n) {}

    void addEdge(int u, int v, int weight) {
        adj[u][v] = weight; // ��Ӵ�Ȩ��
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

// ��Դ����s�ҵ����·���ĺ���
void singleSourceShortest(Graph& graph, int s, vector<int>& dist, vector<int>& pred) {
    const int n = graph.numVertices();
    pred.assign(n, -1);
    dist.assign(n, numeric_limits<int>::max());
    vector<bool> visited(n);
    dist[s] = 0;

    for (int i = 0; i < n; i++) {
        int u = -1;
        int sd = numeric_limits<int>::max(); // ���費�ɴ�

        // ��δ����Ķ������ҵ�������С�Ķ���
        for (int j = 0; j < n; j++) {
            if (!visited[j] && dist[j] < sd) {
                sd = dist[j];
                u = j;
            }
        }

        // ���û�пɴ�Ķ��㣬�˳�
        if (u == -1) {
            break;
        }

        // ��������Ϊ�ѷ���
        visited[u] = true;

        // ���������ڽӶ���ľ���
        for (auto ci = graph.begin(u); ci != graph.end(u); ++ci) {
            int v = ci->first; // �ڽӽڵ�v
            long newLen = dist[u] + ci->second; // ͨ��u����·������
            if (newLen < dist[v]) {
                dist[v] = newLen;
                pred[v] = u;
            }
        }
    }
}

// ������
int main() {
    Graph graph(5); // ����һ����5�������ͼ
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
    int source = 0; // ��ʼ����

    singleSourceShortest(graph, source, dist, pred);

    // ��ӡ���
    cout << "��Դ���� " << source << " ��������ľ���:\n";
    for (int i = 0; i < dist.size(); ++i) {
        cout << "���� " << i << ": " << dist[i] << ", ǰ��: " << pred[i] << endl;
    }

    return 0;
}

