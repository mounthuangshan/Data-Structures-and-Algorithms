#include <iostream>
#include <vector>
#include <list>

using namespace std;

// ����ߵ�����ö��
enum edgeType { Tree, Backward, Forward, Cross };

// ���嶥����ɫö��
enum class vertexColor { white, gray, black };

// ����߱�ǩ�ṹ
struct EdgeLabel {
    int u, v;
    edgeType type;
    EdgeLabel(int _u, int _v, edgeType _type) : u(_u), v(_v), type(_type) {}
};

// ����ͼ�����ͣ�ʹ���ڽӱ��ʾ��
class Graph {
public:
    vector<vector<int>> adj;
    Graph(int n) : adj(n) {}

    void addEdge(int u, int v) {
        adj[u].push_back(v);
    }

    const vector<int>& operator[](int u) const {
        return adj[u];
    }

    auto begin(int u) const {
        return adj[u].begin();
    }

    auto end(int u) const {
        return adj[u].end();
    }
};

void dfs_visit(Graph const& graph, int u, vector<int>& d, vector<int>& f, vector<int>& pred, vector<vertexColor>& color, int& ctr, list<EdgeLabel>& labels);

// ���ʶ��� u ��������Ϣ
void dfs_visit(Graph const& graph, int u, vector<int>& d, vector<int>& f, vector<int>& pred, vector<vertexColor>& color, int& ctr, list<EdgeLabel>& labels)
{
    color[u] = vertexColor::gray; // ������ u ���Ϊ��ɫ�����ڷ��ʣ�
    d[u] = ++ctr; // ��¼����ʱ��

    for (int v : graph[u]) {
        if (color[v] == vertexColor::white) { // ������� v ��Ȼ�ǰ�ɫ��δ���ʣ�
            pred[v] = u; // ����ǰ��
            dfs_visit(graph, v, d, f, pred, color, ctr, labels); // �ݹ���� v
        }
    }

    color[u] = vertexColor::black; // ������ u ���Ϊ��ɫ����ɷ��ʣ�
    f[u] = ++ctr; // ��¼���ʱ��

    // ���������ڽӶ���
    for (auto ci = graph.begin(u); ci != graph.end(u); ++ci) {
        int v = *ci;
        edgeType type = Cross; // ��ʼ���ߵ�����Ϊ�����
        if (color[v] == vertexColor::white) {
            type = Tree; // ��� v �ǰ�ɫ����Ϊ����
        }
        else if (color[v] == vertexColor::gray) {
            type = Backward; // ��� v �ǻ�ɫ����Ϊ�ر�
        }
        else {
            if (d[u] < d[v]) {
                type = Forward; // ��� d[u] < d[v]����Ϊǰ���
            }
        }
        labels.push_back(EdgeLabel(u, v, type)); // ��ӱ߱�ǩ
    }
}

// �Ӷ��� s ��ʼִ�������������
void dfs_search(Graph const& graph, int n, vector<int>& d, vector<int>& f, vector<int>& pred, vector<vertexColor>& color, list<EdgeLabel>& labels) {
    int ctr = 0; // ���ֺ����ʱ��ļ�����
    for (int u = 0; u < n; u++) {
        if (color[u] == vertexColor::white) { // ������� u �ǰ�ɫ
            dfs_visit(graph, u, d, f, pred, color, ctr, labels); // ��ʼ����
        }
    }
}

int main() {
    // ����һ������ 5 �������ͼ
    Graph graph(5);
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(1, 3);
    graph.addEdge(1, 4);
    graph.addEdge(2, 4);

    int n = 5; // ��������
    vector<int> d(n, 0); // ����ʱ��
    vector<int> f(n, 0); // ���ʱ��
    vector<int> pred(n, -1); // ǰ��
    vector<vertexColor> color(n, vertexColor::white); // ������ɫ
    list<EdgeLabel> labels; // �߱�ǩ

    // �Ӷ��� 0 ��ʼ�����������
    dfs_search(graph, n, d, f, pred, color, labels);

    // ������ֺ����ʱ��
    cout << "����\t����ʱ��\t���ʱ��\n";
    for (int i = 0; i < n; i++) {
        cout << i << "\t" << d[i] << "\t\t" << f[i] << "\n";
    }

    // ����߱�ǩ
    cout << "\n�߱�ǩ:\n";
    for (const auto& label : labels) {
        cout << "(" << label.u << ", " << label.v << ") : " << static_cast<int>(label.type) << "\n";
    }

    return 0;
}


/*
void dfs_visit(int u, int& time, vector<int>& d, vector<int>& f, vector<int>& color, vector<vector<int>>& G) {
    color[u] = 1;
    d[u] = ++time;
    for (int v : G[u]) {
        if (color[v] == 0) {
            dfs_visit(v, time, d, f, color, G);
        }
    }
    color[u] = 2;
    f[u] = ++time;
}
*/

/*
������ dfs_visit �����ڹ����϶���ִ���������������DFS������ʵ������һЩ���ԵĲ��졣�����ǶԱȺ�˵����

1. �������ͺ�����
�϶δ����е� dfs_visit:

���ܵĲ����ж��������ͼ�Ľṹ��ǰ���ڵ����顢��ɫ���顢ʱ����������߱�ǩ�б�ȡ�
ʹ�õ���ɫ������ö�� vertexColor��������ʾδ���ʡ����ڷ��ʺ�����ɡ�
�´����е� dfs_visit:

�������٣���Ҫ�Ƕ��� u��ʱ�� time������ʱ������ d�����ʱ������ f����ɫ���� color ��ͼ G��
��ɫʹ��������0, 1, 2������ʾ״̬���ֱ��Ӧδ���ʡ����ڷ��ʺ�����ɡ�
2. ��ɫ����
�϶δ���:
ʹ�� vertexColor ö������ʾ�����״̬����ֵ gray ��ʾ���ڷ��ʣ���ֵ black ��ʾ����ɡ�
�´���:
ʹ�ü򵥵���������ʾ״̬��color[u] = 1 �������ڷ��ʣ�color[u] = 2 ��������ɡ�
3. �߱�ǩ����
�϶δ���:

�ڱ����ڽӵ�ʱ����¼�ߵ����ͣ����ߡ��رߡ�ǰ��ߡ�����ߣ���������Щ��Ϣ�洢�ڱ߱�ǩ�б��С�
�´���:

û�д���߱�ǩ��רע�ڶ���ķ��ֺ����ʱ�䡣
4. �ݹ����
�϶δ���:

�ڵݹ���� dfs_visit ֮ǰ����ǰ���ڵ����� pred��
�´���:

û�и���ǰ���ڵ����Ϣ�������Ӱ���DFS�����ؽ������������
5. ������
�϶δ���:
ʹ�ø��ḻ�Ľṹ������Ͷ��壬�����˴���Ŀɶ��Ժ�ά���ԡ�
�´���:
��Լ�ֱ�ӣ��ʺϿ���ʵ��DFS���������ں���������ȱ��ĳЩ��Ϣ��
�ܽ�
��������������ҪĿ������ͬ�ģ�����ʵ����������������϶δ����ṩ�˸���Ĺ��ܺ���Ϣ������ǰ����¼�ͱ߱�ǩ�������´�������Լ򵥣������ڻ�����DFSʵ�֡�������Ŀ���ǹ���DFS�������ͼ�Ľṹ���϶δ�����ܸ��ʺϣ����ֻ��Ҫ������DFS�������´����㹻�á�
*/






