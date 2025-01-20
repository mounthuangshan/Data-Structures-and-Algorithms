#include <iostream>
#include <vector>
#include <algorithm> // for sort
#include <tuple>    // for std::tuple

using namespace std;

struct Edge {
    int src, dest, weight;
    Edge(int s, int d, int w) : src(s), dest(d), weight(w) {}
};

// �ȽϺ��������ڰ���Ȩ������
bool compareEdges(const Edge& a, const Edge& b) {
    return a.weight < b.weight;
}

class DisjointSet {
public:
    DisjointSet(int n) : parent(n), rank(n, 0) {
        for (int i = 0; i < n; ++i) {
            parent[i] = i; // ÿ���ڵ��ʼΪ�Լ�����
        }
    }

    int find(int u) {
        if (parent[u] != u) {
            parent[u] = find(parent[u]); // ·��ѹ��
        }
        return parent[u];
    }

    void unionSets(int u, int v) {
        int rootU = find(u);
        int rootV = find(v);
        if (rootU != rootV) {
            // ���Ⱥϲ�
            if (rank[rootU] < rank[rootV]) {
                parent[rootU] = rootV;
            }
            else if (rank[rootU] > rank[rootV]) {
                parent[rootV] = rootU;
            }
            else {
                parent[rootV] = rootU;
                rank[rootU]++;
            }
        }
    }

private:
    vector<int> parent; // �洢���ڵ�
    vector<int> rank;   // �洢������
};

class Graph {
public:
    Graph(int vertices) : V(vertices) {}

    void addEdge(int src, int dest, int weight) {
        edges.emplace_back(src, dest, weight);
    }

    void kruskalMST() {
        // 1. ���ߵ�Ȩ������
        sort(edges.begin(), edges.end(), compareEdges);

        DisjointSet ds(V);
        vector<Edge> mst; // �洢��С�������ı�

        // 2. �������бߣ�ѡ��Ȩ����С�Ҳ��γɻ��ı�
        for (const auto& edge : edges) {
            int u = edge.src;
            int v = edge.dest;
            if (ds.find(u) != ds.find(v)) {
                ds.unionSets(u, v); // �ϲ���������
                mst.push_back(edge); // ���뵽��С������
            }
        }

        // ��ӡ��С������
        cout << "��С�������ıߣ�\n";
        for (const auto& edge : mst) {
            cout << edge.src << " -- " << edge.dest << " (Ȩ��: " << edge.weight << ")\n";
        }
    }

private:
    int V; // ������
    vector<Edge> edges; // �ߵļ���
};

int main() {
    Graph graph(5); // ����һ������ 5 �������ͼ

    // ��ӱ�
    graph.addEdge(0, 1, 10);
    graph.addEdge(0, 2, 6);
    graph.addEdge(0, 3, 5);
    graph.addEdge(1, 3, 15);
    graph.addEdge(2, 3, 4);

    // ���� Kruskal �㷨
    graph.kruskalMST();

    return 0;
}
