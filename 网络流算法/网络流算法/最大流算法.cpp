#include <vector>
#include <climits>
#include <algorithm>
#include <iostream>

// ǰ������
class FlowNetwork;
class Search;
class VertexInfo;

// ��ʾ������ߵĽṹ��
struct Edge {
    int capacity;  // ����
    int flow;      // ����
    Edge(int cap = 0) : capacity(cap), flow(0) {}
};

class VertexInfo {
public:
    int previous;  // ǰ���ڵ�
    bool forward;  // �Ƿ�Ϊ�����
    VertexInfo() : previous(-1), forward(true) {}
};

class FlowNetwork {
public:
    std::vector<std::vector<Edge>> edges;  // �ߵľ���
    std::vector<VertexInfo> vertices;      // ������Ϣ
    int sourceIndex;                       // Դ������
    int sinkIndex;                         // �������

    FlowNetwork(int n, int source, int sink)
        : edges(n, std::vector<Edge>(n)),
        vertices(n),
        sourceIndex(source),
        sinkIndex(sink) {
    }

    Edge& edge(int u, int v) {
        return edges[u][v];
    }
};

class Search {
public:
    virtual bool findAugmentingPath(std::vector<VertexInfo>& vertices) = 0;  // Ѱ������·��
    virtual ~Search() {}
};

class FordFulkerson {
private:
    FlowNetwork& network;  // ������
    Search& searchMethod;  // ��������

protected:
    void processPath(std::vector<VertexInfo>& vertices) {
        int v = network.sinkIndex;
        int delta = INT_MAX;  // Ѱ����С����

        // ��һ�飺�ҵ���С��ʣ������
        while (v != network.sourceIndex) {
            int u = vertices[v].previous;
            int flow;

            if (vertices[v].forward) {
                // ʹ��ʣ���������������
                flow = network.edge(u, v).capacity - network.edge(u, v).flow;
            }
            else {
                // ���ٷ���ߵ�����
                flow = network.edge(v, u).flow;
            }

            if (flow < delta) {
                delta = flow;
            }
            v = u;  // ����·��
        }

        // �ڶ��飺��������ֵ
        v = network.sinkIndex;
        while (v != network.sourceIndex) {
            int u = vertices[v].previous;
            if (vertices[v].forward) {
                network.edge(u, v).flow += delta;
            }
            else {
                network.edge(v, u).flow -= delta;
            }
            v = u;  // ����·��
        }

        // ����м�����Ϊ��һ�ε�����׼��
        std::fill(vertices.begin(), vertices.end(), VertexInfo());
    }

public:
    FordFulkerson(FlowNetwork& network, Search& method)
        : network(network), searchMethod(method) {
    }

    bool compute() {
        bool augmented = false;
        while (searchMethod.findAugmentingPath(network.vertices)) {
            processPath(network.vertices);
            augmented = true;
        }
        return augmented;
    }
};

// ���ڲ��Ե� Search ʵ��ʾ��
class SimpleSearch : public Search {
public:
    bool findAugmentingPath(std::vector<VertexInfo>& vertices) override {
        // �򻯵�ʵ�֣���������ʾ
        // ʵ��ʹ��ʱ��Ӧ�ڴ˴�ʵ�� BFS �� DFS
        return false;
    }
};

int main() {
    // ����һ������ 4 �������������
    // Դ��: 0, ���: 3
    FlowNetwork network(4, 0, 3);

    // ����һЩ�ߵ�����
    network.edge(0, 1).capacity = 10;
    network.edge(1, 2).capacity = 5;
    network.edge(2, 3).capacity = 8;

    // ������������
    SimpleSearch search;

    // ���� Ford-Fulkerson ʵ��
    FordFulkerson ff(network, search);

    // ���������
    bool success = ff.compute();

    if (success) {
        std::cout << "�ɹ����������\n";
    }
    else {
        std::cout << "δ�ҵ�����·��\n";
    }

    return 0;
}